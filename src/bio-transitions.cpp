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

#include "SignalProcessor.h"
#include "ErrorFilter.h"
#include "WordHistGenerator.h"
#include "Transitions.h"

#include "HDF5HistWriter.h"
#include "HDF5TransitionsWriter.h"

#include <boost/numeric/ublas/io.hpp> 

using namespace std;

static void show_usage(string name){
    std::cerr << "Usage: " << name << " <option(s)> SOURCES" << endl
              << "Options:\n"
              << "\t-h,--help\t\tShow this message.\n"
              << "\t-c,--config\t\tConfig file.\n"
              << "\t-i,--input\t\tInput file.\n"
              << "\t-o,--output\t\tOutput file\n" << endl;
}


int main(int argc, char* argv[]){
    int c;
    string configFilename, inputFilename, outputFilename;
	vector< shared_ptr<ErrorFilter> > EFilters;
	vector< shared_ptr<BinSignalGenerator> > HistGens;
    vector< shared_ptr<Transitions> > TransitionGens;
    vector<float> binTimes;
	shared_ptr<ProblemConfig> info;

    while ((c = getopt(argc,argv,"c:i:o:")) != -1) { //@todo: use long options
        switch (c){
            case 'h':
                show_usage(argv[0]);
                break;
            case 'c':
                configFilename += optarg;
                break;
            case 'i':
                inputFilename += optarg;
                break;
            case 'o':
                outputFilename += optarg;
                break;
        }
    }

    if (configFilename.empty()){
        cerr << "Error: Required CONFIG file" << endl;
        show_usage(argv[0]);
    }

    ifstream test(configFilename);
    if (!test.good()){
        cerr << "Error: Cannot find CONFIG file" << endl;
        return 0;
    }
    test.close();
    test.open(inputFilename);
    if (!test.good()){
        cerr << "Error: Cannot find INPUT file " << endl;
        return 0;
    }

    //@todo: detect if outputFile exists
    //@todo: if outputFile exists -> rename
    
    if (outputFilename.empty()){
		info = make_shared<ProblemConfig> (ProblemConfig(configFilename));
	}
	else{
		info = make_shared<ProblemConfig> (ProblemConfig(configFilename, inputFilename, outputFilename));
	}

    HDF5TransitionsWriter H5FileWriter(info->getOutputFileName(), info->getWordLengths(), info->getBinTimes());    

    H5FileWriter.writeProblemInfo(info);
    

    SignalProcessor sp(info);
    
    for (double bT : info->getBinTimes()){
        shared_ptr<ErrorFilter> errorFilter = make_shared<ErrorFilter>(bT, info->getTotalTime(), sp);
        EFilters.push_back(errorFilter);
        sp.attach(errorFilter);
        for (int wL : info->getWordLengths()){
            shared_ptr<BinSignalGenerator> binSignalGen = make_shared<BinSignalGenerator>(wL,errorFilter);
            errorFilter->attach(binSignalGen);
            HistGens.push_back(binSignalGen);
            
            shared_ptr<Transitions> transitionGen = make_shared<Transitions>(binSignalGen, binSignalGen->getMaxWords());
            binSignalGen->attach(transitionGen);
            TransitionGens.push_back(transitionGen);
        }
    }
    
    sp.run();

    for (auto transGen : TransitionGens){
        //auto tran = transGen->getTransitionProbabilities();
        //cout << tran(0,0) << endl;
        //transGen->printTransitionMatrix();
        
        hsize_t transitionMatrixDims[2];
        transitionMatrixDims[0]=transGen->getNStates(); transitionMatrixDims[1]=transGen->getNStates();
        
        pair<float,int> par(transGen->getSubject()->getBinTime(),transGen->getSubject()->getWordLength());
        
        

        H5FileWriter.writeTransitions(move(transGen->getTransitionProbabilities()), par, transitionMatrixDims);
        //H5FileWriter.writeEntropy(histGen->getEntropy()/histGen->getWordLength(), 
                                        //histGen->getBinTime(), histGen->getWordLength());
		//H5FileWriter.writeBias(histGen->getBias(),
                                        //histGen->getBinTime(), histGen->getWordLength());
        //H5FileWriter.writeCorrectedEntropy(histGen->getCorrectedEntropy(),
                                        //histGen->getBinTime(), histGen->getWordLength());
    }
    
    /*for (auto eFilter : EFilters){
        H5FileWriter.writeErrors(eFilter->getSpkErrors(), eFilter->getBitErrors(), eFilter->getBinTime());
    }*/
  
    return 0;
}
