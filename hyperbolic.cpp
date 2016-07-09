/***************************************************************************
 *   Copyright (C) 2005-2008, 2013, 2016 by Daniel Gulotta                 *
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
#include "hyperbolic.h"

hyperbolic_coord normalize(const hyperbolic_coord &c)
{
	double n=c*c;
	if(n>0.) return c/sqrt(n);
	else if(n<0.) return c/sqrt(-n);
	else return c;
}

hyperbolic_coord & inplace_normalize(hyperbolic_coord &c)
{
	double n=c*c;
	if(n>0.) c/=sqrt(n);
	else if(n<0.) c/=sqrt(-n);
	return c;
}

hyperbolic_coord cross(const hyperbolic_coord &a, const hyperbolic_coord &b)
{
	return hyperbolic_coord(a.z*b.y-a.y*b.z,a.x*b.z-a.z*b.x,a.x*b.y-a.y*b.x);
}

hyperbolic_coord midpoint(const hyperbolic_coord &a, const hyperbolic_coord &b)
{
	return normalize(a+b);
}

const hyperbolic_transformation hyperbolic_transformation::identity(1.,0.,0.,0.,1.,0.,0.,0.,1.);

hyperbolic_transformation hyperbolic_transformation::reflection(const hyperbolic_coord &c)
{
	double xy=-2*c.x*c.y;
	double xz=2*c.x*c.z;
	double yz=2*c.y*c.z;
	return hyperbolic_transformation(
		1-2*c.x*c.x,
		xy,
		xz,
		xy,
		1-2*c.y*c.y,
		yz,
		-xz,
		-yz,
		1+2*c.z*c.z);
}

hyperbolic_transformation hyperbolic_transformation::rotation_180(const hyperbolic_coord &c)
{
	auto xy=-2*c.x*c.y;
	auto xz=2*c.x*c.z;
	auto yz=2*c.y*c.z;
	return hyperbolic_transformation(
		-2*c.x*c.x-1,
		xy,
		xz,
		xy,
		-2*c.y*c.y-1,
		yz,
		-xz,
		-yz,
		2*c.z*c.z-1);
}

hyperbolic_transformation hyperbolic_transformation::rotation_origin(double a)
{
	double c=cos(a), s=sin(a);
	return hyperbolic_transformation(c,-s,0,s,c,0,0,0,1);
}

hyperbolic_transformation hyperbolic_transformation::rotation(double a, const hyperbolic_coord &h)
{
	double c=cos(a), s=sin(a);
	return hyperbolic_transformation(
		c+(c-1)*h.x*h.x,
		(c-1)*h.x*h.y-s*h.z,
		(1-c)*h.x*h.z+s*h.y,
		(c-1)*h.y*h.x+s*h.z,
		c+(c-1)*h.y*h.y,
		(1-c)*h.y*h.z-s*h.x,
		(c-1)*h.z*h.x+s*h.y,
		(c-1)*h.z*h.y-s*h.x,
		c+(1-c)*h.z*h.z);
}

hyperbolic_transformation hyperbolic_transformation::glide_reflection(const hyperbolic_coord &c)
{
	double z=sqrt(1-c*c);
	double d=1/(1-z);
	return hyperbolic_transformation(
		z+d*c.x*c.x,
		-c.z+d*c.x*c.y,
		c.y-d*c.x*c.z,
		c.z+d*c.y*c.x,
		z+d*c.y*c.y,
		-c.x-d*c.y*c.z,
		c.y+d*c.z*c.x,
		-c.x+d*c.z*c.y,
		z-d*c.z*c.z);
}

// this should be the same as the above, except for rounding
hyperbolic_transformation hyperbolic_transformation::glide_reflection(const hyperbolic_coord &from, const hyperbolic_coord &to)
{
	hyperbolic_coord c=cross(from,to);
	double z=from*to;
	double d=1/(1-z);
	return hyperbolic_transformation(
		z+d*c.x*c.x,
		-c.z+d*c.x*c.y,
		c.y-d*c.x*c.z,
		c.z+d*c.y*c.x,
		z+d*c.y*c.y,
		-c.x-d*c.y*c.z,
		c.y+d*c.z*c.x,
		-c.x+d*c.z*c.y,
		z-d*c.z*c.z);
}

hyperbolic_transformation hyperbolic_transformation::translation(const hyperbolic_coord &e1, const hyperbolic_coord &e2)
{
	hyperbolic_coord e = cross(e2,e1);
	double c = e1*e2;
	double d = 1/(c+1);
	return hyperbolic_transformation(
		c - d * e.x * e.x,
		-e.z - d * e.x * e.y,
		e.y + d * e.x * e.z,
		e.z - d * e.y * e.x,
		c - d * e.y * e.y,
		-e.x + d * e.y * e.z,
		e.y - d * e.z * e.x,
		-e.x - d * e.z * e.y,
		c + d * e.z * e.z);
}

planar_coord projection(const hyperbolic_coord &hc,projtype pt)
{
	double denom=(pt==projtype::POINCARE)?(1+hc.z):hc.z;
	return planar_coord(hc.x/denom,hc.y/denom);
}

hyperbolic_coord inverse_projection(const planar_coord &pc,projtype pt)
{
	if(pt==projtype::POINCARE) {
		double r = (1-pc.x*pc.x-pc.y*pc.y)/2;
		return hyperbolic_coord(pc.x/r,pc.y/r,(1-r)/r);
	}
	else {
		double r = sqrt(1-pc.x*pc.x-pc.y*pc.y);
		return hyperbolic_coord(pc.x/r,pc.y/r,1/r);
	}
}

