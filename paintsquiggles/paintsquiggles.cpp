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
		l.color = random_color();
		l.pastel = false;
		generate(grid,norm);
		fill(grid,l.pixels);
	}
	merge();
}

void paintsquiggles::fill(const stripes_grid &grid,vector<unsigned char> &pix)
{
	double norm=grid.intensity();
	norm=sqrt(norm)/(size*64);
	double height, alpha;
	int i, j;
	for(i=0;i<size;i++)
		for(j=0;j<size;j++) {
			height=grid.get_symmetric(i,j).real()/norm;
			pix[i+size*j]=255.99/(pow(abs(height)/(10.*thickness),sharpness)+1.);
		}
}

void paintsquiggles::generate(stripes_grid &grid, function<double(int,int)> &f) {
	grid.clear();
	int i,j;
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			grid(i,j) = random_levy_1d(levy_alpha,1.);
	grid.transform();
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			grid(i,j) *= pow(f(i,j),exponent/2.);
	grid.transform();
}
