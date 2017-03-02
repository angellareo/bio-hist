//      WordsObserver.h
//      
//      Copyright 2017 Ángel Lareo <angel.lareo@gmail.com>
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

#ifndef __WORDS_OBSERVER_H__
#define __WORDS_OBSERVER_H__

#include <iostream>
#include <memory>
#include <cmath>
#include <BitsObserver.h>

class WordsObserver
{
private:
    std::shared_ptr<BitsObserver> _model;
public:
    WordsObserver(std::shared_ptr<BitsObserver> mod) {
        _model = mod;
        _model->attach(this);
    }
    virtual ~WordsObserver(){}
    virtual void update(int bit) = 0;
    std::shared_ptr<BitsObserver> getSubject(){
    	return _model;
    }
};

#endif /* __WORDS_OBSERVER_H__ */
