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

#include <algorithm>
#include <future>
#include "../randgen.h"
#include "../stripes_common.h"
#include "paintsquiggles.h"

using std::fill;
using std::function;

void paintsquiggles::paint(int sz, symgroup sym)
{
	function<double(int,int)> norm;
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
	layers.resize(n_colors);
	grids.clear();
	grids.resize(n_colors,symmetric_canvas<uint8_t>(sz,sym,0));
	for(int i=0;i<n_colors;i++) {
		layer &l = layers[i];
		l.pixels=&(grids[i].as_canvas());
		l.color = generate_color();
		l.pastel = false;
	}
	std::vector<std::future<stripes_grid *>> futures((layers.size()+1)/2);
	std::random_device rd;
	for(int i=0;i<futures.size();i++) {
		// fftw_malloc is not thread safe, so allocate the grid beforehand
		stripes_grid *g = new stripes_grid(sz,sym);
		auto seed = rd();
		futures[i] = std::async(std::launch::async,[&,i,g,seed]() {
			std::default_random_engine rng(seed);
			generate((*g),norm,rng);
			fill(*g,const_cast<canvas<uint8_t> &>(*layers[2*i].pixels),(stripes_grid::proj_t)&std::real);
			if(2*i+1<layers.size())
				fill(*g,const_cast<canvas<uint8_t> &>(*layers[2*i+1].pixels),(stripes_grid::proj_t)&std::imag);
			return g;
		});
	}
	for(auto &f : futures)
		delete f.get();
	image=symmetric_canvas<color_t>(sz,sym,black);
	merge(const_cast<canvas<color_t> &>(image.as_canvas()),layers);
}

void paintsquiggles::fill(const stripes_grid &grid,canvas<uint8_t> &pix,const function<double(const complex<double> &)> &proj)
{
	int size = pix.height();
	double norm=grid.intensity(proj);
	norm=sqrt(norm)/(size*64);
	double height, alpha;
	int i, j;
	for(j=0;j<size;j++)
		for(i=0;i<size;i++) {
			height=proj(grid.get_symmetric(i,j))/norm;
			pix(i,j)=255.99/(pow(abs(height)/(10.*thickness),sharpness)+1.);
		}
}

void paintsquiggles::generate(stripes_grid &grid, function<double(int,int)> &f, std::default_random_engine &rng) {
	grid.clear();
	int i,j, size=grid.get_size();
	for(j=0;j<size;j++)
		for(i=0;i<size;i++)
			grid(i,j) = complex<double>(random_levy_1d(levy_alpha,1.,rng),random_levy_1d(levy_alpha,1.,rng));
	grid.transform();
	for(j=0;j<size;j++)
		for(i=0;i<size;i++)
			grid(i,j) *= pow(f(i,j),exponent/2.);
	grid.transform();
}
