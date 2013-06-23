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
#include "paintstripes.h"

void paintstripes::paint(int sz, symgroup sym)
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
	complex<double> z;
	grid.generate(norm,levy_alpha);
	fill(red,grid);
	grid.generate(norm,levy_alpha);
	fill(blue,grid);
	grid.generate(norm,levy_alpha);
	fill(green,grid);
}

void paintstripes::fill(vector<unsigned char> &arr, const stripes_grid &g)
{
	int i,j;
	double norm(0.);
	for(i=0;i<size;i++)
		for(j=0;j<size;j++) {
			double d=g.get_symmetric(i,j).real();
			norm+=d*d;
		}
	norm=sqrt(norm)/(size*64);
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			arr[i+size*j]=colorchop(128.+g.get_symmetric(i,j).real()/norm);
}
