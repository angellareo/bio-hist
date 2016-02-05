//      Transitions.h
//      
//      Copyright 2014 Ángel Lareo <angel.lareo@gmail.com>
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

#ifndef __BIN_WORDS_TRANSITIONS_H__
#define __BIN_WORDS_TRANSITIONS_H__

#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>

class Transitions
{
private:
    boost::numeric::ublas::matrix<double> _initial;
    boost::numeric::ublas::matrix<double> _probabilities;

public:
    Transitions(std::vector<int> signal, int numberOfStates)
    {
        _probabilities = boost::numeric::ublas::matrix<double>(numberOfStates,numberOfStates);
        
        for (unsigned i = 0; i < _probabilities.size1(); ++i)
            for (unsigned j = 0; j < _probabilities.size2(); ++j){
                _probabilities(i,j)=0;
        }
        
        /*
        for (unsigned i = 0; i < (signal.size()-1); ++i){
            _probabilities (signal[i], signal[i+1]) += (1.0/(signal.size()-1));
            std::cout << "(" << signal[i] << "," << signal[i+1] << ")" << std::endl;
        }*/

        for (unsigned i = 0; i < _probabilities.size1(); ++i){
            for (unsigned j = 0; j < _probabilities.size2(); ++j){
                std::cout << _probabilities(i,j) << " ";
            }
            std::cout << std::endl;
        }
    }

    ~Transitions(){
    }

};

#endif /* __BIN_WORDS_TRANSITIONS_H__ */
