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
#include "BinWordsSignalGenerator.h"
#include "Statistics.h"
#include "Transitions.h"

using namespace std;

//@todo: get config file from command line


//@todo move program logic to specific classes: Keep main only to initialize GUI & classes
int main(int argc, char* argv[]){
    //float entropy = 0.0;
	vector< shared_ptr<ErrorFilter> > EFilters;
	vector< shared_ptr<BinWordsSignalGenerator> > BinWordsSignalGens;
	vector<float> binTimes;
	shared_ptr<ProblemConfig> info;
	//libStats::Statistics TransitionMatrixStats;

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
    if (argc!=4){
		info = make_shared<ProblemConfig> (ProblemConfig(argv[1]));
	}
	else{
		info = make_shared<ProblemConfig> (ProblemConfig(argv[1], argv[2], argv[3]));
	}

    shared_ptr<SignalProcessor> sp = shared_ptr<SignalProcessor>(new SignalProcessor(info));
    
    for (double bT : info->getBinTimes()){
        shared_ptr<ErrorFilter> errorFilter(new ErrorFilter(bT, info->getTotalTime(), sp));
        EFilters.push_back(errorFilter);
        for (int wL : info->getWordLengths()){
            shared_ptr<BinWordsSignalGenerator> BinWordsSignalGen(new BinWordsSignalGenerator(wL, errorFilter));
	        BinWordsSignalGens.push_back(BinWordsSignalGen);
        }
    }
    
    sp->run();
    
    for (auto signal : BinWordsSignalGens){
        vector<int> v = signal->getBinWordsSignal();
        Transitions tObj(v, 16);
        //hsize_t matrixDims[2];
        //histDims[0]=; histDims[1]=;
        //cout << v[0] << "..." << v.back() << endl;
    }
    
    //for (auto eFilter : EFilters){
        //H5FileWriter.writeErrors(eFilter->getErrors(), eFilter->getBitErrors(), eFilter->getBinTime());
    //}
    
    return 0;
}
