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
	stripes_grid_norm norm(sz,sym);
	int i,j;
	for(i=0;i<sz;i++)
		for(j=0;j<sz;j++)
			tie(gridr(i,j),gridg(i,j),gridb(i,j))=random_levy_6d(levy_alpha,pow(norm(i,j),.5+1./levy_alpha));
	gridr.transform();
	gridg.transform();
	gridb.transform();
	double factor = sqrt((gridr.intensity()+gridg.intensity()+gridb.intensity())/3)/(sz*64);
	image = symmetric_canvas<color_t>(sz,sym);
	canvas<color_t> & c = image.unsafe_get_canvas();
	auto normalize = [factor] (const stripes_grid &g,int i,int j) -> uint8_t {
		return colorchop(128.+g.get_symmetric(i,j).real()/factor);
	};
	for(j=0;j<sz;j++)
		for(i=0;i<sz;i++) {
			c(i,j).red=normalize(gridr,i,j);
			c(i,j).green=normalize(gridg,i,j);
			c(i,j).blue=normalize(gridb,i,j);
		}
}
