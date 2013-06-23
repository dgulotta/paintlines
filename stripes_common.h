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
#include "painter.h"

using std::complex;
using std::fill;

class stripes_grid {
public:
	stripes_grid(painter *p) : paint(p), size(p->get_size()),
		array((complex<double> *)fftw_malloc(sizeof(complex<double>)*size*size)),
		plan(fftw_plan_dft_2d(size,size,(fftw_complex *)array,(fftw_complex *)array,FFTW_BACKWARD,FFTW_ESTIMATE)),
		phase(2*M_PI/size) {}
	~stripes_grid() {
		fftw_destroy_plan(plan);
		fftw_free(array);
	}
	complex<double> & operator () (int x, int y) { return array[mod(x,size)+size*mod(y,size)];}
	const complex<double> operator () (int x, int y) const { return array[mod(x,size)+size*mod(y,size)];}
	complex<double> get_symmetric(int x, int y) const;
	void clear() { fill(array,array+size*size,complex<double>(0,0)); }
	void generate(double (stripes_grid::*normfunc)(int,int), double alpha);
	double norm_hexagonal(int x, int y);
	double norm_orthogonal(int x, int y);
private:
	painter *paint;
	int size;
	complex<double> *array;
	fftw_plan plan;
	double phase;
};

double random_levy_1d_power_alpha(double alpha, double scale);

complex<double> random_levy_2d(double alpha, double scale);

#endif
