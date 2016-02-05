#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>

//      HDF5HistWriter.cpp
//      
//      Copyright 2013 Ángel Lareo <angel.lareo@gmail.com>
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

#include <boost/lexical_cast.hpp>

#include "HDF5HistWriter.h"

using namespace std;
using namespace libStats;

HDF5HistWriter::HDF5HistWriter(std::string fileName, vector<int> wordLengths, vector<float> binTimes) : 
									_entropyStats(fileName, "Entropy"), _errorsStats(fileName, "Errors"),_fileName(fileName){
	hsize_t wordLengthDims[2];
	hsize_t binTimeDims[2];
	hsize_t entropyDims[2];
    hsize_t errorDims[2];
	
	binTimeDims[0]=binTimes.size();			binTimeDims[1]=1;
	wordLengthDims[0]=wordLengths.size();		wordLengthDims[1]=1;
	entropyDims[0]=binTimeDims[0]*wordLengthDims[0];	entropyDims[1]=3;
	errorDims[0]=binTimeDims[0];	errorDims[1]=3;
    
    //todo: Move to problem info
	_binTimeStreamer = _entropyStats.get_matrix_streamer(binTimeDims, "BinTimes");
	_wordLengthStreamer = _entropyStats.get_matrix_streamer(wordLengthDims, "WordLengths");

	_entropyStreamer = _entropyStats.get_matrix_streamer(entropyDims, "Data");
    
    _errorsStreamer = _errorsStats.get_matrix_streamer(errorDims, "Data");

	for (int i : wordLengths){
		*_wordLengthStreamer << i;
	}
	
	for (float i:binTimes){
		*_binTimeStreamer << i;
	}
}

HDF5HistWriter::~HDF5HistWriter(){
	_entropyStats.dump();
    _errorsStats.dump();
}

void HDF5HistWriter::writeProblemInfo(shared_ptr<ProblemConfig> info){
	hsize_t auxDims[2] = {1, 1};
	libStats::Statistics stats(_fileName, "ProblemInfo");
	auto streamer = stats.get_matrix_streamer(auxDims, "vThreshold");
	*streamer << info->getVolThreshold();
	streamer = stats.get_matrix_streamer(auxDims, "maxTime");
	*streamer << info->getTotalTime();
	streamer = stats.get_matrix_streamer(auxDims, "volColumn");
	*streamer << info->getVolColumn();
	stats.dump();
}

void HDF5HistWriter::writeBintime(float binTime){
	*_binTimeStreamer << binTime;
}

void HDF5HistWriter::writeWordLength(int wordLength){
	*_wordLengthStreamer << wordLength;
}

void HDF5HistWriter::writeEntropy(float entropy, float binTime, int wordLength){
	*_entropyStreamer << entropy << binTime << wordLength;
}

void HDF5HistWriter::writeHistData(histVector orderedHist, std::pair<float, int> GroupName, hsize_t histDims[2]){
	libStats::Statistics stats(_fileName, string("Hist_" + boost::lexical_cast<string>(GroupName.first) + "_" + boost::lexical_cast<string>(GroupName.second)));
	
	auto streamer = stats.get_matrix_streamer(histDims, "HistData");
	for(pair<int,int> words_pair : orderedHist){
		if (words_pair.first==0) break;
		//cout << words_pair.second << " " << words_pair.first << endl;
		*streamer << words_pair.second << words_pair.first;  //second=Word  first=#
	}
	stats.dump();
}

void HDF5HistWriter::writeErrors(int errors, int bitErrors, float binTime){
    *_errorsStreamer << errors << bitErrors << binTime;
}
