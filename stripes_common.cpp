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
 
#include "randgen.h"
#include "stripes_common.h"
#include <random>

using std::function;
using std::polar;

complex<double> stripes_grid::get_symmetric(int x, int y) const {
	complex<double> sum(0,0);
	point<int> p(x,y);
	int xn, yn;
	for(auto &t : transformations) {
		std::tie(xn,yn)=t(p);
		sum+=(*this)(xn,yn);
	}
	return sum;
}

function<double(int,int)> stripes_grid::norm_hexagonal(int sz) {
	double p=2.*M_PI/sz;
	return [p] (int x, int y) {
		if(x==0&&y==0) x=1;
		return 1./(3.-cos(p*x)-cos(p*y)-cos(p*(x-y)));
	};
}

function<double(int,int)> stripes_grid::norm_orthogonal(int sz) {
	double p=2.*M_PI/sz;
	return [p] (int x, int y) {
		if(x==0&&y==0) x=1;
		return 1./(2.-cos(p*x)-cos(p*y));
	};
}

double stripes_grid::intensity(const function<double(const complex<double> &)> &proj) const {
	double s=0;
	int i, j;
	for(j=0;j<size;j++)
		for(i=0;i<size;i++) {
			double d=proj(get_symmetric(i,j));
			s+=d*d;
		}
	return s;
}
