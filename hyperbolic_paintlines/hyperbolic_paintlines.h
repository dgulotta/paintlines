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

#ifndef _HYPERBOLIC_PAINTLINES_H
#define _HYPERBOLIC_PAINTLINES_H

#include "../canvas.h"
#include "../color.h"
#include "../hyperbolic_painter.h"
#include "../layer.h"

class hyperbolic_paintlines
{
public:
	hyperbolic_paintlines() : radius(4.99), brightness(5.), sharpness(1.5), ncolors(0) {}
	void paint(int sz, hyperbolic_symmetry_group &sym);
	void set_ncolors(int n) {ncolors=n;}
	void set_thickness(double r, double b, double s) { radius=r; brightness=b; sharpness=s; }
	const std::vector<layer> & get_layers() { return layers; }
	const canvas<color_t> & get_image() { return image; }
	void set_projtype(projtype p) { pt=p; }
private:
	int size() { return image.height(); }
	screen_coord toscreen(const planar_coord &pc) { return coord_converter(size()).toscreen(pc); }
	planar_coord fromscreen(const screen_coord &sc) { return coord_converter(size()).fromscreen(sc); }
	std::function<void(const hyperbolic_coord &)> drawdot_symmetric;
	void drawdot(const hyperbolic_coord & hc);
	void fill_poincare();
	void fill_klein();
	void fill_color(const canvas<float> &mask);
	planar_coord (*proj)(const hyperbolic_coord &);
	void drawsmoothline(const hyperbolic_coord &end1, 
			const hyperbolic_coord &end2, double var, double min);
	canvas<color_t> image;
	std::vector<canvas<uint8_t>> grids;
	std::vector<layer> layers;
	canvas<uint8_t> *active_grid;
	projtype pt;
	double radius;
	double brightness;
	double sharpness;
	int ncolors;
	int t;
};

#endif
