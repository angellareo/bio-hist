//      SpikeDetector.h
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

#ifndef __SPIKE_DETECTOR_H__
#define __SPIKE_DETECTOR_H__

#include <iostream>

class SpikeDetector
{
protected:
    double _filterTime;
    double _vThreshold;
    double _lastSpkTime = 0.0;
    
public:
    virtual int detectSpike (double voltage, double time) = 0;
};

#endif /* __SPIKE_DETECTOR_H__ */
