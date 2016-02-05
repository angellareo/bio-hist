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
    float _entropy;
    
    WordsBuffer _wordsbuf;
    
    std::unique_ptr<int[]> _hist;
    std::vector< std::pair<int,int> > _wordsPosVector;
    
public:
    WordHistGenerator(int wordLength, std::shared_ptr<SpikesObserver> mod) : BitsObserver (mod){
        _binTime = mod->getBinTime();
        _maxWords=(int)pow(2,wordLength); 
        
        _hist = std::unique_ptr<int[]>(new int[_maxWords]());
        for (int i=0; i<_maxWords; i++){
            _hist[i] = 0;
        }
        _wordLength=wordLength;
        wbInit(&_wordsbuf, wordLength, MAX_WORDS_BUFF);        
    }
    
    virtual ~WordHistGenerator(){
        
    }
    
    void update(int bit){
        if (bit==BIT_END){
            createHist();
            calculateEntropy();
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
        wbCreateHistogram(&_wordsbuf, _hist.get());
        std::vector<int> wordsVec(_hist.get(), _hist.get()+_maxWords);
        
        for (std::vector<int>::iterator it = wordsVec.begin(); (it != wordsVec.end()); ++it){
            _wordsPosVector.push_back(std::pair<int,int>(*it,it-wordsVec.begin()));
        }
        
        std::sort(_wordsPosVector.begin(), _wordsPosVector.end(), std::greater< std::pair<int,int> > ());
    }
    
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
        return _entropy;
    }
};

#endif /* __WORD_HIST_GENERATOR_H__ */
