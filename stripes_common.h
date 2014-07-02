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

#ifndef _STRIPES_COMMON_H
#define _STRIPES_COMMON_H

#include <fftw3.h>
#include <algorithm>
#include <cmath>
#include <complex>
#include <functional>
#include "symmetric_canvas.h"

using std::complex;

class stripes_grid {
public:
	stripes_grid(int sz, symgroup sg) : size(sz),
		group(sg),
		array((complex<double> *)fftw_malloc(sizeof(complex<double>)*size*size)),
		transformations(generate_transforms(sg,sz)),
		plan(fftw_plan_dft_2d(size,size,(fftw_complex *)array,(fftw_complex *)array,FFTW_BACKWARD,FFTW_ESTIMATE)),
		phase(2*M_PI/size) {}
	~stripes_grid() {
		fftw_destroy_plan(plan);
		fftw_free(array);
	}
	complex<double> & operator () (int x, int y) { return array[mod(x,size)+size*mod(y,size)];}
	const complex<double> operator () (int x, int y) const { return array[mod(x,size)+size*mod(y,size)];}
	complex<double> get_symmetric(int x, int y) const;
	void clear() { std::fill(array,array+size*size,complex<double>(0,0)); }
	static std::function<double(int,int)> norm_hexagonal(int sz);
	static std::function<double(int,int)> norm_orthogonal(int sz);
	std::function<double(int,int)> norm() {
		return sym_is_hexagonal[group]?norm_hexagonal(size):norm_orthogonal(size);
	}
	void transform() { fftw_execute(plan); }
	int get_size() const { return size; }
	symgroup get_group() { return group; }
	typedef double (*proj_t)(const complex<double> &);
	double intensity(const std::function<double(const complex<double> &)> &proj=(proj_t)&std::real) const;
private:
	int size;
	symgroup group;
	complex<double> *array;
	std::vector<transformation> transformations;
	fftw_plan plan;
	double phase;
};


#endif
