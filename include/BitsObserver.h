//      BitsObserver.h
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

#ifndef __BITS_OBSERVER_H__
#define __BITS_OBSERVER_H__

#include <iostream>
#include <memory>
#include <cmath>
#include <SpikesObserver.h>

class BitsObserver
{
private:
    std::shared_ptr<SpikesObserver> _model;
public:
    BitsObserver(std::shared_ptr<SpikesObserver> mod) {
        _model = mod;
        _model->attach(this);
    }
    virtual ~BitsObserver(){}
    virtual void update(int bit) = 0;
protected:
    SpikesObserver* getSubject();
};

#endif /* __BITS_OBSERVER_H__ */
