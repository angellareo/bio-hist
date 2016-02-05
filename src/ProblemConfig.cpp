//      problemConfig.cpp
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

#include <ProblemConfig.h>

using namespace std;

ProblemConfig::ProblemConfig(string fileName){
    _config = YAML::LoadFile(fileName);
}

ProblemConfig::ProblemConfig(string fileName, string inputFile, string outputFile):
															_inputFile(inputFile),
															_outputFile(outputFile){
    _config = YAML::LoadFile(fileName);
}

ProblemConfig::~ProblemConfig(){

}

double ProblemConfig::getVolThreshold(){
    double vThr;
    vThr = _config["volThreshold"].as<double>();
    return vThr;
}

int ProblemConfig::getVolColumn(){
    int vCol;
    vCol = _config["volColumn"].as<int>();
    return vCol;
}

double ProblemConfig::getTotalTime(){
    double tTime;
    tTime = _config["totalTime"].as<double>() * 1000;
    return tTime;
}

std::string ProblemConfig::getSpkDetType(){
    return _config["spikeDetector"].as<string>();
}

vector<int> ProblemConfig::getWordLengths(){
    vector<int> vAux;
    for(YAML::const_iterator it=_config["wordLengths"].begin(); it!=_config["wordLengths"].end();++it)
        vAux.push_back(it->as<int>());
    return vAux;
}

vector<float> ProblemConfig::getBinTimes(){
    vector<float> vAux;
    for(YAML::const_iterator it=_config["binTimes"].begin(); it!=_config["binTimes"].end();++it)
        vAux.push_back(it->as<float>());
    return vAux;
}

int ProblemConfig::getNumWL(){
    return _config["wordLengths"].size();
}

string  ProblemConfig::getInputFileName(){
	if (_inputFile.empty())
		return _config["inputFile"].as<string>();
	else
		return _inputFile;
}

string  ProblemConfig::getOutputFileName(){
	if (_outputFile.empty())
		return _config["outputFile"].as<string>();
	else
		return _outputFile;
}

int ProblemConfig::getNumBT(){
    return _config["binTimes"].size();
}


