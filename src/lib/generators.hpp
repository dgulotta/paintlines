/***************************************************************************
 *   Copyright (C) 2005, 2013-2017 by Daniel Gulotta                       *
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

#ifndef _GENERATORS_HPP
#define _GENERATORS_HPP

#include "canvas.hpp"
#include "color.hpp"
#include "hyperbolic_group.hpp"
#include "symmetric_canvas.hpp"

struct hyperbolic_lines_param
{
	int size;
	float radius;
	float brightness;
	float sharpness;
	projtype proj;
};

canvas<uint8_t> paint_hyperbolic_lines(const hyperbolic_symmetry_group &sg,const hyperbolic_lines_param &params);

symmetric_canvas<color_t> paint_stripes(int sz, symgroup sym, double alpha);

symmetric_canvas<color_t> drawtrap(int sz, symgroup sg);
canvas<color_t> drawquasitrap(int width, int height,int symmetry,double quasiperiod);
canvas<color_t> drawquasitrap_poly(int width, int height ,int symmetry,double quasiperiod);

enum class walk_fill { AREA, LENGTH };

wrap_canvas<color_t> draw_walk(int width, int height, bool balanced, walk_fill wf);
canvas<color_t> draw_walk2(int width, int height, bool balanced, walk_fill wf);

#ifdef FFTWPROGS
#include <vector>

canvas<color_t> paint_quasiperiodic_stripes(int size, int fftsize, double alpha);

std::vector<symmetric_canvas<uint8_t>> paint_squiggles(size_t ncolors,
	size_t size, symgroup sg, double alpha, double exponent, double thickness,
	double sharpness);

#endif
#endif
