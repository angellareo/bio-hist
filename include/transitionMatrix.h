#ifndef __PEZ_HIST__
#define __PEZ_HIST__

#define BIT_DETECTED_OUT 2
#define BIT_NOT_DETECTED_OUT 0
#define END_WINDOW_OUT 1
#define WORD_DETECTED_OUT 3

#define END_HISTOGRAM_TIME -2

#define FIRST_CALL 0
#define NEW_TIME_WINDOW 1

#define FALSE 0
#define TRUE 1

#include "HDF5HistWriter.h"
#include "ProblemInfo.h"
#include "ProblemConfig.h"

extern "C" {
	#include "wordsBuffer.h"
}

//template <typename T,unsigned S>
//inline unsigned arraysize(const T (&v)[S]) { return S; }

void readInfoArguments(char* argv[]);
void updateParameters(float binTime, int wordLength);
void print2File(HDF5HistWriter &H5FileWriter, std::vector< std::pair<int,int> > pairVector, float entropy);
void fileHistSimulation(WordsBuffer *wb);
void orderHistVector(std::vector<int> &wordsVec, std::vector< std::pair<int,int> >& wordsPosVector);
void calculateEntropy(std::vector<int> &probs, int n, float& entropy);
void split(const std::string& str, const std::string& delim, std::vector<std::string>& parts);
double histogramModel(WordsBuffer *wb, double currentV, double time, double initTime);
int detect_spike (double voltage, double time);
int detect_spike_2 (double voltage, double time);

#endif
