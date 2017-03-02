//      ErrorFilter.cpp
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

#include <iostream>
#include <cmath>
#include <ErrorFilter.h>
#include <SpikesObserver.h>

using namespace std;

ErrorFilter::ErrorFilter(double binTime, double endTime, shared_ptr<SignalProcessor> mod) : SpikesObserver(mod, binTime){
    _endTime = endTime;
}

void ErrorFilter::update(double time){
    if (time == _endTime){
        notify (BIT_END);
        return;
    }
    
    if (floor(time/_binTime) < _lastBitNum){
        cout << "<ErrorFilter> UPDATE ERROR: Decreasing time." << endl; //@ToDo: Exception!
    }
    
    if (floor(time/_binTime) == _lastBitNum){
        _errors++;
        //cout << "err" << endl;
        if (_lastBit != BIT_ERROR) _bitErrors++;
        _lastBit = BIT_ERROR;
    }
    
    if (floor(time/_binTime) > _lastBitNum){
        notify(_lastBit);

        for (int i = 1; i < (floor(time/_binTime) - _lastBitNum); i++){
            notify(0);
        }
        
        _lastBit = 1;
        _lastBitNum = floor(time/_binTime);
    }
}
