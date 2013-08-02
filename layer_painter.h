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

#ifndef _LAYER_PAINTER_H

#define _LAYER_PAINTER_H

#include "painter.h"

struct layer {
	vector<unsigned char> pixels;
	color_tuple color;
	bool pastel;
};

class layer_painter : virtual public painter {
public:
	layer_painter() { clear_color_generator(); }
	void set_color_generator(const function<color_tuple(void)> &f) { generate_color = f; }
	void clear_color_generator() { generate_color = &layer_painter::random_color;}
	const vector<layer> & get_layers() { return layers; }
protected:
	static color_tuple random_color();
	function<color_tuple(void)> generate_color;
	void merge();
	vector<layer> layers;
};

#endif
