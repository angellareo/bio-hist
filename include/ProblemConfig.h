//      problemConfig.h
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

#ifndef __PROBLEM_CONFIG_H__
#define __PROBLEM_CONFIG_H__

#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <string>

class ProblemConfig
{
private:
	YAML::Node _config;
    std::string _inputFile;
	std::string _outputFile;
    
public:
	ProblemConfig(std::string fileName);
	ProblemConfig(std::string fileName, std::string inputFile, std::string outputFile);

	virtual ~ProblemConfig();
    
    double getVolThreshold();
    std::vector<int> getWordLengths();
    std::vector<float> getBinTimes();
    std::string getSpkDetType();
    int getNumWL();
    int getNumBT();
    std::string getInputFileName();
    std::string getOutputFileName();
    double getTotalTime();
    int getVolColumn();
};

#endif /* __PROBLEM_CONFIG_H__ */
