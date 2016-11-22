#ifndef __PROBLEM_INFO__
#define __PROBLEM_INFO__

typedef struct{
	const char* inputFileName, *outputFileName;
	int wordLength, volColumn, numBT, numWL, maxWords;
	double vThreshold, binTime, maxTime;
} ProblemInfo;

#endif
