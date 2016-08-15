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

#include <tuple>
#include "paintclouds.h"
#include "../randgen.h"
#include "../converter/converters.h"

using std::max;
using std::min;

// I don't think this corresponds to any well-known distribution.
// I think I used to be using a normal distribution here, but
// decided that fatter tails made nicer pictures.
double clouds_rand_exp_cos(double a)
{
	return random_exponential(a)*cos(random_angle());
}

double clouds_rand_cauchy(double a)
{
	return .01*random_cauchy(a*a);
}

double clouds_rand_normal(double a)
{
	return 1.2*random_normal(a);
}

double clouds_rand_sechsquare(double a) {
	return .3*random_sechsquare(a);
}

uint8_t random_byte(short b1, short b2, int d,clouds_randfunc r)
{
	return colorchop((b1+b2)/2.+r(d)+.5);
}

color_t random_color(color_t c1, color_t c2, int d,clouds_randfunc r)
{
	return color_t(random_byte(c1.red,c2.red,d,r),
		random_byte(c1.green,c2.green,d,r),random_byte(c1.blue,c2.blue,d,r));
}

void fill_tri(canvas<color_t> &tri,clouds_randfunc r,bool quad=false)
{
	int sz=tri.height()-1;
	for(int d=sz/2;d>0;d/=2)
		for(int i=d;i<sz;i+=d)
			for(int j=d;i+j<sz;j+=d) {
				int di=(i&d), dj=(j&d);
				if(di||dj) {
					color_t c1 = tri(i-di,j+dj);
					color_t c2 = tri(i+di,j-dj);
					tri(i,j)=random_color(c1,c2,d,r);
					if(quad) {
						color_t c3 = tri(sz-i+di,sz-j-dj);
						color_t c4 = tri(sz-i-di,sz-j+dj);
						tri(sz-i,sz-j)=random_color(c3,c4,d,r);
					}
				}
			}
}

void fill_line(canvas<color_t> &tri, int i1, int j1, int i2, int j2,clouds_randfunc r)
{
	int d = max(abs(i1-i2),abs(j1-j2))/2;
	if(d>0) {
		int mi=(i1+i2)/2, mj=(j1+j2)/2;
		color_t c1 = tri(i1,j1);
		color_t c2 = tri(i2,j2);
		tri(mi,mj)=random_color(c1,c2,d,r);
		fill_line(tri,i1,j1,mi,mj,r);
		fill_line(tri,mi,mj,i2,j2,r);
	}
}

int tri_size(int m)
{
	int n=4;
	while(n<m) n*=2;
	return n;
}

void copy(symmetric_canvas<color_t> &img, canvas<color_t> &tri, int x1, int y1, int x2, int y2, int x3, int y3, int scale=1, bool reverse=false)
{
	int d=tri.height()-1;
	long long xmin=divide(min({x1,x2,x3}),scale), ymin=divide(min({y1,y2,y3}),scale);
	long long xmax=-divide(-max({x1,x2,x3}),scale), ymax=-divide(-max({y1,y2,y3}),scale);
	long long t=(x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2));
	for(long long x=xmin;x<=xmax;x++)
		for(long long y=ymin;y<=ymax;y++) {
			long long a1 = scale*x*(y2-y3)+x2*(y3-scale*y)+x3*(scale*y-y2);
			long long a2 = x1*(scale*y-y3)+scale*x*(y3-y1)+x3*(y1-scale*y);
			long long a3 = x1*(y2-scale*y)+x2*(scale*y-y1)+scale*x*(y1-y2);
			if(a1*t>=0&&a2*t>=0&&a3*t>=0) {
				int tx = (d*a2)/t, ty = (d*a3)/t;
				if(reverse)
					img(x,y)=tri(d-tx,d-ty);
				else
					img(x,y)=tri(tx,ty);
			}
		}
}

canvas<color_t> tri_sax(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=tri(d,0)=col1;
	tri(d/2,0)=col2;
	tri(d/2,d/2)=col3;
	fill_line(tri,0,0,d/2,0,r);
	fill_line(tri,d/2,0,d,0,r);
	for(int i=0;i<=d;i++)
		tri(0,i)=tri(d-i,0);
	fill_line(tri,d,0,d/2,d/2,r);
	fill_line(tri,0,d,d/2,d/2,r);
	fill_tri(tri,r);
	return tri;
}

