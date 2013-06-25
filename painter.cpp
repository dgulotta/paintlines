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

#include "painter.h"
#include "../randgen.h"
#include <iostream>
#include <cstdlib>
#include <tuple>
#include <algorithm>

using namespace std;

inline int gcd(int x, int y) {
	int t;
	while(y) {
		t=x%y;
		x=y;
		y=t;
	}
	return x;
}

void painter_transformation::operator () (int &x, int &y) const
{
	int xnew=xx*x+xy*y+x1;
	y=yx*x+yy*y+y1;
	x=xnew;
}

const painter_transformation pt_p1_list[8] = {
	{ 1,0,0,0,1,0 },
	{ 0,1,0,1,0,0 },
	{ 0,-1,0,-1,0,0 },
	{-1,0,0,0,-1,0 },
	{-1,0,0,0,1,0 },
	{ 0,1,0,-1,0,0 },
	{ 0,-1,0,1,0,0 },
	{ 1,0,0,0,-1,0 },
};

const painter_transformation pt_ident = {1,0,0,0,1,0};
const painter_transformation pt_hflip = {-1,0,0,0,1,0};
const painter_transformation pt_vflip = {1,0,0,0,-1,0};
const painter_transformation pt_dflip = {0,1,0,1,0,0};

void (painter::*(painter::symfuncs[17]))(const function<void(int,int)> &f, int x, int y) = {
	&painter::symmetrize_cm,
	&painter::symmetrize_cmm,
	&painter::symmetrize_p1,
	&painter::symmetrize_p2,
	&painter::symmetrize_p3,
	&painter::symmetrize_p31m,
	&painter::symmetrize_p3m1,
	&painter::symmetrize_p4,
	&painter::symmetrize_p4g,
	&painter::symmetrize_p4m,
	&painter::symmetrize_p6,
	&painter::symmetrize_p6m,
	&painter::symmetrize_pg,
	&painter::symmetrize_pgg,
	&painter::symmetrize_pm,
	&painter::symmetrize_pmg,
	&painter::symmetrize_pmm,
};

void painter::symmetrize_p1(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
}

void painter::symmetrize_p2(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,size-y);
}

void painter::symmetrize_pm(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y);
}

void painter::symmetrize_cm(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,x);
}

void painter::symmetrize_pg(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(x+halfsize,size-y);
}

void painter::symmetrize_p4(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,size-x);
	f(size-x,size-y);
	f(size-y,x);
}

void painter::symmetrize_pmm(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y);
	f(x,size-y);
	f(size-x,size-y);
}

void painter::symmetrize_cmm(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,x);
	f(size-y,size-x);
	f(size-x,size-y);
}

void painter::symmetrize_pmg(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y);
	f(x+halfsize,size-y);
	f(halfsize-x,size-y);
}

void painter::symmetrize_pgg(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y+halfsize);
	f(x+halfsize,size-y);
	f(halfsize-x,halfsize-y);
}

void painter::symmetrize_p4g(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,size-x);
	f(size-x,size-y);
	f(size-y,x);
	f(halfsize-y,halfsize-x);
	f(x+halfsize,halfsize-y);
	f(y+halfsize,x+halfsize);
	f(halfsize-x,y+halfsize);
}

void painter::symmetrize_p4m(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y);
	f(x,size-y);
	f(size-x,size-y);
	f(y,x);
	f(size-y,x);
	f(y,size-x);
	f(size-y,size-x);
}

void painter::symmetrize_p3(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
}

void painter::symmetrize_p31m(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
	f(size-y,size-x);
	f(size-x,x+y);
	f(x+y,size-y);
}

void painter::symmetrize_p3m1(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
	f(y,x);
	f(x,size-x-y);
	f(size-x-y,y);
}

void painter::symmetrize_p6(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
	f(size-x,size-y);
	f(x+y,size-x);
	f(size-y,x+y);
}

