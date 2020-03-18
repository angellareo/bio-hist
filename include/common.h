//      common.h
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

#ifndef __COMMON__
#define __COMMON__

#define BIT_DETECTED_OUT 2
#define BIT_NOT_DETECTED_OUT 0
#define END_WINDOW_OUT 1
#define WORD_DETECTED_OUT 3

#define END_HISTOGRAM_TIME -2

#define FIRST_CALL 0
#define NEW_TIME_WINDOW 1

#define FALSE 0
#define TRUE 1

extern "C" {
	#include "wordsBuffer.h"
}

#endif
