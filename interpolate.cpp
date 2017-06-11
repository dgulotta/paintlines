/***************************************************************************
 *   Copyright (C) 2014 by Daniel Gulotta                                  *
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

#include "interpolate.h"
#include <cmath>

using std::make_tuple;

interpolation_list interpolate_linear(double x, double y)
{
	int x0 = int(floor(x)), y0 = int(floor(y));
	double xt = x-x0, yt = y-y0;
	return {
		make_tuple(x0,y0,(1-xt)*(1-yt)),
		make_tuple(x0+1,y0,xt*(1-yt)),
		make_tuple(x0,y0+1,(1-xt)*yt),
		make_tuple(x0+1,y0+1,xt*yt)
	};
}

interpolation_list interpolate_cubic(double x, double y)
{
	int x0 = int(floor(x)), y0 = int(floor(y));
	double xt = x-x0, yt = y-y0;
	double xf0 = -.5 * (xt-1)*(xt-1)*xt;
	double xf1 = (1.5*xt-2.5)*xt*xt+1;
	double xf2 = (-1.5*xt*xt+2*xt+.5)*xt;
	double xf3 = .5 * (xt-1) * xt * xt;
	double yf0 = -.5 * (yt-1)*(yt-1)*yt;
	double yf1 = (1.5*yt-2.5)*yt*yt+1;
	double yf2 = (-1.5*yt*yt+2*yt+.5)*yt;
	double yf3 = .5 * (yt-1) * yt * yt;
	return {
		make_tuple(x0-1,y0-1,xf0*yf0),
		make_tuple(x0,y0-1,xf1*yf0),
		make_tuple(x0+1,y0-1,xf2*yf0),
		make_tuple(x0+2,y0-1,xf3*yf0),
		make_tuple(x0-1,y0,xf0*yf1),
		make_tuple(x0,y0,xf1*yf1),
		make_tuple(x0+1,y0,xf2*yf1),
		make_tuple(x0+2,y0,xf3*yf1),
		make_tuple(x0-1,y0+1,xf0*yf2),
		make_tuple(x0,y0+1,xf1*yf2),
		make_tuple(x0+1,y0+1,xf2*yf2),
		make_tuple(x0+2,y0+1,xf3*yf2),
		make_tuple(x0-1,y0+2,xf0*yf3),
		make_tuple(x0,y0+2,xf1*yf3),
		make_tuple(x0+1,y0+2,xf2*yf3),
		make_tuple(x0+2,y0+2,xf3*yf3),
	};
}

interpolation_list interpolate_closest(double x, double y)
{
	return { make_tuple(int(round(x)),int(round(y)),1.) };
}