canvas<color_t> tri_2sab(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(d,0)=col1;
	tri(0,0)=col2;
	tri(d/2,d/2)=col3;
	fill_line(tri,d,0,d/2,d/2,r);
	for(int i=0;i<d/2;i++) tri(i,d-i)=tri(d-i,i);
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,0,0,0,d,r);
	fill_tri(tri,r);
	return tri;
}

canvas<color_t> tri_a222(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=tri(d,0)=color_t(
			((int)col1.red+col2.red+col3.red)/3,
			((int)col1.green+col2.green+col3.green)/3,
			((int)col1.blue+col2.blue+col3.blue)/3);
	tri(d/2,0)=col1;
	tri(0,d/2)=col2;
	tri(d/2,d/2)=col3;
	fill_line(tri,0,0,d/2,0,r);
	fill_line(tri,0,0,0,d/2,r);
	fill_line(tri,d,0,d/2,d/2,r);
	for(int i=0;i<d/2;i++) {
		tri(d-i,0)=tri(i,0);
		tri(0,d-i)=tri(0,i);
		tri(i,d-i)=tri(d-i,i);
	}
	fill_tri(tri,r);
	return tri;
}

canvas<color_t> tri_ab2(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d,0)=col2;
	tri(d/2,d/2)=col3;
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,d,0,d/2,d/2,r);
	for(int i=0;i<=d;i++) tri(0,i)=tri(i,0);
	for(int i=0;i<d/2;i++) tri(i,d-i)=tri(d-i,i);
	fill_tri(tri,r);
	return tri;
}

canvas<color_t> tri_sabc(int sz,color_t col1,color_t col2, color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d,0)=col2;
	tri(0,d)=col3;
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,0,0,0,d,r);
	fill_line(tri,d,0,0,d,r);
	fill_tri(tri,r);
	return tri;
}

canvas<color_t> tri_asb(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d,0)=col2;
	tri(d/2,d/2)=col3;
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,d,0,d/2,d/2,r);
	for(int i=0;i<=d;i++) tri(0,i)=tri(i,0);
	fill_line(tri,0,d,d/2,d/2,r);
	fill_tri(tri,r);
	return tri;
}

canvas<color_t> tri_a2x(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=tri(d,0)=tri(0,d)=col1;
	tri(d/2,d/2)=col2;
	tri(d/2,0)=col3;
	fill_line(tri,0,0,d/2,0,r);
	fill_line(tri,d,0,d/2,0,r);
	fill_line(tri,d,0,d/2,d/2,r);
	for(int i=0;i<=d;i++) tri(0,i)=tri(d-i,0);
	for(int i=0;i<d/2;i++) tri(i,d-i)=tri(d-i,i);
	fill_tri(tri,r);
	return tri;
}

canvas<color_t> tri_22sa(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d/2,0)=col2;
	tri(0,d/2)=col3;
	fill_line(tri,0,0,d/2,0,r);
	fill_line(tri,0,0,0,d/2,r);
	for(int i=0;i<d/2;i++) {
		tri(d-i,0)=tri(i,0);
		tri(0,d-i)=tri(0,i);
	}
	fill_line(tri,d,0,0,d,r);
	fill_tri(tri,r);
	return tri;
}

