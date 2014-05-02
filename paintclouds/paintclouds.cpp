/***************************************************************************
 *   Copyright (C) 2005-2008, 2013 by Daniel Gulotta                       *
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

#include <tuple>
#include "paintclouds.h"
#include "../randgen.h"

using std::max;
using std::min;

// I don't think this corresponds to any well-known distribution.
// I think I used to be using a normal distribution here, but
// decided that fatter tails made nicer pictures.
double paintclouds::rand_exp_cos(double a)
{
	return random_exponential(a)*cos(random_angle());
}

double paintclouds::rand_cauchy(double a)
{
	return .01*random_cauchy(a*a);
}

double paintclouds::rand_normal(double a)
{
	return 1.2*random_normal(a);
}

double paintclouds::rand_sechsquare(double a) {
	return .3*random_sechsquare(a);
}

uint8_t paintclouds::random_byte(short b1, short b2, int d)
{
	return colorchop((b1+b2)/2.+randfunc(d)+.5);
}

color_t paintclouds::random_color(color_t c1, color_t c2, int d)
{
	return color_t(random_byte(c1.red,c2.red,d),
		random_byte(c1.green,c2.green,d),random_byte(c1.blue,c2.blue,d));
}

void paintclouds::fill_tri(canvas<color_t> &tri)
{
	int sz=tri.height()-1;
	for(int d=sz/2;d>0;d/=2)
		for(int i=d;i<sz;i+=d)
			for(int j=d;i+j<sz;j+=d) {
				int di=(i&d), dj=(j&d);
				if(di||dj) {
					color_t c1 = tri(i-di,j+dj);
					color_t c2 = tri(i+di,j-dj);
					tri(i,j)=random_color(c1,c2,d);
				}
			}
}

void paintclouds::fill_line(canvas<color_t> &tri, int i1, int j1, int i2, int j2)
{
	int d = max(abs(i1-i2),abs(j1-j2))/2;
	if(d>0) {
		int mi=(i1+i2)/2, mj=(j1+j2)/2;
		color_t c1 = tri(i1,j1);
		color_t c2 = tri(i2,j2);
		tri(mi,mj)=random_color(c1,c2,d);
		fill_line(tri,i1,j1,mi,mj);
		fill_line(tri,mi,mj,i2,j2);
	}
}

int tri_size(int m)
{
	int n=4;
	while(n<m) n*=2;
	return n;
}

void copy(symmetric_canvas<color_t> &img, canvas<color_t> &tri, int x1, int y1, int x2, int y2, int x3, int y3, int scale=1)
{
	int d=tri.height()-1;
	long xmin=divide(min({x1,x2,x3}),scale), ymin=divide(min({y1,y2,y3}),scale);
	long xmax=-divide(-max({x1,x2,x3}),scale), ymax=-divide(-max({y1,y2,y3}),scale);
	long x,y,t=(x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2));
	for(x=xmin;x<=xmax;x++)
		for(y=ymin;y<=ymax;y++) {
			long a1 = scale*x*(y2-y3)+x2*(y3-scale*y)+x3*(scale*y-y2);
			long a2 = x1*(scale*y-y3)+scale*x*(y3-y1)+x3*(y1-scale*y);
			long a3 = x1*(y2-scale*y)+x2*(scale*y-y1)+scale*x*(y1-y2);
			if(a1*t>=0&&a2*t>=0&&a3*t>=0) {
				img(x,y)=tri((d*a2)/t,(d*a3)/t);
			}
		}
}

void paintclouds::fill_tri_ab2(canvas<color_t> &tri) {
	int d = tri.height()-1;
	tri(0,0)=color_t(red1,green1,blue1);
	tri(d,0)=color_t(red2,green2,blue2);
	tri(d/2,d/2)=color_t(red3,green3,blue3);
	fill_line(tri,0,0,d,0);
	fill_line(tri,d,0,d/2,d/2);
	for(int i=0;i<=d;i++) tri(0,i)=tri(i,0);
	for(int i=0;i<d/2;i++) tri(i,d-i)=tri(d-i,i);
	fill_tri(tri);
}

void paintclouds::fill_tri_sabc(canvas<color_t> &tri) {
	int d = tri.height()-1;
	tri(0,0)=color_t(red1,green1,blue1);
	tri(d,0)=color_t(red2,green2,blue2);
	tri(0,d)=color_t(red3,green3,blue3);
	fill_line(tri,0,0,d,0);
	fill_line(tri,0,0,0,d);
	fill_line(tri,d,0,0,d);
	fill_tri(tri);
}

void paintclouds::fill_tri_asb(canvas<color_t> &tri) {
	int d = tri.height()-1;
	tri(0,0)=color_t(red1,green1,blue1);
	tri(d,0)=color_t(red2,green2,blue2);
	tri(d/2,d/2)=color_t(red3,green3,blue3);
	fill_line(tri,0,0,d,0);
	fill_line(tri,d,0,d/2,d/2);
	for(int i=0;i<=d;i++) tri(0,i)=tri(i,0);
	fill_line(tri,0,d,d/2,d/2);
	fill_tri(tri);
}

void paintclouds::paint(int size, symgroup sym)
{
	grid=symmetric_canvas<color_t>(size,sym);
	int halfsize=size/2;
	switch(sym) {
	case SYM_CM:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		tri(0,0)=tri(d,0)=color_t(red1,green1,blue1);
		tri(d/2,0)=color_t(red2,green2,blue2);
		tri(d/2,d/2)=color_t(red3,green3,blue3);
		fill_line(tri,0,0,d/2,0);
		fill_line(tri,d/2,0,d,0);
		for(int i=0;i<=d;i++)
			tri(0,i)=tri(d-i,0);
		fill_line(tri,d,0,d/2,d/2);
		fill_line(tri,0,d,d/2,d/2);
		fill_tri(tri);
		copy(grid,tri,size,0,size,size,0,0);
		break;
	}
	case SYM_CMM:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		tri(d,0)=color_t(red1,green1,blue1);
		tri(0,0)=color_t(red2,green2,blue2);
		tri(d/2,d/2)=color_t(red3,green3,blue3);
		fill_line(tri,d,0,d/2,d/2);
		for(int i=0;i<d/2;i++) tri(i,d-i)=tri(d-i,i);
		fill_line(tri,0,0,d,0);
		fill_line(tri,0,0,0,d);
		fill_tri(tri);
		copy(grid,tri,halfsize,halfsize,0,0,size,0);
		break;
	}
	case SYM_P1:
	{
		int d = tri_size(size);
		canvas<color_t> tri1(d+1,d+1), tri2(d+1,d+1);
		tri1(0,0)=tri1(d,0)=tri1(0,d)=color_t(red1,green1,blue1);
		tri1(d/2,0)=color_t(red2,green2,blue2);
		tri1(0,d/2)=color_t(red3,green3,blue3);
		fill_line(tri1,0,0,d/2,0);
		fill_line(tri1,d/2,0,d,0);
		fill_line(tri1,0,0,0,d/2);
		fill_line(tri1,0,d/2,0,d);
		fill_line(tri1,0,d,d,0);
		for(int i=0;i<=d;i++) {
			tri2(i,0)=tri1(d-i,0);
			tri2(0,i)=tri1(0,d-i);
			tri2(i,d-i)=tri1(d-i,i);
		}
		fill_tri(tri1);
		fill_tri(tri2);
		copy(grid,tri1,0,0,0,size,size,0);
		copy(grid,tri2,size,size,size,0,0,size);
		break;
	}
	case SYM_P2:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		tri(0,0)=tri(d,0)=color_t(((int)red1+red2+red3)/3,
			((int)green1+green2+green3)/3,((int)blue1+blue2+blue3)/3);
		tri(d/2,0)=color_t(red1,green1,blue1);
		tri(0,d/2)=color_t(red2,green2,blue2);
		tri(d/2,d/2)=color_t(red3,green3,blue3);
		fill_line(tri,0,0,d/2,0);
		fill_line(tri,0,0,0,d/2);
		fill_line(tri,d,0,d/2,d/2);
		for(int i=0;i<d/2;i++) {
			tri(d-i,0)=tri(i,0);
			tri(0,d-i)=tri(0,i);
			tri(i,d-i)=tri(d-i,i);
		}
		fill_tri(tri);
		copy(grid,tri,0,0,size,0,0,size);
		break;
	}
	case SYM_P3:
	{
		int d = tri_size((2*size+1)/3);
		canvas<color_t> tri1(d+1,d+1), tri2(d+1,d+1);
		tri1(0,0)=color_t(red1,green1,blue1);
		tri1(d,0)=color_t(red2,green2,blue2);
		tri1(0,d)=color_t(red3,green3,blue3);
		fill_line(tri1,0,0,d,0);
		fill_line(tri1,0,0,0,d);
		fill_line(tri1,d,0,0,d);
		for(int i=0;i<=d;i++) {
			tri2(i,0)=tri1(i,0);
			tri2(0,i)=tri1(0,i);
			tri2(i,d-i)=tri1(i,d-i);
		}
		fill_tri(tri1);
		fill_tri(tri2);
		copy(grid,tri1,0,0,size,size,2*size,-size,3);
		copy(grid,tri2,0,0,size,size,-size,2*size,3);
		break;
	}
	case SYM_P31M:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		fill_tri_asb(tri);
		copy(grid,tri,size,size,3*size,0,0,3*size,3);
		break;
	}
	case SYM_P3M1:
	{
		int d = tri_size((2*size+1)/3);
		canvas<color_t> tri(d+1,d+1);
		fill_tri_sabc(tri);
		copy(grid,tri,0,0,size,size,2*size,-size,3);
		break;
	}
	case SYM_P4:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		fill_tri_ab2(tri);
		copy(grid,tri,halfsize,halfsize,size,0,0,0);
		break;
	}
	case SYM_P4G:
	{
		int d = tri_size(halfsize);
		canvas<color_t> tri(d+1,d+1);
		fill_tri_asb(tri);
		copy(grid,tri,0,0,0,halfsize,halfsize,0);
		break;
	}
	case SYM_P4M:
	{
		int d = tri_size(halfsize);
		canvas<color_t> tri(d+1,d+1);
		fill_tri_sabc(tri);
		copy(grid,tri,0,0,halfsize,0,halfsize,halfsize);
		break;
	}
	case SYM_P6:
	{
		int d = tri_size((2*size+1)/3);
		canvas<color_t> tri(d+1,d+1);
		fill_tri_ab2(tri);
		copy(grid,tri,0,0,size,size,2*size,-size,3);
		break;
	}
	case SYM_P6M:
	{
		int d = tri_size((2*size+1)/3);
		canvas<color_t> tri(d+1,d+1);
		fill_tri_sabc(tri);
		copy(grid,tri,0,0,2*size,2*size,3*size,0,6);
		break;
	}
	case SYM_PG:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		tri(0,0)=tri(d,0)=tri(d/2,d/2)=color_t(red1,green1,blue1);
		tri(d/2,0)=tri(0,d/2)=color_t(red2,green2,blue2);
		tri(3*d/4,d/4)=color_t(red3,green3,blue3);
		fill_line(tri,d,0,3*d/4,d/4);
		fill_line(tri,d/2,d/2,3*d/4,d/4);
		for(int i=0;i<=d/2;i++)
			tri(d/2-i,d/2+i)=tri(d-i,i);
		fill_line(tri,0,0,d/2,0);
		fill_line(tri,d,0,d/2,0);
		for(int i=0;i<=d;i++) tri(0,i)=tri(d-i,0);
		fill_tri(tri);
		copy(grid,tri,2*size,3*size,0,-size,4*size,-size,4);
		break;
	}
	case SYM_PGG:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		tri(0,0)=tri(d,0)=tri(0,d)=color_t(red1,green1,blue1);
		tri(d/2,d/2)=color_t(red2,green2,blue2);
		tri(d/2,0)=color_t(red3,green3,blue3);
		fill_line(tri,0,0,d/2,0);
		fill_line(tri,d,0,d/2,0);
		fill_line(tri,d,0,d/2,d/2);
		for(int i=0;i<=d;i++) tri(0,i)=tri(d-i,0);
		for(int i=0;i<d/2;i++) tri(i,d-i)=tri(d-i,i);
		fill_tri(tri);
		copy(grid,tri,halfsize,0,0,-halfsize,0,halfsize);
		break;
	}
	case SYM_PM:
	{
		int d = tri_size(size);
		canvas<color_t> tri1(d+1,d+1), tri2(d+1,d+2);
		tri1(0,0)=color_t(red1,green1,blue1);
		tri1(d,0)=tri1(0,d)=color_t(red2,green2,blue2);
		tri1(d/2,d/2)=color_t(red3,green3,blue3);
		fill_line(tri1,0,0,d,0);
		fill_line(tri1,0,0,0,d);
		fill_line(tri1,d,0,d/2,d/2);
		fill_line(tri1,0,d,d/2,d/2);
		for(int i=0;i<=d;i++) {
			tri2(i,0)=tri1(d-i,0);
			tri2(0,i)=tri1(0,d-i);
		}
		fill_line(tri2,d,0,0,d);
		fill_tri(tri1);
		fill_tri(tri2);
		copy(grid,tri1,halfsize,halfsize,0,size,0,0);
		copy(grid,tri2,0,0,halfsize,-halfsize,halfsize,halfsize);
		break;
	}
	case SYM_PMG:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		tri(0,0)=color_t(red1,green1,blue1);
		tri(d/2,0)=color_t(red2,green2,blue2);
		tri(0,d/2)=color_t(red3,green3,blue3);
		fill_line(tri,0,0,d/2,0);
		fill_line(tri,0,0,0,d/2);
		for(int i=0;i<d/2;i++) {
			tri(d-i,0)=tri(i,0);
			tri(0,d-i)=tri(0,i);
		}
		fill_line(tri,d,0,0,d);
		fill_tri(tri);
		copy(grid,tri,3*size,3*size,size,size,size,5*size,4);
		break;
	}
	case SYM_PMM:
	{
		int d = tri_size(halfsize);
		canvas<color_t> tri1(d+1,d+1), tri2(d+1,d+1);
		tri1(0,0)=tri2(0,0)=color_t(red1,green1,blue1);
		tri1(d,0)=color_t(red2,green2,blue2);
		tri1(0,d)=color_t(red3,green3,blue3);
		fill_line(tri1,0,0,d,0);
		fill_line(tri1,0,0,0,d);
		fill_line(tri1,d,0,0,d);
		for(int i=0;i<=d;i++)
			tri2(i,d-i)=tri1(d-i,i);
		fill_line(tri2,0,0,d,0);
		fill_line(tri2,0,0,0,d);
		fill_tri(tri1);
		fill_tri(tri2);
		copy(grid,tri1,0,0,0,halfsize,halfsize,0);
		copy(grid,tri2,halfsize,halfsize,halfsize,0,0,halfsize);
		break;
	}}
}
