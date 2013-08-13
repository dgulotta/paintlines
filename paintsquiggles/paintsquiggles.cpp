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

void paintsquiggles::paint(int sz, symgroup sym)
{
	painter::paint(sz,sym);
	stripes_grid grid(this);
	function<double(int,int)> norm;
	switch(sym) {
	case SYM_P3:
	case SYM_P31M:
	case SYM_P3M1:
	case SYM_P6:
	case SYM_P6M:
    	norm=grid.norm_hexagonal();
		break;
	default:
		norm=grid.norm_orthogonal();
  	}
	std::fill(red.begin(),red.end(),0);
	std::fill(green.begin(),green.end(),0);
	std::fill(blue.begin(),blue.end(),0);
	layers.resize(n_colors);
	for(layer &l : layers) {
		l.pixels.resize(size*size);
		l.color = generate_color();
		l.pastel = false;
	}
	vector<std::future<stripes_grid *>> futures((layers.size()+1)/2);
	std::random_device rd;
	for(int i=0;i<futures.size();i++) {
		// fftw_malloc is not thread safe, so allocate the grid beforehand
		stripes_grid *g = new stripes_grid(this);
		auto seed = rd();
		futures[i] = std::async(std::launch::async,[&,i,g,seed]() {
			std::default_random_engine rng(seed);
			generate((*g),norm,rng);
			fill(*g,layers[2*i].pixels,(stripes_grid::proj_t)&std::real);
			if(2*i+1<layers.size())
				fill(*g,layers[2*i+1].pixels,(stripes_grid::proj_t)&std::imag);
			return g;
		});
	}
	for(auto &f : futures)
		delete f.get();
	merge();
}

void paintsquiggles::fill(const stripes_grid &grid,vector<unsigned char> &pix,const function<double(const complex<double> &)> &proj)
{
	double norm=grid.intensity(proj);
	norm=sqrt(norm)/(size*64);
	double height, alpha;
	int i, j;
	for(i=0;i<size;i++)
		for(j=0;j<size;j++) {
			height=proj(grid.get_symmetric(i,j))/norm;
			pix[i+size*j]=255.99/(pow(abs(height)/(10.*thickness),sharpness)+1.);
		}
}

void paintsquiggles::generate(stripes_grid &grid, function<double(int,int)> &f, std::default_random_engine &rng) {
	grid.clear();
	int i,j;
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			grid(i,j) = complex<double>(random_levy_1d(levy_alpha,1.,rng),random_levy_1d(levy_alpha,1.,rng));
	grid.transform();
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			grid(i,j) *= pow(f(i,j),exponent/2.);
	grid.transform();
}
