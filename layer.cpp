/***************************************************************************
 *   Copyright (C) 2013 by Daniel Gulotta                                  *
 *   dgulotta@alum.mit.edu                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA         *
 *   02110-1301  USA                                                       *
 ***************************************************************************/

#include "layer.h"
#include "randgen.h"

using namespace std;

void copy_pastel(uint8_t &c, uint8_t v, uint8_t t)
{
	c+=(t*((int)t-v)+255*((int)v-c))*t/(255*255);
}

void copy_regular(uint8_t &c, uint8_t v, uint8_t t)
{
	c+=((int)v-c)*t/255;
}

color_t default_color_generator()
{
	switch(random_int(6)) {
	case 0: return color_t(255,random_int(256),0);
	case 1: return color_t(0,255,random_int(256));
	case 2: return color_t(random_int(256),0,255);
	case 3: return color_t(random_int(256),255,0);
	case 4: return color_t(0,random_int(256),255);
	default: return color_t(255,0,random_int(256));
	};
}
