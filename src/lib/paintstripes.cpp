/***************************************************************************
 *   Copyright (C) 2005, 2013, 2016, 2018 by Daniel Gulotta                *
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

#ifdef FFTWPROGS
#include "stripes_common.hpp"
#include "randgen.hpp"
#include "generators.hpp"

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

symmetric_canvas<color_t> make_canvas(symgroup sym, const stripes_grid &gridr,
	const stripes_grid &gridg, const stripes_grid &gridb)
{
	int sz = gridr.get_size();
	double factor = sqrt((gridr.intensity()+gridg.intensity()+gridb.intensity())/3)/(sz*64);
	symmetric_canvas<color_t> image(sz,sym);
	canvas<color_t> & c = image.unsafe_get_canvas();
	auto normalize = [factor] (const stripes_grid &g,int i,int j) -> uint8_t {
		return colorchop(128.+g.get_symmetric(i,j).real()/factor);
	};
	for(int j=0;j<sz;j++)
		for(int i=0;i<sz;i++) {
			c(i,j).red=normalize(gridr,i,j);
			c(i,j).green=normalize(gridg,i,j);
			c(i,j).blue=normalize(gridb,i,j);
		}
	return image;

}

symmetric_canvas<color_t> paint_stripes(int sz, symgroup sym, double alpha)
{
	stripes_grid gridr(sz,sym), gridg(sz,sym), gridb(sz,sym);
	stripes_grid_norm norm(sz,sym);
	for(int i=0;i<sz;i++)
		for(int j=0;j<sz;j++)
			tie(gridr(i,j),gridg(i,j),gridb(i,j))=random_levy_6d(alpha,pow(norm(i,j),.5+1./alpha));
    gridr.transform();
    gridg.transform();
    gridb.transform();
	return make_canvas(sym,gridr,gridg,gridb);
}

symmetric_canvas<color_t> paint_clusters(int sz, symgroup sym, double alpha)
{
	stripes_grid gridr(sz,sym), gridg(sz,sym), gridb(sz,sym);
	stripes_grid_norm norm(sz,sym);
	int i,j;
	for(i=0;i<sz;i++)
		for(j=0;j<sz;j++)
			tie(gridr(i,j),gridg(i,j),gridb(i,j))=random_levy_6d(alpha,1.);
	gridr.transform();
	gridg.transform();
	gridb.transform();
	for(i=0;i<sz;i++)
		for(j=0;j<sz;j++) {
			double r = pow(norm(i,j),alpha/2.);
			gridr(i,j)*=r;
			gridg(i,j)*=r;
			gridb(i,j)*=r;
		}
	gridr.transform();
	gridg.transform();
	gridb.transform();
	return make_canvas(sym,gridr,gridg,gridb);
}

#endif