canvas<color_t> quad_sabcd(int sz,color_t col1,color_t col2, color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=tri(d,d)=col1;
	tri(d,0)=col2;
	tri(0,d)=col3;
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,0,0,0,d,r);
	fill_line(tri,d,0,0,d,r);
	fill_line(tri,d,d,0,d,r);
	fill_line(tri,d,d,d,0,r);
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_ao(int sz,color_t col1,color_t col2, color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=tri(d,0)=tri(0,d)=col1;
	tri(d/2,0)=col2;
	tri(0,d/2)=col3;
	fill_tri(tri,r,true);
	fill_line(tri,0,0,d/2,0,r);
	fill_line(tri,d/2,0,d,0,r);
	fill_line(tri,0,0,0,d/2,r);
	fill_line(tri,0,d/2,0,d,r);
	fill_line(tri,0,d,d,0,r);
	for(int i=0;i<=d;i++) {
		tri(d,i)=tri(0,i);
		tri(i,d)=tri(i,0);
	}
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_abc(int sz,color_t col1,color_t col2, color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d,0)=col2;
	tri(0,d)=col3;
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,0,0,0,d,r);
	fill_line(tri,d,0,0,d,r);
	for(int i=0;i<=d;i++) {
		tri(d,d-i)=tri(i,0);
		tri(d-i,d)=tri(0,i);
	}
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_sasb(int sz,color_t col1,color_t col2, color_t col3,clouds_randfunc r)
{
	int d = tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(d,0)=tri(0,0)=col1;
	tri(0,d)=col2;
	tri(0,d/2)=col3;
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,0,0,0,d/2,r);
	fill_line(tri,0,d/2,0,d,r);
	fill_line(tri,d,0,0,d,r);
	for(int i=0;i<=d;i++)
		tri(d,i)=tri(0,i);
	fill_line(tri,0,d,d,d,r);
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_asbc(int sz,color_t col1,color_t col2, color_t col3,clouds_randfunc r)
{
	int d=tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d,0)=col2;
	tri(d,d)=col3;
	fill_line(tri,0,0,d,0,r);
	for(int i=0;i<=d;i++)
		tri(0,i)=tri(i,0);
	fill_line(tri,d,0,d,d,r);
	fill_line(tri,0,d,d,d,r);
	fill_line(tri,d,0,0,d,r);
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_a2sb(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d=tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d,0)=col2;
	tri(d/2,d)=col3;
	fill_line(tri,0,0,d,0,r);
	for(int i=0;i<=d;i++)
		tri(0,i)=tri(i,0);
	fill_line(tri,0,d,d/2,d,r);
	for(int i=0;i<=d/2;i++)
		tri(d-i,d)=tri(i,d);
	fill_line(tri,d,0,d,d,r);
	fill_line(tri,d,0,0,d,r);
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_axx(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d=tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(d,d)=tri(d,0)=tri(0,0)=col1;
	tri(d/2,0)=col2;
	tri(d,d/2)=col3;
	fill_line(tri,0,0,d/2,0,r);
	fill_line(tri,d/2,0,d,0,r);
	fill_line(tri,d,0,d,d/2,r);
	fill_line(tri,d,d/2,d,d,r);
	for(int i=0;i<=d;i++) {
		tri(0,i)=tri(d-i,0);
		tri(i,d)=tri(d,d-i);
	}
	fill_line(tri,d,0,0,d,r);
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_2sabc(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d=tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(d/2,d)=tri(0,0)=col1;
	tri(d,0)=col2;
	tri(0,d)=col3;
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,0,0,0,d,r);
	fill_line(tri,0,d,d/2,d,r);
	for(int i=0;i<=d/2;i++)
		tri(d-i,d)=tri(i,d);
	fill_line(tri,d,0,d,d,r);
	fill_line(tri,d,0,0,d,r);
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_abx(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d=tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d,d)=tri(d,0)=col2;
	tri(d,d/2)=col3;
	fill_line(tri,0,0,d,0,r);
	fill_line(tri,d,0,d,d/2,r);
	fill_line(tri,d,d/2,d,d,r);
	for(int i=0;i<=d;i++) {
		tri(0,i)=tri(i,0);
		tri(i,d)=tri(d,d-i);
	}
	fill_line(tri,d,0,0,d,r);
	fill_tri(tri,r,true);
	return tri;
}

canvas<color_t> quad_ab22(int sz,color_t col1,color_t col2,color_t col3,clouds_randfunc r)
{
	int d=tri_size(sz);
	canvas<color_t> tri(d+1,d+1);
	tri(0,0)=col1;
	tri(d,0)=col2;
	tri(d,d/2)=tri(d/2,d)=col3;
	fill_line(tri,0,0,d,0,r);
	for(int i=0;i<=d;i++)
		tri(0,i)=tri(i,0);
	fill_line(tri,d,0,d,d/2,r);
	fill_line(tri,0,d,d/2,d,r);
	for(int i=0;i<=d/2;i++) {
		tri(d,d-i)=tri(d,i);
		tri(d-i,d)=tri(i,d);
	}
	fill_line(tri,d,0,0,d,r);
	fill_tri(tri,r,true);
	return tri;
}

