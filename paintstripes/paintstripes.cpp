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

#include "../stripes_common.h"
#include "../randgen.h"
#include "paintstripes.h"

#include <complex>
#include <tuple>

using std::tuple;
using std::make_tuple;

typedef std::complex<double> cpx;

tuple<cpx,cpx,cpx> random_levy_6d(double alpha, double scale) {
	cpx z1(random_normal(1),random_normal(1));
	cpx z2(random_normal(1),random_normal(1));
	cpx z3(random_normal(1),random_normal(1));
	double r=scale*random_levy_skew_sqrt(alpha/2);
	return make_tuple(z1*r,z2*r,z3*r);
}

void paintstripes::paint(int sz, symgroup sym)
{
	stripes_grid gridr(sz,sym), gridg(sz,sym), gridb(sz,sym);
	std::function<double(int,int)> norm;
	switch(sym) {
	case SYM_P3:
	case SYM_P31M:
	case SYM_P3M1:
	case SYM_P6:
	case SYM_P6M:
    	norm=stripes_grid::norm_hexagonal(sz);
		break;
	default:
		norm=stripes_grid::norm_orthogonal(sz);
  	}
	int i,j;
	for(i=0;i<sz;i++)
		for(j=0;j<sz;j++)
			tie(gridr(i,j),gridg(i,j),gridb(i,j))=random_levy_6d(levy_alpha,pow(norm(i,j),.5+1./levy_alpha));
	gridr.transform();
	gridg.transform();
	gridb.transform();
	double s = gridr.intensity()+gridg.intensity()+gridb.intensity();
	image = symmetric_canvas<color_t>(sz,sym);
	canvas<color_t> & c = const_cast<canvas<color_t> &>(image.as_canvas());
	fill([&](int x,int y,uint8_t v) { c(x,y).red=v; },gridr,s);
	fill([&](int x,int y,uint8_t v) { c(x,y).green=v; },gridg,s);
	fill([&](int x,int y,uint8_t v) { c(x,y).blue=v; },gridb,s);
}

void paintstripes::fill(const std::function<void(int,int,uint8_t)> &set, const stripes_grid &g, double intensity)
{
	int i,j, size=g.get_size();
	double norm=sqrt(intensity/3)/(size*64);
	for(j=0;j<size;j++)
		for(i=0;i<size;i++)
			set(i,j,colorchop(128.+g.get_symmetric(i,j).real()/norm));
}
