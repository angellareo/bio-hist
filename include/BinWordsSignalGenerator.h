//      BinWordsSignalGenerator.h
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

#ifndef __BIN_WORDS_SIGNAL_GENERATOR_H__
#define __BIN_WORDS_SIGNAL_GENERATOR_H__

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

class BinWordsSignalGenerator : public BitsObserver
{
private:
    const int BIT_ERROR=-1;
    const int BIT_END=-2;

    int _numBits;
    int _wordLength;
    int _binTime;
    int _maxWords;
    float _entropy;
    
    WordsBuffer _wordsbuf;
    
    std::vector<int> binWordsSignal;

public:
    BinWordsSignalGenerator(int wordLength, std::shared_ptr<SpikesObserver> mod) : BitsObserver (mod){
        _binTime = mod->getBinTime();
        _maxWords=(int)pow(2,wordLength); 
        _numBits = 0;

        _wordLength=wordLength;
        wbInit(&_wordsbuf, wordLength, MAX_WORDS_BUFF);        
    }
    
    virtual ~BinWordsSignalGenerator(){
        
    }
    
    void update(int bit){
        if (bit==BIT_END){
            return;
        }
        
        if (bit==BIT_ERROR){
            _numBits=0;
            wbRestartBitBuff(&_wordsbuf);
            return;
        }
        
        wbBitInsert(&_wordsbuf,bit);
        _numBits ++;
        if (_numBits == _wordLength){
            wbStoreWord(&_wordsbuf);
            binWordsSignal.push_back(wbBits2Int(&_wordsbuf));
            _numBits = 0;
        }
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
    
    std::vector<int> getBinWordsSignal(){
        return binWordsSignal;
    }
};

#endif /* __BIN_WORDS_SIGNAL_GENERATOR_H__ */
