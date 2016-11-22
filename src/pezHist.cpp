//      pexHist.cpp
//      
//      Copyright 2015 Ángel Lareo <angel.lareo@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <memory>
#include <fstream>

#include "pezHist.h"
#include "SignalProcessor.h"
#include "ErrorFilter.h"
#include "WordHistGenerator.h"

using namespace std;

int main(int argc, char* argv[]){
	vector< shared_ptr<ErrorFilter> > EFilters;
	vector< shared_ptr<WordHistGenerator> > HistGens;
    vector<float> binTimes;
	shared_ptr<ProblemConfig> info;
	
	if ((argc!=2)&&(argc!=4)){
		cout << "Error: Number of arguments does not match definition." << endl;
		return 0;
	}
	

	ifstream test(argv[1]);
	if (!test.good()){
		cout << "Error: Bad config file" << endl;
		return 0;
	}


    if (argc!=4){
		info = make_shared<ProblemConfig> (ProblemConfig(argv[1]));
	}
	else{
		info = make_shared<ProblemConfig> (ProblemConfig(argv[1], argv[2], argv[3]));
	}

    HDF5HistWriter H5FileWriter(info->getOutputFileName(), info->getWordLengths(), info->getBinTimes());    

    H5FileWriter.writeProblemInfo(info);
    

    shared_ptr<SignalProcessor> sp = shared_ptr<SignalProcessor>(new SignalProcessor(info));
    
    for (double bT : info->getBinTimes()){
        shared_ptr<ErrorFilter> errorFilter(new ErrorFilter(bT, info->getTotalTime(), sp));
        EFilters.push_back(errorFilter);
        for (int wL : info->getWordLengths()){
            shared_ptr<WordHistGenerator> wordHistGen(new WordHistGenerator(wL,errorFilter));
            HistGens.push_back(wordHistGen);
        }
    }
    
    sp->run();
    

    for (auto histGen : HistGens){
        auto hist = histGen->getHist();
        hsize_t histDims[2];
        histDims[0]=histGen->getMaxWords(); histDims[1]=2;
        
        pair<float,int> par(histGen->getBinTime(),histGen->getWordLength());
        
        H5FileWriter.writeHistData(hist, par, histDims);
        H5FileWriter.writeEntropy(histGen->getEntropy()/histGen->getWordLength(), 
                                        histGen->getBinTime(), histGen->getWordLength());
		//H5FileWriter.writeBias(histGen->getBias(),
                                        //histGen->getBinTime(), histGen->getWordLength());
        //H5FileWriter.writeCorrectedEntropy(histGen->getCorrectedEntropy(),
                                        //histGen->getBinTime(), histGen->getWordLength());
    }
    
    for (auto eFilter : EFilters){
        H5FileWriter.writeErrors(eFilter->getErrors(), eFilter->getBitErrors(), eFilter->getBinTime());
    }
  
    return 0;
}
