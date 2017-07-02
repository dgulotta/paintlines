/***************************************************************************
 *   Copyright (C) 2013, 2016 by Daniel Gulotta                            *
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

#include <cmath>
#include "hyperbolic_polygons.hpp"

hyperbolic_coord hyperbolic_polygon::edge(int n) const
{
	auto c=dirs[n];
	return hyperbolic_coord(c.x*rcosh,c.y*rcosh,rsinh);
}

hyperbolic_coord hyperbolic_polygon::vertex(int n) const
{
	return normalize(cross(edge((n+dirs.size()-1)%dirs.size()),edge(n)));
}

hyperbolic_transformation hyperbolic_polygon::trans(int n1,int n2,bool flip) const
{
	double r2c=2*rcosh*rcosh-1;
	double r2s=2*rcosh*rsinh;
	auto c1=dirs[n1], c2=dirs[n2];
	double s=flip?(-1):1;
	return hyperbolic_transformation(
		-r2c*c2.x*c1.x-s*c2.y*c1.y,-r2c*c2.x*c1.y+s*c2.y*c1.x,r2s*c2.x,
		-r2c*c2.y*c1.x+s*c2.x*c1.y,-r2c*c2.y*c1.y-s*c2.x*c1.x,r2s*c2.y,
		-r2s*c1.x,-r2s*c1.y,r2c);
}

hyperbolic_polygon::hyperbolic_polygon(double a1, double a2, double a3)
{
	double c1=cos(a1/2), c2=cos(a2/2), c3=cos(a3/2);
	rcosh=2*c1*c2*c3/sqrt((c1+c2+c3)*(c1+c2-c3)*(c1-c2+c3)*(-c1+c2+c3));
	rsinh=sqrt(rcosh*rcosh-1);
	c1/=rcosh; c3/=rcosh;
	double c1d=1-2*c1*c1, c3d=1-2*c3*c3;
	double s1d=2*c1*sqrt(1-c1*c1), s3d=2*c3*sqrt(1-c3*c3);
	dirs = {{c1d,s1d},{c3d,-s3d},{1,0}};
}

hyperbolic_polygon::hyperbolic_polygon(double a1, double a2, double a3, double a4)
{
	double c1 = cos(a1/2), c2 = cos(a2/2), c3 = cos(a3/2), c4=cos(a4/2);
	rcosh=2*sqrt((c1*c2+c3*c4)*(c1*c3+c2*c4)*(c1*c4+c2*c3)/
		((c1+c2+c3-c4)*(c1+c2-c3+c4)*(c1-c2+c3+c4)*(-c1+c2+c3+c4)));
	rsinh=sqrt(rcosh*rcosh-1);
	c1/=rcosh; c2/=rcosh; c4/=rcosh;
	double c1d=1-2*c1*c1, c2d=1-2*c2*c2, c4d=1-2*c4*c4;
	double s1d=2*c1*sqrt(1-c1*c1), s2d=2*c2*sqrt(1-c2*c2), s4d=2*c4*sqrt(1-c4*c4);
	dirs = {{c1d,s1d},{c1d*c2d-s1d*s2d,c1d*s2d+s1d*c2d},{c4d,-s4d},{1,0}};
}
