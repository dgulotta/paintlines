/***************************************************************************
 *   Copyright (C) 2005-2007, 2013 by Daniel Gulotta                       *
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

#ifndef _PAINTCLOUDS_H
#define _PAINTCLOUDS_H

#include <functional>
#include "../color.h"
#include "../symmetric_canvas.h"

class paintclouds
{
public:
	paintclouds() : randfunc(&paintclouds::rand_exp_cos) {}
	void paint(int size, symgroup sg);
	void set_colors(uint8_t r1, uint8_t g1, uint8_t b1,
			uint8_t r2, uint8_t g2, uint8_t b2,
			uint8_t r3, uint8_t g3, uint8_t b3)
	{red1=r1;red2=r2;red3=r3;blue1=b1;blue2=b2;blue3=b3;green1=g1;green2=g2;
		green3=g3;}
	void set_color1(uint8_t r, uint8_t g, uint8_t b)
	{red1=r;green1=g;blue1=b;}
	void set_color2(uint8_t r, uint8_t g, uint8_t b)
	{red2=r;green2=g;blue2=b;}
	void set_color3(uint8_t r, uint8_t g, uint8_t b)
	{red3=r;green3=g;blue3=b;}
	void set_randfunc(const std::function<double(double)> &r) { randfunc=r; }
	static double rand_exp_cos(double a);
	static double rand_normal(double a);
	static double rand_cauchy(double a);
	static double rand_sechsquare(double a);
	const canvas<color_t> & get_image() { return grid.as_canvas(); }
	const symmetric_canvas<color_t> & get_symmetric_image() { return grid; }
private:
	symmetric_canvas<color_t> grid;
	std::function<double(double)> randfunc;
	uint8_t random_byte(short b1, short b2, int d);
	color_t random_color(color_t c1, color_t c2, int d);
	void fill_line(canvas<color_t> &tri, int i1, int j1, int i2, int j2);
	void fill_tri(canvas<color_t> &tri);
	void fill_tri_ab2(canvas<color_t> &tri);
	void fill_tri_sabc(canvas<color_t> &tri);
	void fill_tri_asb(canvas<color_t> &tri);
	uint8_t red1, red2, red3, blue1, blue2, blue3, green1, green2, green3;
};

#endif
