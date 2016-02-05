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

//@todo: get config file from command line


//@todo move program logic to specific classes: Keep main only to initialize GUI & classes
int main(int argc, char* argv[]){
  /*  //float entropy = 0.0;
	vector< shared_ptr<ErrorFilter> > EFilters;
	vector< shared_ptr<WordHistGenerator> > HistGens;
    vector<float> binTimes;
	shared_ptr<ProblemConfig> info;
	
	if ((argc!=2)&&(argc!=4)){
		cout << "Error: Number of arguments does not match definition." << endl;
		return 0;
	}
	
	//@todo: move to ProblemConfig class + exception
	ifstream test(argv[1]);
	if (!test.good()){
		cout << "Error: Bad config file" << endl;
		return 0;
	}

    //@todo: read config filename from command line
    if (argc!=3){
		info = make_shared<ProblemConfig> (ProblemConfig("config.yaml"));
	}
	else{
		info = make_shared<ProblemConfig> (ProblemConfig("config.yaml", argv[1], argv[2]));
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
    
    //@todo: move to a class -> printResults
    for (auto histGen : HistGens){
        auto hist = histGen->getHist();
        hsize_t histDims[2];
        histDims[0]=histGen->getMaxWords(); histDims[1]=2;
        
        pair<float,int> par(histGen->getBinTime(),histGen->getWordLength());
        
        H5FileWriter.writeHistData(hist, par, histDims);
        H5FileWriter.writeEntropy(histGen->getEntropy()/histGen->getWordLength(), 
                                        histGen->getBinTime(), histGen->getWordLength());
    }
    
    for (auto eFilter : EFilters){
        H5FileWriter.writeErrors(eFilter->getErrors(), eFilter->getBitErrors(), eFilter->getBinTime());
    }*/
    
    return 0;
}
