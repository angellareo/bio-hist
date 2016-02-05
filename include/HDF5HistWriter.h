//      HDF5HistWriter.h
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

//@todo Doxygen comments
#ifndef __HDF5_HIST_WRITER_H__
#define __HDF5_HIST_WRITER_H__

#include "H5Cpp.h"
#include "Statistics.h"
#include "MatrixStreamer.h"
#include "ProblemConfig.h"

class HDF5HistWriter
{
private:
	libStats::Statistics _entropyStats;
   	libStats::Statistics _errorsStats;
	std::string _fileName;
	libStats::MatrixStreamer::SharedPtr _entropyStreamer;
	libStats::MatrixStreamer::SharedPtr _binTimeStreamer;
	libStats::MatrixStreamer::SharedPtr _wordLengthStreamer;
    libStats::MatrixStreamer::SharedPtr _errorsStreamer;
    
public:
	typedef std::vector< std::pair<int,int> > histVector;
	typedef std::pair<float, int> floatIntPair;
	
	HDF5HistWriter(std::string fileName, std::vector<int> wordLengths, std::vector<float> binTimes);
	virtual ~HDF5HistWriter();
	
	void writeProblemInfo(std::shared_ptr<ProblemConfig> info);

	void writeBintime(float binTime);
	void writeWordLength(int wordLength);

	void writeEntropy(float entropy, float binTime, int wordLength);
    void writeErrors(int errors, int bitErrors, float binTime);
	void writeHistData(histVector orderedHist, floatIntPair GroupName, hsize_t histDims[2]);
};

#endif //__HDF5_HIST_WRITER_H__
