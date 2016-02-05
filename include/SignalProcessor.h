//      SignalProcessor.h
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

#ifndef __SIGNAL_PROCESSOR_H__
#define __SIGNAL_PROCESSOR_H__

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>

#include "SpikeDetector.h"
#include "SpikesObserver.h"
#include "ProblemConfig.h"

class SignalProcessor
{
private:
    const int END_HIST_TIME = -2;
    std::vector<SpikesObserver*> _views;
	std::ifstream _inputFile;
	double _initTime;
    double _endTime;
    std::unique_ptr<SpikeDetector> _spkDetector;
    std::shared_ptr<ProblemConfig> _info;
    void split(const std::string& str, const std::string& delim, std::vector<std::string>& parts);
    
public:
    SignalProcessor(std::shared_ptr<ProblemConfig> info);
    virtual ~SignalProcessor();
    void attach(SpikesObserver *obs) {
        _views.push_back(obs);
    }
    void notify(double time);
    void run();
    int histogramModel(double currentV, double time);
    int histogramModel(double currentV, double time, double endTime);
};

#endif /* __SIGNAL_PROCESSOR_H__ */
