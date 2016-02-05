//      DowncrossSpikeDetector.h
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

#ifndef __DOWNCROSS_SPIKE_DETECTOR_H__
#define __DOWNCROSS_SPIKE_DETECTOR_H__

#include <iostream>
#include <SpikeDetector.h>

class DowncrossSpikeDetector : public SpikeDetector
{
private:
	double _lastVol1=0.0;
    
public:
    DowncrossSpikeDetector(double vThreshold, double filterTime);
    virtual ~DowncrossSpikeDetector();
    int detectSpike (double voltage, double time);
};

DowncrossSpikeDetector::DowncrossSpikeDetector(double vThreshold= 7.0, double filterTime = 1.0){
    _vThreshold = vThreshold;
    _filterTime = filterTime;
}
    
DowncrossSpikeDetector::~DowncrossSpikeDetector(){

}
    
int DowncrossSpikeDetector::detectSpike (double voltage, double time){
    int spike=0;
    
    if ((_lastVol1>=_vThreshold) &&
        (voltage<_vThreshold) &&
        (time-_lastSpkTime > _filterTime))
    {
        spike=1;
        _lastSpkTime = time;
    }

    _lastVol1 = voltage;

    return spike;
}

#endif /* __DOWNCROSS_SPIKE_DETECTOR_H__ */
