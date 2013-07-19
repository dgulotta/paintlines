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
 
#include "randgen.h"
#include <tuple>

#include "layer_painter.h"

using std::make_tuple;

void layer_painter::merge()
{
	fill(red.begin(),red.end(),0);
  	fill(green.begin(),green.end(),0);
  	fill(blue.begin(),blue.end(),0);
	int r, g, b, a, i;
	auto copy_pastel = [&] (unsigned char &c, int v, int t) { c+=(255*t+(255-t)*(v-c))*t/(255*255); };
	auto copy_regular = [&] (unsigned char &c, int v, int t) { c+=(v-c)*t/255; };
	for(layer &l : layers) {
		tie(r,g,b)=l.color;
		for(i=size*size-1;i>=0;i--) {
			a=l.pixels[i];
			auto copy = l.pastel?copy_pastel:copy_regular;
			copy(red[i],r,a);
			copy(green[i],g,a);
			copy(blue[i],b,a);
		}
	}
}

color_tuple layer_painter::random_color()
{
	switch(random_int(6)) {
	case 0: return make_tuple(255,random_int(256),0);
	case 1: return make_tuple(0,255,random_int(256));
	case 2: return make_tuple(random_int(256),0,255);
	case 3: return make_tuple(random_int(256),255,0);
	case 4: return make_tuple(0,random_int(256),255);
	default: return make_tuple(255,0,random_int(256));
	};
}
