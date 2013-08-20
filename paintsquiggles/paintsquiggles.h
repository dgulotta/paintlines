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

#ifndef _PAINTSQUIGGLES_H
#define _PAINTSQUIGGLES_H

#include <fftw3.h>
#include <complex>
#include <random>
#include <vector>
#include "../color.h"
#include "../layer.h"
#include "../symmetric_canvas.h"

class stripes_grid;

class paintsquiggles
{
public:
	paintsquiggles() : generate_color(&default_color_generator), levy_alpha(1.0), exponent(2.), thickness(1.), sharpness(2.) {}
	void paint(int sz, symgroup sym);
	void set_alpha(double alpha) {levy_alpha=alpha;}
	void set_exponent(double e) {exponent = e;}
	void set_ncolors(int n) {n_colors=n;}
	void set_thickness(double t) {thickness=t;}
	void set_sharpness(double s) {sharpness=s;}
	const std::vector<layer> & get_layers() { return layers; }
	const canvas<color_t> & get_image() { return image.as_canvas(); }
	const symmetric_canvas<color_t> & get_symmetric_image() { return image; }
	void set_color_generator(const std::function<color_t()> &f) { generate_color = f; }
private:
	void fill(const stripes_grid &grid, canvas<uint8_t> &pix, const std::function<double(const std::complex<double> &)> &proj);
	void generate(stripes_grid &grid, std::function<double(int,int)> &f, std::default_random_engine &rng);
	std::vector<layer> layers;
	std::vector<symmetric_canvas<uint8_t>> grids;
	symmetric_canvas<color_t> image;
	std::function<color_t()> generate_color;
	int n_colors;
	double levy_alpha;
	double exponent;
	double thickness;
	double sharpness;
};

#endif
