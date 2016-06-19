/***************************************************************************
 *   Copyright (C) 2005, 2013-2014 by Daniel Gulotta                       *
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
	layers.resize(n_colors);
	grids.clear();
	grids.resize(n_colors,symmetric_canvas<uint8_t>(sz,sym,0));
	for(int i=0;i<n_colors;i++) {
		layer &l = layers[i];
		l.pixels=&(grids[i].as_canvas());
		l.color = generate_color();
		l.pastel = false;
	}
	std::vector<std::future<void>> futures((layers.size()+1)/2);
	std::random_device rd;
	for(int i=0;i<futures.size();i++) {
		// fftw_malloc is not thread safe, so allocate the grid beforehand
		auto g = std::make_unique<stripes_grid>(sz,sym);
		auto seed = rd();
		futures[i] = std::async(std::launch::async,[this,i,g=std::move(g),seed]() {
			std::default_random_engine rng(seed);
			if(2*i+1<layers.size())
				fill_two(grids[2*i],grids[2*i+1],rng,levy_alpha,exponent,thickness,sharpness,*g);
			else
				fill_one(grids[2*i],rng,levy_alpha,exponent,thickness,sharpness,*g);
		});
	}
	for(auto &f : futures)
		f.wait();
	image=symmetric_canvas<color_t>(sz,sym,black);
	merge(image.unsafe_get_canvas(),layers);
}

typedef std::function<complex<double>(const std::function<double()> &)> dtocx;

void fill(const stripes_grid &grid,canvas<uint8_t> &pix,const function<double(const complex<double> &)> &proj,double thickness,double sharpness)
{
	int size = pix.height();
	double norm=grid.intensity(proj);
	norm=sqrt(norm)/(grid.get_size()*64);
	double height, alpha;
	int i, j;
	for(j=0;j<grid.get_size();j++)
		for(i=0;i<grid.get_size();i++) {
			height=proj(grid.get_symmetric(i,j))/norm;
			pix(i,j)=255.99/(pow(abs(height)/(10.*thickness),sharpness)+1.);
		}
}

void generate(stripes_grid &grid, const dtocx &gen,
	std::default_random_engine &rng, double alpha, double exponent)
{
	grid.clear();
	int i,j, size=grid.get_size();
	stripes_grid_norm norm(size,grid.get_group());
	auto randfunc = [&] () { return random_levy_1d(alpha,1.,rng); };
	for(j=0;j<size;j++)
		for(i=0;i<size;i++)
			grid(i,j) = gen(randfunc);
	grid.transform();
	for(j=0;j<size;j++)
		for(i=0;i<size;i++)
			grid(i,j) *= pow(norm(i,j),exponent/2.);
	grid.transform();
}

void paintsquiggles::fill_one(symmetric_canvas<uint8_t> &grid,std::default_random_engine &rng,
	double alpha, double exponent, double thickness, double sharpness)
{
	stripes_grid g(grid.size(),grid.group());
	fill_one(grid,rng,alpha,exponent,thickness,sharpness,g);
}

void paintsquiggles::fill_one(symmetric_canvas<uint8_t> &grid,std::default_random_engine &rng,
	double alpha, double exponent, double thickness, double sharpness, stripes_grid &sgr)
{
	generate(sgr,[] (const function<double()> &f) { return complex<double>(f(),0); },rng,alpha,exponent);
	fill(sgr,grid.unsafe_get_canvas(),(stripes_grid::proj_t)std::real,thickness,sharpness);
}

void paintsquiggles::fill_two(symmetric_canvas<uint8_t> &g1, symmetric_canvas<uint8_t> &g2,
	std::default_random_engine &rng, double alpha, double exponent, double thickness, 
	double sharpness)
{
	stripes_grid g(g1.size(),g1.group());
	fill_two(g1,g2,rng,alpha,exponent,thickness,sharpness,g);
}

void paintsquiggles::fill_two(symmetric_canvas<uint8_t> &g1, symmetric_canvas<uint8_t> &g2,
	std::default_random_engine &rng, double alpha, double exponent, double thickness, 
	double sharpness, stripes_grid &sgr)
{
	generate(sgr,[] (const function<double()> &f) { return complex<double>(f(),f()); },rng,alpha,exponent);
	fill(sgr,g1.unsafe_get_canvas(),(stripes_grid::proj_t)std::real,thickness,sharpness);
	fill(sgr,g2.unsafe_get_canvas(),(stripes_grid::proj_t)std::imag,thickness,sharpness);
}


