/***************************************************************************
 *   Copyright (C) 2005, 2013-2014, 2016 by Daniel Gulotta                 *
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
using std::vector;

typedef std::function<complex<double>(const std::function<double()> &)> dtocx;

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

void fill(const stripes_grid &grid,canvas<uint8_t> &pix,const function<double(const complex<double> &)> &proj,double thickness,double sharpness)
{
	double norm=grid.intensity(proj);
	norm=sqrt(norm)/(grid.get_size()*64);
	double height;
	int i, j;
	for(j=0;j<grid.get_size();j++)
		for(i=0;i<grid.get_size();i++) {
			height=proj(grid.get_symmetric(i,j))/norm;
			pix(i,j)=255.99/(pow(abs(height)/(10.*thickness),sharpness)+1.);
		}
}

void fill(symmetric_canvas<uint8_t> *grid1,symmetric_canvas<uint8_t> *grid2,
	std::default_random_engine &rng, double alpha, double exponent,
	double thickness, double sharpness,stripes_grid &sgr)
{
	if(grid2) {
		generate(sgr,[] (const function<double()> &f) { return complex<double>(f(),f()); },rng,alpha,exponent);
		fill(sgr,grid2->unsafe_get_canvas(),(stripes_grid::proj_t)std::imag,thickness,sharpness);
	}
	else {
		generate(sgr,[] (const function<double()> &f) { return complex<double>(f(),0); },rng,alpha,exponent);
	}
	fill(sgr,grid1->unsafe_get_canvas(),(stripes_grid::proj_t)std::real,thickness,sharpness);
}

vector<symmetric_canvas<uint8_t>> paint_squiggles(size_t ncolors, size_t size, symgroup sg,
	double alpha, double exponent, double thickness, double sharpness)
{
	vector<symmetric_canvas<uint8_t>> grids(ncolors);
	vector<std::future<void>> futures((ncolors+1)/2);
	std::random_device rd;
	for(size_t i=0;i<futures.size();i++) {
		symmetric_canvas<uint8_t> *grid1=&(grids[2*i]), *grid2;
		(*grid1)=symmetric_canvas<uint8_t>(size,sg);
		if(2*i+1<ncolors) {
			grid2=&(grids[2*i+1]);
			(*grid2)=symmetric_canvas<uint8_t>(size,sg);
		}
		else grid2=nullptr;
		auto g = std::make_unique<stripes_grid>(size,sg);
		auto seed=rd();
		futures[i]=std::async(std::launch::async,[g=std::move(g),grid1,grid2,seed,alpha,exponent,thickness,sharpness]() {
			std::default_random_engine rng(seed);
			fill(grid1,grid2,rng,alpha,exponent,thickness,sharpness,*g);
		});
	}
	for(auto &f : futures)
		f.wait();
	return grids;
}
