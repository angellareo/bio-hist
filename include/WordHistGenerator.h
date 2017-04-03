//      WordHistGenerator.h
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

#ifndef __WORD_HIST_GENERATOR_H__
#define __WORD_HIST_GENERATOR_H__

#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>

#include <SpikesObserver.h>
#include <SignalProcessor.h>
#include <BitsObserver.h>
#include <BinSignalGenerator.h>
extern "C" {
	#include "wordsBuffer.h"
}

class WordHistGenerator : public WordsObserver
{
private:
    //const int BIT_ERROR=-1;
    //const int BIT_END=-2;

    int _numWords;
    float _entropy=-1;
    float _bias=-1;
    bool _histCreated=0;
    std::vector<int> _wordsBuf;    
    std::vector< std::pair<int,int> > _wordsPosVector;

    void calculateEntropy(){
        _entropy = 0;
        for (auto& nOccurWord : _wordsPosVector){
			if (nOccurWord.first!=0){
                float prob = float(nOccurWord.first)/_numWords;
                _entropy -= prob * (log(prob)/log(2));
            }
        }
    }

    void calculateBias(){
        _bias = (-getSubject()->getMaxWords()+1)/(2*_numWords*0.69314718055994530941723212145818 /*ln 2*/ );
    }

public:

    WordHistGenerator(std::shared_ptr<BitsObserver> mod) : WordsObserver (mod){
          
    }
    
    virtual ~WordHistGenerator(){
        
    }
    
    void update(int word){
        if (word==BinSignalGenerator::SIGNAL_END){
            //calculateEntropy();
            //calculateBias();
            return;
        }
        _wordsBuf.push_back(word);
        _numWords++;
    }
    
    void createHist(){             
        std::vector<int> wordsVec(getSubject()->getMaxWords(),0);
        for(auto word : _wordsBuf){
            wordsVec[word]++;
        }

        int pos = 0; 
        for (std::vector<int>::iterator it = wordsVec.begin(); (it != wordsVec.end()); ++it){
            _wordsPosVector.push_back(std::pair<int,int>(*it, pos++));
        }
        std::sort(_wordsPosVector.begin(), _wordsPosVector.end(), std::greater< std::pair<int,int> > ());
    }
    
    std::vector< std::pair<int,int> > getHist(){
        if (!_histCreated){
            createHist();
            _histCreated = 1;
        }
        return _wordsPosVector;
    }
    
    float getEntropy(){
        if (_entropy==-1){
            calculateEntropy();
        }
        return _entropy;
    }

    float getBias(){
        if (_bias==-1){
            calculateBias();
        }
        return _bias;
    }
    
    float getCorrectedEntropy(){
        return (_entropy - _bias);
    }
};

#endif /* __WORD_HIST_GENERATOR_H__ */