void painter::symmetrize_p6m(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
	f(size-x,size-y);
	f(x+y,size-x);
	f(size-y,x+y);
	f(y,x);
	f(size-x-y,y);
	f(x,size-x-y);
	f(size-y,size-x);
	f(x+y,size-y);
	f(size-x,x+y);
}

vector<tuple<int,int>> rectangle(int xmin, int ymin, int xmax, int ymax) {
	vector<tuple<int,int>> v;
	int x,y;
	for(x=xmin;x<=xmax;x++)
		for(y=ymin;y<=ymax;y++)
			v.push_back(make_tuple(x,y));
	return v;
}

vector<tuple<int,int>> triangle(int x1, int y1, int x2, int y2, int x3, int y3, int scale=1) {
	vector<tuple<int,int>> v;
	long xmin=divide(min({x1,x2,x3}),scale), ymin=divide(min({y1,y2,y3}),scale);
	long xmax=-divide(-max({x1,x2,x3}),scale), ymax=-divide(-max({y1,y2,y3}),scale);
	long x,y;
	for(x=xmin;x<=xmax;x++)
		for(y=ymin;y<=ymax;y++) {
			long a1 = scale*x*(y2-y3)+x2*(y3-scale*y)+x3*(scale*y-y2);
			long a2 = x1*(scale*y-y3)+scale*x*(y3-y1)+x3*(y1-scale*y);
			long a3 = x1*(y2-scale*y)+x2*(scale*y-y1)+scale*x*(y1-y2);
			if(a1*a2>=0&&a1*a3>=0&&a2*a3>=0)
				v.push_back(make_tuple(x,y));
		}
	return v;
}

