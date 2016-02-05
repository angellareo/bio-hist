//      SignalProcessor.cpp
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

#include <iostream>
#include <memory>
#include "SignalProcessor.h"
#include "UpcrossSpikeDetector.h"
#include "DowncrossSpikeDetector.h"
#include "FormSpikeDetector.h"

using namespace std;

SignalProcessor::SignalProcessor(shared_ptr<ProblemConfig> info){
    _info = info;
    
    string type = _info->getSpkDetType();
    
    if (type.compare("Upcross"))//@todo: constant string
        _spkDetector = unique_ptr<SpikeDetector>(new UpcrossSpikeDetector(info->getVolThreshold(),1.0));
    if (type.compare("Downcross"))//@todo: constant string
        _spkDetector = unique_ptr<SpikeDetector>(new DowncrossSpikeDetector(info->getVolThreshold(),1.0));
    if (type.compare("FormSpike"))//@todo: constant string
        _spkDetector = unique_ptr<SpikeDetector>(new FormSpikeDetector(info->getVolThreshold(),1.0));
    
}

SignalProcessor::~SignalProcessor(){
}

void SignalProcessor::run(){
    string auxStr;

    //@todo: Check file existence
    if (_info->getInputFileName() == ""){
        cout << "No Input File Name." << endl;
        throw -1;
    }

    _inputFile.open (_info->getInputFileName(), ios::in | ios::binary);
    if(!_inputFile){
        cout << "Input File Not Found." << endl;
        throw -1;
    }

    _inputFile.clear() ;
	_inputFile.seekg(0, ios::beg) ;
    
    getline (_inputFile, auxStr);
	vector<string> partsAux;
	string space(" ");
	split (auxStr, space, partsAux); 
    
    _initTime = atof(partsAux[0].c_str());
    
    while (!_inputFile.eof()){
		vector<string> parts;
        split (auxStr, space, parts);

        if (END_HIST_TIME == histogramModel(atof(parts[_info->getVolColumn()-1].c_str()), atof(parts[0].c_str()))){
			break;
		}
		
		getline (_inputFile, auxStr);
    }
    
    _inputFile.close();
}

int SignalProcessor::histogramModel(double currentV, double time){
    int spk = _spkDetector->detectSpike(currentV,time);
    if (spk==1){
        notify(time-_initTime);
    }
    if ((time - _initTime) >= _info->getTotalTime()){
        notify(_info->getTotalTime());
        return END_HIST_TIME;
    }
    return spk;
}

int SignalProcessor::histogramModel(double currentV, double time, double endTime){
    int spk = _spkDetector->detectSpike(currentV,time);
    if (spk==1){
        notify(time-_initTime);
    }
    if ((time - _initTime) >= endTime){
        notify(endTime);
        return END_HIST_TIME;
    }
    return spk;
}

void SignalProcessor::notify(double time) {
  for (auto v : _views)
    v->update(time);
}

void SignalProcessor::split(const string& str, const string& delim, vector<string>& parts) {
  size_t start, end = 0;
  while (end < str.size()) {
    start = end;
    while (start < str.size() && (delim.find(str[start]) != string::npos)) {
      start++;  // skip initial whitespace
    }
    end = start;
    while (end < str.size() && (delim.find(str[end]) == string::npos)) {
      end++; // skip to end of word
    }
    if (end-start != 0) {  // just ignore zero-length strings.
      parts.push_back(string(str, start, end-start));
    }
  }
}
