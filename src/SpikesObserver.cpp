//      SpikesObserver.cpp
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

#include "SpikesObserver.h"
#include "SignalProcessor.h"
#include "BitsObserver.h"

using namespace std;

SpikesObserver::SpikesObserver(SignalProcessor& mod, double binTime): _model(mod){
    _binTime = binTime;
    //_model.attach(this);
}

SignalProcessor& SpikesObserver::getSubject() {
    return _model;
}

void SpikesObserver::notify(int bit) {
  for (auto v : _views)
    v->update(bit);
}

void SpikesObserver::attach(shared_ptr<BitsObserver> obs) {
    _views.push_back(obs);
}