symmetric_canvas<color_t> paint_clouds(int size, symgroup sym, color_t col1, color_t col2,
	color_t col3,clouds_randfunc r)
{
	symmetric_canvas<color_t> grid(size,sym);
	canvas<color_t> tri;
	int halfsize=size/2;
	switch(sym) {
	case symgroup::CM:
		tri = tri_sax(size,col1,col2,col3,r);
		copy(grid,tri,size,0,size,size,0,0);
		break;
	case symgroup::CMM:
		tri = tri_2sab(size,col1,col2,col3,r);
		copy(grid,tri,halfsize,halfsize,0,0,size,0);
		break;
	case symgroup::P1:
		tri = quad_ao(size,col1,col2,col3,r);
		copy(grid,tri,0,0,0,size,size,0,1,false);
		copy(grid,tri,size,size,size,0,0,size,1,true);
		break;
	case symgroup::P2:
		tri = tri_a222(size,col1,col2,col3,r);
		copy(grid,tri,0,0,size,0,0,size);
		break;
	case symgroup::P3:
		tri = quad_abc((2*size+1)/3,col1,col2,col3,r);
		copy(grid,tri,0,0,size,size,2*size,-size,3,false);
		copy(grid,tri,0,0,-size,2*size,size,size,3,true);
		break;
	case symgroup::P31M:
		tri = tri_asb(size,col1,col2,col3,r);
		copy(grid,tri,size,size,3*size,0,0,3*size,3);
		break;
	case symgroup::P3M1:
		tri = tri_sabc((2*size+1)/3,col1,col2,col3,r);
		copy(grid,tri,0,0,size,size,2*size,-size,3);
		break;
	case symgroup::P4:
		tri = tri_ab2(size,col1,col2,col3,r);
		copy(grid,tri,halfsize,halfsize,size,0,0,0);
		break;
	case symgroup::P4G:
		tri = tri_asb(halfsize,col1,col2,col3,r);
		copy(grid,tri,0,0,0,halfsize,halfsize,0);
		break;
	case symgroup::P4M:
		tri = tri_sabc(halfsize,col1,col2,col3,r);
		copy(grid,tri,0,0,halfsize,0,halfsize,halfsize);
		break;
	case symgroup::P6:
		tri = tri_ab2((2*size+1)/3,col1,col2,col3,r);
		copy(grid,tri,0,0,size,size,2*size,-size,3);
		break;
	case symgroup::P6M:
		tri = tri_sabc((2*size+1)/3,col1,col2,col3,r);
		copy(grid,tri,0,0,2*size,2*size,3*size,0,6);
		break;
	case symgroup::PG:
	{
		int d = tri_size(size);
		canvas<color_t> tri(d+1,d+1);
		tri(0,0)=tri(d,0)=tri(d/2,d/2)=col1;
		tri(d/2,0)=tri(0,d/2)=col2;
		tri(3*d/4,d/4)=col3;
		fill_line(tri,d,0,3*d/4,d/4,r);
		fill_line(tri,d/2,d/2,3*d/4,d/4,r);
		for(int i=0;i<=d/2;i++)
			tri(d/2-i,d/2+i)=tri(d-i,i);
		fill_line(tri,0,0,d/2,0,r);
		fill_line(tri,d,0,d/2,0,r);
		for(int i=0;i<=d;i++) tri(0,i)=tri(d-i,0);
		fill_tri(tri,r);
		copy(grid,tri,2*size,3*size,0,-size,4*size,-size,4);
		break;
	}
	case symgroup::PGG:
		tri = tri_a2x(size,col1,col2,col3,r);
		copy(grid,tri,halfsize,0,0,-halfsize,0,halfsize);
		break;
	case symgroup::PM:
		tri = quad_sasb(size,col1,col2,col3,r);
		copy(grid,tri,0,0,0,size,halfsize,halfsize,1,false);
		copy(grid,tri,halfsize,size+halfsize,halfsize,halfsize,0,size,1,true);
		break;
	case symgroup::PMG:
		tri = tri_22sa(size,col1,col2,col3,r);
		copy(grid,tri,3*size,3*size,size,size,size,5*size,4);
		break;
	case symgroup::PMM:
		tri = quad_sabcd(halfsize,col1,col2,col3,r);
		copy(grid,tri,0,0,0,halfsize,halfsize,0,1,false);
		copy(grid,tri,halfsize,halfsize,halfsize,0,0,halfsize,1,true);
		break;
	}
	return grid;
}

