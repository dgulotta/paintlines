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
 
#include "randgen.h"
#include "randomize.h"
#include <tuple>

using namespace std;

struct painter_transformation {
	painter_transformation() {}
	painter_transformation(int _xx, int _xy, int _x1, int _yx, int _yy, int _y1)
		: xx(_xx), xy(_xy), x1(_x1), yx(_yx), yy(_yy), y1(_y1) {}
	void operator () (int &x, int &y) const {
		int xnew=xx*x+xy*y+x1;
		y=yx*x+yy*y+y1;
		x=xnew;
	}
	int xx;
	int xy;
	int x1;
	int yx;
	int yy;
	int y1;
};

typedef function<void(const vector<tuple<int,int>> &,const painter_transformation &,const painter_transformation &)> copy_func_t; 

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

inline int divide(int x, int y)
{
	return (x>0)?(x/y):(-(-x)/y);
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

void randomize_p1(int xtiles, int ytiles, int size, const copy_func_t &copy)
{
	int k, l;
	int halfsize=size/2;
	wrap_canvas<int> z(xtiles*2,ytiles*2);
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
	auto points = triangle(0,0,0,size,halfsize,halfsize);
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
}

void randomize_p2(int xtiles, int ytiles, int size, const copy_func_t &copy)
{
	int k,l;
	int halfsize=size/2;
	auto points = triangle(0,0,size,0,halfsize,halfsize,2);
	int xt2=xtiles*2, yt2=ytiles*2;
	wrap_canvas<int> centers(xt2,yt2,bind(random_int,2));
	wrap_canvas<int> spokes(xt2,yt2,bind(random_int,2));
	painter_transformation from_trans[8] = {
		{1,0,0,0,1,0},
		{0,1,0,1,0,0},
		{0,1,halfsize,1,0,halfsize},
		{1,0,halfsize,0,1,halfsize},
		{1,0,0,0,-1,0},
		{0,1,0,-1,0,0},
		{0,1,halfsize,-1,0,halfsize},
		{1,0,halfsize,0,-1,halfsize}};
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
}

void randomize_p3(int xtiles, int ytiles, int size, const copy_func_t &copy)
{
	int k,l;
	auto points = triangle(0,0,0,size+2,size+2,0,3);
	auto gen = bind(random_int,3);
	wrap_canvas<int> types0(xtiles,ytiles,gen);
	wrap_canvas<int> types1(xtiles,ytiles,gen);
	wrap_canvas<int> types2(xtiles,ytiles,gen);
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

void randomize_p4(int xtiles, int ytiles, int size, const copy_func_t &copy)
{
	int k,l;
	int halfsize = size/2;
	auto points=triangle(0,0,0,size,halfsize,halfsize,2);
	wrap_canvas<int> z(xtiles,ytiles,bind(random_int,0x400));
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
}

void randomize(int xtiles, int ytiles, symgroup sg, int size, const function<void(int,int,int,int)> &f)
{
	copy_func_t copy_func = [&](const vector<tuple<int,int>> &v,const painter_transformation &from_trans,const painter_transformation &to_trans) {
		int xt, yt, xf, yf;
		for(auto &t : v) {
			tie(xt,yt)=t;
			to_trans(xt,yt);
			tie(xf,yf)=t;
			from_trans(xf,yf);
			f(xf,yf,xt,yt);
		}
	};
	switch(sg) {
	case SYM_CM:
	case SYM_P1:
	case SYM_PG:
	case SYM_PM:
		randomize_p1(xtiles,ytiles,size,copy_func);
		break;
	case SYM_CMM:
	case SYM_P2:
	case SYM_PGG:
	case SYM_PMG:
	case SYM_PMM:
		randomize_p2(xtiles,ytiles,size,copy_func);
		break;
	case SYM_P3:
	case SYM_P3M1:
	case SYM_P31M:
	case SYM_P6:
	case SYM_P6M:
		randomize_p3(xtiles,ytiles,size,copy_func);
		break;
	case SYM_P4:
	case SYM_P4G:
	case SYM_P4M:
		randomize_p4(xtiles,ytiles,size,copy_func);
	}
}
