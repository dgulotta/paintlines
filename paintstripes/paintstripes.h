/***************************************************************************
 *   Copyright (C) 2005, 2013 by Daniel Gulotta                            *
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

#ifndef _PAINTSTRIPES_H
#define _PAINTSTRIPES_H

#include <functional>
#include "../color.h"
#include "../symmetric_canvas.h"

class stripes_grid;

class paintstripes
{
public:
	paintstripes() : levy_alpha(1.0) {}
	void paint(int sz, symgroup sym);
	void set_alpha(double alpha) {levy_alpha=alpha;}
	const canvas<color_t> & get_image() { return image.as_canvas(); }
	const symmetric_canvas<color_t> & get_symmetric_image() { return image; }
private:
	void fill(const std::function<void(int,int,uint8_t)> &set, const stripes_grid &g, double intensity);
	symmetric_canvas<color_t> image;
	double levy_alpha;
};

#endif
