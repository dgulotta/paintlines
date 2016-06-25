/***************************************************************************
 *   Copyright (C) 2013, 2016 by Daniel Gulotta                            *
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

#include <algorithm>
#include <cmath>
#include <complex>
#include <memory>
#include <functional>
#include "symmetric_canvas.h"
#include "fftw_cxx.h"

using std::complex;

class stripes_grid {
public:
	stripes_grid(int sz, symgroup sg) :
		size(sz),
		group(sg),
		array(allocate_complex(sz*sz)),
		plan(fftw_plan_dft_2d(size,size,farray(),farray(),FFTW_BACKWARD,FFTW_ESTIMATE)),
		transformations(transformation<int>::group(sg,sz)) {}
	complex<double> & operator () (int x, int y) { return array[mod(x,size)+size*mod(y,size)];}
	const complex<double> operator () (int x, int y) const { return array[mod(x,size)+size*mod(y,size)];}
	complex<double> get_symmetric(int x, int y) const;
	void clear() { std::fill(array.get(),array.get()+size*size,complex<double>(0,0)); }
	void transform() { fftw_execute(plan.get()); }
	int get_size() const { return size; }
	symgroup get_group() { return group; }
	typedef double (*proj_t)(const complex<double> &);
	double intensity(const std::function<double(const complex<double> &)> &proj=(proj_t)&std::real) const;
private:
	fftw_complex * farray() { return reinterpret_cast<fftw_complex *>(array.get()); }
	int size;
	symgroup group;
	std::unique_ptr<complex<double>[],fftw_free_deleter<complex<double>>> array;
	std::unique_ptr<std::remove_pointer_t<fftw_plan>,fftw_plan_deleter> plan;
	std::vector<transformation<int>> transformations;
};

class stripes_grid_norm {
public:
	stripes_grid_norm(int sz, symgroup sg) : phase(2*M_PI/sz), hexagonal(sym_is_hexagonal(sg)) {}
	double operator () (int x, int y) const;
private:
	double phase;
	bool hexagonal;
};

#endif
