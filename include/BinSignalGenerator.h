//      BinSignalGenerator.h
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

#ifndef __BIN_SIGNAL_GENERATOR_H__
#define __BIN_SIGNAL_GENERATOR_H__

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <memory>

#include <SpikesObserver.h>
#include <SignalProcessor.h>
#include <BitsObserver.h>
#include <Transitions.h>
extern "C" {
	#include "wordsBuffer.h"
}

class BinSignalGenerator : public BitsObserver
{
private:
    int _numBits;
    int _wordLength;
    int _binTime;
    int _maxWords;
    float _entropy;

    WordsBuffer _wordsbuf;
    
    std::vector<int> binSignal;

public:
    static const int BIT_ERROR=-1;
    static const int BIT_END=-2;
    static const int WORD_RESET=-1;
    static const int SIGNAL_END=-2;

    BinSignalGenerator(int wordLength, std::shared_ptr<SpikesObserver> mod) : BitsObserver (mod) {
        _binTime = mod->getBinTime();
        _maxWords=(int)pow(2,wordLength); 
        _numBits = 0;

        _wordLength=wordLength;
        wbInit(&_wordsbuf, wordLength, MAX_WORDS_BUFF);        
    }
    
    virtual ~BinSignalGenerator(){
        
    }
    
    void update(int bit){
        if (bit==BIT_END){
            notify(SIGNAL_END);
            return;
        }
        
        if (bit==BIT_ERROR){
            _numBits=0;
            wbRestartBitBuff(&_wordsbuf);
            notify(WORD_RESET);
            return;
        }

        binSignal.push_back(bit);     
        wbBitInsert(&_wordsbuf,bit);
        _numBits ++;
        if (_numBits >= _wordLength){
            wbStoreWord(&_wordsbuf);
            notify(_wordsbuf.words[_wordsbuf.numWords-1]);
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
    
    std::vector<int> getBinSignal(){
        return binSignal;
    }
};

#endif /* __BIN_SIGNAL_GENERATOR_H__ */