canvas<color_t> paint_hyperbolic_clouds(int size, const fundamental_domain &fd,
	projtype pt,color_t col1, color_t col2, color_t col3,clouds_randfunc r)
{
	canvas<color_t> tri;
	int trisize = (int)(size*fd.polygon.sinh_radius()/fd.polygon.cosh_radius()*
		1.73205080756887729352*(pt==projtype::POINCARE?.5:1));
	auto csg = clouds_supported_groups();
	auto it = find_if(csg.cbegin(),csg.cend(),[e=fd.family->get_edges()] (const auto &v) {
		return v.size()==e.size()&&equal(e.cbegin(),e.cend(),v.cbegin()); });
	switch(it-csg.cbegin()) {
	case 0:
		tri = tri_sax(trisize,col1,col2,col3,r);
		break;
	case 1:
		tri = tri_2sab(trisize,col1,col2,col3,r);
		break;
	case 2:
		tri = tri_a222(trisize,col1,col2,col3,r);
		break;
	case 3:
		tri = tri_ab2(trisize,col1,col2,col3,r);
		break;
	case 4:
		tri = tri_asb(trisize,col1,col2,col3,r);
		break;
	case 5:
		tri = tri_sabc(trisize,col1,col2,col3,r);
		break;
	case 6:
		tri = tri_a2x(trisize,col1,col2,col3,r);
		break;
	case 7:
		tri = tri_22sa(trisize,col1,col2,col3,r);
		break;
	case 8:
		tri = quad_asbc(trisize,col1,col2,col3,r);
		break;
	case 9:
		tri = quad_sabcd(trisize,col1,col2,col3,r);
		break;
	case 10:
		tri = quad_a2sb(trisize,col1,col2,col3,r);
		break;
	case 11:
		tri = quad_sasb(trisize,col1,col2,col3,r);
		break;
	case 12:
		tri = quad_ao(trisize,col1,col2,col3,r);
		break;
	case 13:
		tri = quad_abc(trisize,col1,col2,col3,r);
		break;
	case 14:
		tri = quad_axx(trisize,col1,col2,col3,r);
		break;
	case 15:
		tri = quad_2sabc(trisize,col1,col2,col3,r);
		break;
	case 16:
		tri = quad_abx(trisize,col1,col2,col3,r);
		break;
	case 17:
		tri = quad_ab22(trisize,col1,col2,col3,r);
		break;
	default:
		throw std::logic_error("Not supported");
	};
	coord_transformer trans;
	int tsz=tri.width()-1;
	if(fd.generators.size()==3)
		trans = triangle_transformer(fd.polygon,0,0,tsz,0,0,tsz);
	else
		trans = quadrilateral_transformer(fd.polygon,0,0,tsz,0,tsz,tsz,0,tsz);
	return copy_hyperbolic(tri,pt,size,fd,trans,interpolate_closest);
}

std::vector<std::vector<fundamental_domain_family::side>> clouds_supported_groups()
{
	return {
		{{1,true},{0,true},{2,true}},
		{{0,true},{1,true},{2,false}},
		{{0,false},{1,false},{2,false}},
		{{1,false},{0,false},{2,false}},
		{{1,false},{0,false},{2,true}},
		{{0,true},{1,true},{2,true}},
		{{1,true},{0,true},{2,false}},
		{{0,false},{1,false},{2,true}},
		{{1,false},{0,false},{2,true},{3,true}},
		{{0,true},{1,true},{2,true},{3,true}},
		{{1,false},{0,false},{2,true},{3,false}},
		{{2,false},{1,true},{0,false},{3,true}},
		{{2,false},{3,false},{0,false},{1,false}},
		{{3,false},{2,false},{1,false},{0,false}},
		{{1,true},{0,true},{3,true},{2,true}},
		{{0,true},{1,true},{2,true},{3,false}},
		{{1,false},{0,false},{3,true},{2,true}},
		{{1,false},{0,false},{2,false},{3,false}},
	};
}
