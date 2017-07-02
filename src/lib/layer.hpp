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

#ifndef _LAYER_H
#define _LAYER_H

#include "canvas.hpp"
#include "color.hpp"

struct layer {
	layer() : pixels(nullptr), color(black), pastel(false) {}
	const canvas<uint8_t> *pixels;
	color_t color;
	bool pastel;
};

void copy_pastel(uint8_t &c, uint8_t v, uint8_t t);
void copy_regular(uint8_t &c, uint8_t v, uint8_t t);

template <typename Canvas, typename Container>
void merge(Canvas &img, const Container &layers)
{
	for(const layer &l : layers) {
		color_t col=l.color;
		auto &src = *(l.pixels);
		auto copy = l.pastel?&copy_pastel:&copy_regular;
		for(int j=0;j<img.height();j++)
			for(int i=0;i<img.width();i++) {
				uint8_t a = src(i,j);
				auto &pix = img(i,j);
				(*copy)(pix.red,col.red,a);
				(*copy)(pix.green,col.green,a);
				(*copy)(pix.blue,col.blue,a);
			}
	}
}

color_t default_color_generator();

#endif