void painter::randomize(int xtiles, int ytiles, vector<unsigned char> &r,
		vector<unsigned char> &g, vector<unsigned char> &b)
{
	int width=xtiles*size;
	int height=ytiles*size;
	int i,j, k, l;
	int bigsize=width*height;
	r.resize(bigsize);
	g.resize(bigsize);
	b.resize(bigsize);
	auto target_pixel = [&](int x, int y) {
		int index = mod(x,width)+width*mod(y,height);
		return tie(r[index],g[index],b[index]);
	};
	auto copy = [&](const vector<tuple<int,int>> &v, const painter_transformation &from_trans, const painter_transformation &to_trans) {
		int xt, yt, xf, yf;
		for(auto &t : v) {
			tie(xt,yt)=t;
			to_trans(xt,yt);
			tie(xf,yf)=t;
			from_trans(xf,yf);
			target_pixel(xt,yt)=pixel(xf,yf);
		}

	};
	vector<tuple<int,int>> points;
	switch(sg) {
		case SYM_P1:
		case SYM_PM:
		case SYM_PG:
		case SYM_CM:
		{
			wrap_grid<int> z(xtiles*2,ytiles*2);
			for(k=0;k<2*xtiles;k++)
				for(l=0;l<2*ytiles;l++)
					if (k&l&1)
						z(k,l)=random_int(2);
					else if((k|l)&1)
						z(k,l)=0xF;
			auto fill = [&](int shift, int flip) {
				int k, l, ke, le, kc, lc, v;
				for(k=0;k<xtiles*2;k++)
					for(l=(k+1)%2;l<ytiles*2;l+=2) {
						v=random_int(2);
						ke=k;
						le=l;
						kc=k|1;
						lc=l|1;
						while(z(ke,le)&(4<<shift)) {
							z(ke,le)&=~((4|v)<<shift);
							if(z(kc,lc)^flip) {
								tie(ke,le)=make_tuple(kc+le-lc,lc+ke-kc);
								v^=1;
							}
							else {
								tie(ke,le)=make_tuple(kc-le+lc,lc-ke+kc);
							}
							tie(kc,lc)=make_tuple(2*ke-kc,2*le-lc);
						}
					}
			};
			fill(0,0);
			fill(1,1);
			points = triangle(0,0,0,size,halfsize,halfsize);
			for(k=0;k<xtiles;k++)
				for(l=0;l<ytiles;l++) {
					int el=z(2*k,2*l+1), er=z(2*k+2,2*l+1)^0x3;
					int et=z(2*k+1,2*l)^0x3, eb=z(2*k+1,2*l+2);
					int flip;
					if(el==er)
						flip=4*random_int(2);
					else
						flip=((el^et)==0x1||(el^eb)==0x2)?0:4;
					copy(points,pt_p1_list[el^flip],painter_transformation(1,0,k*size,0,1,l*size));
					copy(points,pt_p1_list[et^flip],painter_transformation(0,1,k*size,1,0,l*size));
					copy(points,pt_p1_list[er^flip],painter_transformation(-1,0,(k+1)*size,0,-1,(l+1)*size));
					copy(points,pt_p1_list[eb^flip],painter_transformation(0,-1,(k+1)*size,-1,0,(l+1)*size));
				}
		} break;
		case SYM_CMM:
		case SYM_P2:
		case SYM_PMM:
		case SYM_P4M:
		case SYM_PMG:
		case SYM_PGG:
		{
			points = triangle(0,0,size,0,halfsize,halfsize,2);
			int xt2=xtiles*2, yt2=ytiles*2, t2=xt2*yt2;
			wrap_grid<int> centers(xt2,yt2,bind(random_int,2));
			wrap_grid<int> spokes(xt2,yt2,bind(random_int,2));
			int xoff = (sg==SYM_PMG||sg==SYM_PGG)?halfsize/2:0;
			int yoff = (sg==SYM_PGG)?halfsize/2:0;
			painter_transformation from_trans[8] = {
				{1,0,xoff,0,1,yoff},
				{0,1,xoff,1,0,yoff},
				{0,1,xoff+halfsize,1,0,yoff+halfsize},
				{1,0,xoff+halfsize,0,1,yoff+halfsize},
				{1,0,xoff,0,-1,yoff},
				{0,1,xoff,-1,0,yoff},
				{0,1,xoff+halfsize,-1,0,yoff+halfsize},
				{1,0,xoff+halfsize,0,-1,yoff+halfsize}};
			for(k=0;k<xt2;k++)
				for(l=k%2;l<yt2;l+=2) {
					copy(points,from_trans[4*spokes(k+1,l+1)+2*centers(k,l)+centers(k+1,l)],
						painter_transformation(1,0,halfsize*k,0,1,halfsize*l));
					copy(points,from_trans[4*spokes(k+1,l+1)+2*centers(k,l)+centers(k,l+1)],
						painter_transformation(0,1,halfsize*k,1,0,halfsize*l));
					copy(points,from_trans[4*spokes(k,l+1)+2*centers(k,l)+centers(k,l+1)],
						painter_transformation(0,-1,halfsize*k,1,0,halfsize*l));
					copy(points,from_trans[4*spokes(k,l+1)+2*centers(k,l)+centers(k-1,l)],
						painter_transformation(-1,0,halfsize*k,0,1,halfsize*l));
					copy(points,from_trans[4*spokes(k,l)+2*centers(k,l)+centers(k-1,l)],
						painter_transformation(-1,0,halfsize*k,0,-1,halfsize*l));
					copy(points,from_trans[4*spokes(k,l)+2*centers(k,l)+centers(k,l-1)],
						painter_transformation(0,-1,halfsize*k,-1,0,halfsize*l));
					copy(points,from_trans[4*spokes(k+1,l)+2*centers(k,l)+centers(k,l-1)],
						painter_transformation(0,1,halfsize*k,-1,0,halfsize*l));
					copy(points,from_trans[4*spokes(k+1,l)+2*centers(k,l)+centers(k+1,l)],
						painter_transformation(1,0,halfsize*k,0,-1,halfsize*l));
				}
		} break;
		case SYM_P4:
		{
			points=triangle(0,0,0,size,halfsize,halfsize,2);
			wrap_grid<int> z(xtiles,ytiles,bind(random_int,0x400));
			auto from_trans = [&](bool a, bool b) {
				if(a)
					if(b)
						return painter_transformation(1,0,0,0,1,0);
					else
						return painter_transformation(0,1,0,1,0,0);
				else
					if(b)
						return painter_transformation(0,-1,halfsize,-1,0,halfsize);
					else
						return painter_transformation(-1,0,halfsize,0,-1,halfsize);
			};
			for(k=0;k<xtiles;k++)
				for(l=0;l<ytiles;l++) {
					copy(points,from_trans(z(k,l)&0x1,z(k,l)&0x4),
						painter_transformation(1,0,size*k,0,1,size*l));
					copy(points,from_trans(z(k,l)&0x1,z(k,l)&0x8),
						painter_transformation(0,1,size*k,1,0,size*l));
					copy(points,from_trans(z(k,l)&0x1,z(k,l)&0x10),
						painter_transformation(0,1,size*k,-1,0,size*l));
					copy(points,from_trans(z(k,l)&0x1,z(k,l)&0x20),
						painter_transformation(1,0,size*k,0,-1,size*l));
					copy(points,from_trans(z(k,l)&0x1,z(k,l)&0x40),
						painter_transformation(-1,0,size*k,0,-1,size*l));
					copy(points,from_trans(z(k,l)&0x1,z(k,l)&0x80),
						painter_transformation(0,-1,size*k,-1,0,size*l));
					copy(points,from_trans(z(k,l)&0x1,z(k,l)&0x100),
						painter_transformation(0,-1,size*k,1,0,size*l));
					copy(points,from_trans(z(k,l)&0x1,z(k,l)&0x200),
						painter_transformation(-1,0,size*k,0,1,size*l));
					copy(points,from_trans(z(k,l)&0x2,z(k+1,l+1)&0x80),
						painter_transformation(1,0,size*k+halfsize,0,1,size*l+halfsize));
					copy(points,from_trans(z(k,l)&0x2,z(k+1,l+1)&0x40),
						painter_transformation(0,1,size*k+halfsize,1,0,size*l+halfsize));
					copy(points,from_trans(z(k,l)&0x2,z(k+1,l)&0x200),
						painter_transformation(0,1,size*k+halfsize,-1,0,size*l+halfsize));
					copy(points,from_trans(z(k,l)&0x2,z(k+1,l)&0x100),
						painter_transformation(1,0,size*k+halfsize,0,-1,size*l+halfsize));
					copy(points,from_trans(z(k,l)&0x2,z(k,l)&0x8),
						painter_transformation(-1,0,size*k+halfsize,0,-1,size*l+halfsize));
					copy(points,from_trans(z(k,l)&0x2,z(k,l)&0x4),
						painter_transformation(0,-1,size*k+halfsize,-1,0,size*l+halfsize));
					copy(points,from_trans(z(k,l)&0x2,z(k,l+1)&0x20),
						painter_transformation(0,-1,size*k+halfsize,1,0,size*l+halfsize));
					copy(points,from_trans(z(k,l)&0x2,z(k,l+1)&0x10),
						painter_transformation(-1,0,size*k+halfsize,0,1,size*l+halfsize));
				}
		} break;
		case SYM_P4G:
		{
			points=triangle(0,0,halfsize,0,halfsize,halfsize);
			auto random_trans = [&]() { return random_bool()?pt_dflip:pt_ident; };
			for(k=0;k<xtiles;k++)
				for(l=0;l<ytiles;l++) {
					copy(points,random_trans(),painter_transformation(1,0,k*size,0,1,l*size));
					copy(points,random_trans(),painter_transformation(0,1,k*size,1,0,l*size));
					copy(points,random_trans(),painter_transformation(-1,0,k*size,0,1,l*size));
					copy(points,random_trans(),painter_transformation(0,-1,k*size,1,0,l*size));
					copy(points,random_trans(),painter_transformation(1,0,k*size,0,-1,l*size));
					copy(points,random_trans(),painter_transformation(0,1,k*size,-1,0,l*size));
					copy(points,random_trans(),painter_transformation(-1,0,k*size,0,-1,l*size));
					copy(points,random_trans(),painter_transformation(0,-1,k*size,-1,0,l*size));
				}
		} break; 
		case SYM_P3:
		case SYM_P31M:
		case SYM_P6:
			/* I think the other randomizer is more interesting.
			 * Maybe eventually multiple randomizers per symmetry group
			 * will be supported.
			 for(k=0;k<xtiles;k++)
			 for(l=0;l<ytiles;l++) {
			 int z=rand();
			 for(i=0;i*3<size;i++) {
			 for(j=i;j<(size-i)/2;j++) {
			 int index=(z&4)?(i+size*j):(j+size*i);
			 int index2=((k+1)%xtiles)*size+i+width*(l*size+j);
			 PAINTER_COPY_RGB;
			 index=(z&8)?(i+size*j):(j+size*i);
			 index2=k*size+j+width*(((l+1)%ytiles)*size+i);
			 PAINTER_COPY_RGB;
			 }
			 for(;j<size-2*i;j++) {
			 int index=(z&16)?(i+size*j):(j+size*i);
			 int index2=((k+1)%xtiles)*size+i+width*(l*size+j);
			 PAINTER_COPY_RGB;
			 index=(z&32)?(i+size*j):(j+size*i);
			 index2=k*size+j+width*(((l+1)%ytiles)*size+i);
			 PAINTER_COPY_RGB;
			 }
			 for(;j<size-i/2;j++) {
			 int index=(z&1)?(i+size*j):(j+size*i);
			 int index2=k*size+i+width*(l*size+j);
			 PAINTER_COPY_RGB;
			 index=(z&2)?(i+size*j):(j+size*i);
			 index2=k*size+j+width*(l*size+i);
			 PAINTER_COPY_RGB; 
			 }
			 }
			 for(;i*3<2*size;i++)
			 for(j=i;j<size-i/2;j++) {
			 int index=(z&1)?(i+size*j):(j+size*i);
			 int index2=k*size+i+width*(l*size+j);
			 PAINTER_COPY_RGB;
			 index=(z&2)?(i+size*j):(j+size*i);
			 index2=k*size+j+width*(l*size+i);
			 PAINTER_COPY_RGB; 
			 }
			 for(;i<size;i++) {
			 for(j=2*(size-i);j<size-i/2;j++) {
			 int index=(z&4)?(i+size*j):(j+size*i);
			 int index2=k*size+i+width*(l*size+j);
			 PAINTER_COPY_RGB;
			 index=(z&8)?(i+size*j):(j+size*i);
			 index2=k*size+j+width*(l*size+i);
			 PAINTER_COPY_RGB; 
			 }
			 for(;j<=i;j++) {
			 int index=(z&16)?(i+size*j):(j+size*i);
			 int index2=k*size+i+width*(l*size+j);
			 PAINTER_COPY_RGB;
			 index=(z&32)?(i+size*j):(j+size*i);
			 index2=k*size+j+width*(l*size+i);
			 PAINTER_COPY_RGB;
			 }
			 }
			 }
			 break;
			 */
		case SYM_P3M1:
		case SYM_P6M:
		{
			points = triangle(0,0,0,size+2,size+2,0,3);
			auto gen = bind(random_int,3);
			wrap_grid<int> types0(xtiles,ytiles,gen);
			wrap_grid<int> types1(xtiles,ytiles,gen);
			wrap_grid<int> types2(xtiles,ytiles,gen);
			auto from_trans = [&](int mt,int nt, int z) {
				if(mt==0)
					if((nt==0&&z)||nt==1)
						return painter_transformation(1,0,0,0,1,0);
					else
						return painter_transformation(0,-1,0,-1,0,0);
				else if(mt==1)
					if((nt==1&&z)||nt==2)
						return painter_transformation(1,0,(size+1)/3,0,1,(size+1)/3);
					else
						return painter_transformation(0,-1,(size+1)/3,-1,0,(size+1)/3);
				else
					if((nt==2&&z)||nt==0)
						return painter_transformation(1,0,-((size+1)/3),0,1,-((size+1)/3));
					else
						return painter_transformation(0,-1,-((size+1)/3),-1,0,-((size+1)/3));
			};
			for(k=0;k<xtiles;k++)
				for(l=0;l<ytiles;l++) {
					int z=random_int(0x200);
					copy(points,from_trans(types0(k,l),types1(k,l),z&0x1),
						painter_transformation(1,0,size*k,0,1,size*l));
					copy(points,from_trans(types1(k,l),types0(k,l),z&0x1),
						painter_transformation(0,-1,size*k+(size+1)/3,-1,0,size*l+(size+1)/3));
					copy(points,from_trans(types0(k,l),types2(k-1,l),z&0x2),
						painter_transformation(-1,0,size*k,1,1,size*l));
					copy(points,from_trans(types2(k-1,l),types0(k,l),z&0x2),
						painter_transformation(0,1,size*k-(size+1)/3,-1,-1,size*l+(2*size+1)/3));
					copy(points,from_trans(types0(k,l),types1(k-1,l),z&0x4),
						painter_transformation(-1,-1,size*k,1,0,size*l));
					copy(points,from_trans(types1(k-1,l),types0(k,l),z&0x4),
						painter_transformation(1,1,size*k-(2*size+1)/3,0,-1,size*l+(size+1)/3));
					copy(points,from_trans(types0(k,l),types2(k-1,l-1),z&0x8),
						painter_transformation(0,-1,size*k,-1,0,size*l));
					copy(points,from_trans(types2(k-1,l-1),types0(k,l),z&0x8),
						painter_transformation(1,0,size*k-(size+1)/3,0,1,size*l-(size+1)/3));
					copy(points,from_trans(types0(k,l),types1(k,l-1),z&0x10),
						painter_transformation(0,1,size*k,-1,-1,size*l));
					copy(points,from_trans(types1(k,l-1),types0(k,l),z&0x10),
						painter_transformation(-1,0,size*k+(size+1)/3,1,1,size*l-(2*size+1)/3));
					copy(points,from_trans(types0(k,l),types2(k,l-1),z&0x20),
						painter_transformation(1,1,size*k,0,-1,size*l));
					copy(points,from_trans(types2(k,l-1),types0(k,l),z&0x20),
						painter_transformation(-1,-1,size*k+(2*size+1)/3,1,0,size*l-(size+1)/3));
					copy(points,from_trans(types1(k,l),types2(k,l),z&0x40),
						painter_transformation(1,0,size*k+(size+1)/3,0,1,size*l+(size+1)/3));
					copy(points,from_trans(types2(k,l),types1(k,l),z&0x40),
						painter_transformation(0,-1,size*k+(2*size+1)/3,-1,0,size*l+(2*size+1)/3));
					copy(points,from_trans(types1(k-1,l),types2(k-2,l),z&0x80),
						painter_transformation(-1,-1,size*k-(2*size+1)/3,1,0,size*l+(size+1)/3));
					copy(points,from_trans(types2(k-2,l),types1(k-1,l),z&0x80),
						painter_transformation(1,1,size*k-(4*size+1)/3,0,-1,size*l+(2*size+1)/3));
					copy(points,from_trans(types1(k,l-1),types2(k,l-2),z&0x100),
						painter_transformation(0,1,size*k+(size+1)/3,-1,-1,size*l-(2*size+1)/3));
					copy(points,from_trans(types2(k,l-2),types1(k,l-1),z&0x100),
						painter_transformation(-1,0,size*k+(2*size+1)/3,1,1,size*l-(4*size+1)/3));
					}
		}
	}
}
