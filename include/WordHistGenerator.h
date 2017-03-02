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

    int _wordLength;
    int _maxWords;
    int _numWords;
    float _entropy=-1;
    float _bias=-1;
    
    std::vector<int> _wordsBuf;
    std::vector<int> _hist;
    
    void calculateEntropy(){
        _entropy = 0;
        for (int nOccurWord : _hist){
			if (nOccurWord!=0){
                float prob = float(nOccurWord)/_numWords;
                _entropy -= prob * (log(prob)/log(2));
            }
        }
    }

    void calculateBias(){
        _bias = (-_maxWords+1)/(2*_numWords*0.69314718055994530941723212145818 /*ln 2*/ );
    }

public:

    WordHistGenerator(int wordLength, std::shared_ptr<BitsObserver> mod) : WordsObserver (mod){
        _maxWords=(int)pow(2,wordLength);
        _wordLength=wordLength;      
    }
    
    virtual ~WordHistGenerator(){
        
    }
    
    void update(int word){
        if (word==BinSignalGenerator::SIGNAL_END){
            createHist();
            //calculateEntropy();
            //calculateBias();
            return;
        }
        _wordsBuf.push_back(word);
        _numWords++;
    }
    
    void createHist(){
        std::vector<int> v(_maxWords,0);
        _hist = std::move(v);
        for(auto word : _wordsBuf){
            _hist[word]++;
        }
    }
    
    std::vector<int> getHist(){
        return _hist;
    }
    
    int getWordLength(){
        return _wordLength;
    }
    
    int getMaxWords(){
        return _maxWords;
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
