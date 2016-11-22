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
extern "C" {
	#include "wordsBuffer.h"
}

class WordHistGenerator : public BitsObserver
{
private:
    const int BIT_ERROR=-1;
    const int BIT_END=-2;

    int _wordLength;
    int _binTime;
    int _maxWords;
    float _entropy=-1;
    float _bias=-1;
    
    WordsBuffer _wordsbuf;
    
    std::vector< std::pair<int,int> > _wordsPosVector;
    
    void calculateEntropy(){
        _entropy = 0;
        for (std::vector< std::pair<int,int> >::iterator it = _wordsPosVector.begin(); (it != _wordsPosVector.end()); ++it){
			float prob = static_cast< float >(it->first)/_wordsbuf.numWords;
            if ((prob > 0.0000001)||(prob < -0.0000001)){
                _entropy -= prob * (log(prob)/log(2));
                //cout << "----word#: " << *it << " Entropy: " << entropy << " Prob: " << prob << " log2(p): "<< (log(prob)/log(2)) << endl;
            }
        }
    }

    void calculateBias(){
        _bias = (-_wordsbuf.maxWords+1)/(2*_wordsbuf.numWords*0.69314718055994530941723212145818 /*ln 2*/ );
    }

public:

    WordHistGenerator(int wordLength, std::shared_ptr<SpikesObserver> mod) : BitsObserver (mod){
        _binTime = mod->getBinTime();
        _maxWords=(int)pow(2,wordLength);

        _wordLength=wordLength;
        wbInit(&_wordsbuf, wordLength, _maxWords);        
    }
    
    virtual ~WordHistGenerator(){
        
    }
    
    void update(int bit){
        if (bit==BIT_END){
            createHist();
            //calculateEntropy();
            //calculateBias();
            return;
        }
        
        if (bit==BIT_ERROR){
            //wbRestartBitBuff(&_wordsbuf);
            return;
        }
        
        wbBitInsert(&_wordsbuf,bit);
        wbStoreWord(&_wordsbuf);
    }
    
    void createHist(){  
        int hist[_maxWords];
        wbCreateHistogram(&_wordsbuf, hist);
        std::vector<int> wordsVec(hist, hist+_maxWords);
        
        int pos = 0; 
        for (std::vector<int>::iterator it = wordsVec.begin(); (it != wordsVec.end()); ++it){
            _wordsPosVector.push_back(std::pair<int,int>(*it, pos++));
        }
        
        std::sort(_wordsPosVector.begin(), _wordsPosVector.end(), std::greater< std::pair<int,int> > ());
    }
    
    std::vector< std::pair<int,int> > getHist(){
        return _wordsPosVector;
    }
    
    int getWordLength(){
        return _wordLength;
    }
    
    int getBinTime(){
        return _binTime;
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
