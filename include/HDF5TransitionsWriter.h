//      HDF5TransitionsWriter.h
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

//@todo Doxygen comments
#ifndef __HDF5_TRANSITIONS_WRITER_H__
#define __HDF5_TRANSITIONS_WRITER_H__

#include "H5Cpp.h"
#include "Statistics.h"
#include "MatrixStreamer.h"
#include "ProblemConfig.h"
#include "Transitions.h"
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <string>

class HDF5TransitionsWriter
{
private:
	libStats::Statistics _transitionsStats;
	std::string _fileName;
	libStats::MatrixStreamer::SharedPtr _transitionsStreamer;
	libStats::MatrixStreamer::SharedPtr _binTimeStreamer;
	libStats::MatrixStreamer::SharedPtr _wordLengthStreamer;
    
public:
	typedef std::pair<float, int> floatIntPair;
	
	HDF5TransitionsWriter(std::string fileName, std::vector<int> wordLengths, std::vector<float> binTimes) : _transitionsStats(fileName, "Transitions"),
																							  				 _fileName(fileName){
		hsize_t wordLengthDims[2];
		hsize_t binTimeDims[2];
		//hsize_t transitionDims[2];
		
		binTimeDims[0]=binTimes.size();			binTimeDims[1]=1;
		wordLengthDims[0]=wordLengths.size();		wordLengthDims[1]=1;
		//transitionsDims[0]=maxWords;	transitionsDims[1]=maxWords;
	    
	    //todo: Move to problem info
		_binTimeStreamer = _transitionsStats.get_matrix_streamer(binTimeDims, "BinTimes");
		_wordLengthStreamer = _transitionsStats.get_matrix_streamer(wordLengthDims, "WordLengths");

		//_transitionsStreamer = _transitionsStats.get_matrix_streamer(entropyDims, "Transitions");

		for (int i : wordLengths){
			*_wordLengthStreamer << i;
		}
		
		for (float i:binTimes){
			*_binTimeStreamer << i;
		}
	}
	virtual ~HDF5TransitionsWriter(){
		_transitionsStats.dump();
    }
	
	void writeProblemInfo(std::shared_ptr<ProblemConfig> info){
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

	//void writeBintime(float binTime);
	//void writeWordLength(int wordLength);

	void writeTransitions(boost::numeric::ublas::matrix<float>&& transitionsMatrix, std::pair<float, int> GroupName, hsize_t transDims[2]){
		libStats::Statistics stats(_fileName, std::string("Transitions_" + 
														  boost::lexical_cast<std::string>(GroupName.first) + "_" +
														  boost::lexical_cast<std::string>(GroupName.second)));
	
		auto streamer = stats.get_matrix_streamer(transDims, "Data");
		for (unsigned i = 0; i < transitionsMatrix.size1(); ++i){
            for (unsigned j = 0; j < transitionsMatrix.size2(); ++j){
				*streamer << transitionsMatrix(i,j);
			}
		}
		stats.dump();
	}
};

#endif //__HDF5_TRANSITIONS_WRITER_H__
