/***************************************************************************
 *   Copyright (C) 2005, 2013 by Daniel Gulotta                            *
 *   dgulotta@alum.mit.edu                                                 *
 *   Portions copyright (C)  1996, 1997, 1998, 1999, 2000 James Theiler,   *
 *   Brian Gough                                                           *
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
	double (stripes_grid::*norm)(int,int);
	switch(sym) {
	case SYM_P3:
	case SYM_P31M:
	case SYM_P3M1:
	case SYM_P6:
	case SYM_P6M:
    	norm=&stripes_grid::norm_hexagonal;
		break;
	default:
		norm=&stripes_grid::norm_orthogonal;
  	}
	std::fill(red.begin(),red.end(),0);
	std::fill(green.begin(),green.end(),0);
	std::fill(blue.begin(),blue.end(),0);
	int n(n_colors);
	while(n--) {
		switch(random_int(6)) {
			case 0:
				redbrush=255;
				greenbrush=random_int(256);
				bluebrush=0;
				break;
			case 1:
				redbrush=0;
				greenbrush=255;
				bluebrush=random_int(256);
				break;
			case 2:
				redbrush=random_int(256);
				greenbrush=0;
				bluebrush=255;
				break;
			case 3:
				redbrush=random_int(256);
				greenbrush=255;
				bluebrush=0;
				break;
			case 4:
				redbrush=0;
				greenbrush=random_int(256);
				bluebrush=255;
				break;
			default:
				redbrush=255;
				greenbrush=0;
				bluebrush=random_int(256);
		}
		grid.generate(norm,levy_alpha);
		fill(grid);
	}
}

void paintsquiggles::fill(const stripes_grid &grid)
{
	int i,j;
	double norm(0.);
	for(i=0;i<size;i++)
		for(j=0;j<size;j++) {
			double d=grid.get_symmetric(i,j).real();
			norm+=d*d;
		}
	norm=sqrt(norm)/(size*64);
	double height, alpha;
	for(i=0;i<size;i++)
		for(j=0;j<size;j++) {
			height=grid.get_symmetric(i,j).real()/norm;
			alpha=1./(pow(abs(height)/(10.*thickness),sharpness)+1.);
			red[i+size*j]=((1-alpha)*red[i+size*j]+alpha*redbrush);
			green[i+size*j]=((1-alpha)*green[i+size*j]+alpha*greenbrush);
			blue[i+size*j]=((1-alpha)*blue[i+size*j]+alpha*bluebrush);
		}
}
