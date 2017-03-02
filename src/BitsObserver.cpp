//      BitsObserver.cpp
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

#include "BitsObserver.h"
#include "SpikesObserver.h"
#include "WordsObserver.h"

using namespace std;

BitsObserver::BitsObserver(int wordLength, shared_ptr<SpikesObserver> mod) {
    _model = mod;
    _model->attach(this);

    _binTime = mod->getBinTime();
    _wordLength= wordLength;
    _maxWords=(int)pow(2,_wordLength); 
}

shared_ptr<SpikesObserver> BitsObserver::getSubject() {
    return _model;
}

void BitsObserver::notify(int bit) {
  for (auto v : _views)
    v->update(bit);
}

void BitsObserver::attach(WordsObserver *obs) {
    _views.push_back(obs);
}
