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
#include <boost/numeric/ublas/io.hpp> 

#include "WordsObserver.h"

class Transitions : public WordsObserver
{
private:
    int nStates;
    int nTrans = 0;
    int lastWord = -1;

    boost::numeric::ublas::matrix<int> _probabilities;

public:
    typedef boost::numeric::ublas::matrix<float> TransitionMatrix;
    static const int WORD_RESET=-1;

    Transitions(std::shared_ptr<BitsObserver> subject, int numberOfStates) : WordsObserver(subject), nStates(numberOfStates)
    {
        _probabilities = boost::numeric::ublas::matrix<int>(nStates,nStates);
        
        for (unsigned i = 0; i < _probabilities.size1(); ++i){
            for (unsigned j = 0; j < _probabilities.size2(); ++j){
                _probabilities(i,j)=0;
            }
        }
    }

    void update(int word){
        //std::cout << word << std::endl;
        if (word == WORD_RESET)
            lastWord=word;
        else if (word>=0) addTransition(word);
    }

    void addTransition(int destWord){
        if (lastWord==WORD_RESET){ //Initial word of a transition
            lastWord = destWord;
        }
        else{
            _probabilities(lastWord, destWord)++;
            //std::cout << "(" << lastWord << "," << destWord << ") = " << _probabilities(lastWord, destWord) << std::endl;
            nTrans++;
            lastWord = destWord;
        }
    }

    void removeTransition(int originWord, int destWord){
        _probabilities(originWord, destWord)--;
        nTrans--;
    }

    TransitionMatrix getTransitionProbabilities(){
        TransitionMatrix _resProbabilities(nStates,nStates);

        for (unsigned i = 0; i < _probabilities.size1(); ++i){
            for (unsigned j = 0; j < _probabilities.size2(); ++j){
                _resProbabilities(i,j) = (double)_probabilities(i,j)/nTrans;
            }
        }

        return _resProbabilities;
    }

    ~Transitions(){
    }

    void printTransitionMatrix(){
        std::cout << _probabilities << std::endl;
    }

    int getNStates(){
        return nStates;
    }

};

#endif /* __BIN_WORDS_TRANSITIONS_H__ */
