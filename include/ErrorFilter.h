//      ErrorFilter.h
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

#ifndef __ERROR_FILTER_H__
#define __ERROR_FILTER_H__

#include <iostream>
#include <memory>
#include <cmath>
#include <SpikesObserver.h>
#include <SignalProcessor.h>

class ErrorFilter : public SpikesObserver
{
private:
    const int BIT_ERROR=-1;
    const int BIT_END=-2;
    
    double _endTime;
    long int _lastBitNum = -1;
    int _lastBit = -1;
    
    int _errors=0;
    int _bitErrors=0;
    
public:
    ErrorFilter(double binTime, double endTime, SignalProcessor& mod);
    virtual ~ErrorFilter(){}

    void update(double time);
    
    int getErrors(){
        return _errors;
    }
    
    int getBitErrors(){
        return _bitErrors;
    }    
};

#endif /* __ERROR_FILTER_H__ */
