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
 
#include "randgen.hpp"
#include "randomize.hpp"
#include <tuple>

using namespace std;
using namespace std::placeholders;

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

const painter_transformation pt_pg_list[8] = {
	{ 1, 0, 0, 0, 1, 0},
	{-1, 0, 0, 0, 1, 0},
	{ 1, 0, 0, 0,-1, 0},
	{-1, 0, 0, 0,-1, 0},
	{ 0,-1, 0, 1, 0, 0},
	{ 0, 1, 0, 1, 0, 0},
	{ 0,-1, 0,-1, 0, 0},
	{ 0, 1, 0,-1, 0, 0},
};

const painter_transformation pt_ident = {1,0,0,0,1,0};
const painter_transformation pt_hflip = {-1,0,0,0,1,0};
const painter_transformation pt_vflip = {1,0,0,0,-1,0};
const painter_transformation pt_dflip = {0,1,0,1,0,0};

vector<tuple<int,int>> rectangle(int xmin, int ymin, int xmax, int ymax) {
	vector<tuple<int,int>> v;
	for(int x=xmin;x<=xmax;x++)
		for(int y=ymin;y<=ymax;y++)
			v.emplace_back(x,y);
	return v;
}

vector<tuple<int,int>> triangle(int x1, int y1, int x2, int y2, int x3, int y3, int scale=1) {
	vector<tuple<int,int>> v;
	long long xmin=divide(min({x1,x2,x3}),scale), ymin=divide(min({y1,y2,y3}),scale);
	long long xmax=-divide(-max({x1,x2,x3}),scale), ymax=-divide(-max({y1,y2,y3}),scale);
	for(long long x=xmin;x<=xmax;x++)
		for(long long y=ymin;y<=ymax;y++) {
			long long a1 = scale*x*(y2-y3)+x2*(y3-scale*y)+x3*(scale*y-y2);
			long long a2 = x1*(scale*y-y3)+scale*x*(y3-y1)+x3*(y1-scale*y);
			long long a3 = x1*(y2-scale*y)+x2*(scale*y-y1)+scale*x*(y1-y2);
			if(a1*a2>=0&&a1*a3>=0&&a2*a3>=0)
				v.emplace_back(x,y);
		}
	return v;
}

void fill(int ke, int le, int kc, int lc, const function<bool(int,int)> &mark, const function<tuple<int,int>(int,int,int,int)> &mov, const function<void(int,int,int,int)> &put)
{
	while(mark(ke,le)) {
		put(ke,le,kc,lc);
		tie(ke,le)=mov(ke,le,kc,lc);
		tie(kc,lc)=make_tuple(2*ke-kc,2*le-lc);
	}
}

int dot_prod(tuple<int,int> t1, int x1, int x2)
{
	return get<0>(t1)*x1+get<1>(t1)*x2;
}

tuple<int,int> p1_move1(int ke, int le, int kc, int lc)
{
	return make_tuple(kc-lc+le,lc-kc+ke);
}

tuple<int,int> p1_move2(int ke, int le, int kc, int lc)
{
	return make_tuple(kc+lc-le,lc+kc-ke);
}

void randomize_p1(int xtiles, int ytiles, int size, const copy_func_t &copy)
{
	int k, l;
	int halfsize=size/2;
	wrap_canvas<int> z(xtiles*2,ytiles*2);
	wrap_canvas<tuple<int,int>> c1(xtiles*2,ytiles*2), c2(xtiles*2,ytiles*2);
	for(k=0;k<2*xtiles;k++)
		for(l=0;l<2*ytiles;l++)
			if (k&l&1)
				z(k,l)=random_int(2);
			else if((k|l)&1)
				z(k,l)=0xC;
	auto mark = [&z] (int b, int i, int j) { bool a = z(i,j)&b; z(i,j)&=~b; return a; };
	auto mark1 = bind(mark,0x4,_1,_2), mark2 = bind(mark,0x8,_1,_2);
	auto mov1 = [&z] (int ke, int le, int kc, int lc) {
		return (z(kc,lc)?p1_move1:p1_move2)(ke,le,kc,lc);
	};
	auto mov2 = [&z] (int ke, int le, int kc, int lc) {
		return (z(kc,lc)?p1_move2:p1_move1)(ke,le,kc,lc);
	};
	auto put = [] (wrap_canvas<tuple<int,int>> &c, int ke, int le, int kc, int lc) {
		c(ke,le)=make_tuple(ke-kc,le-lc);
	};
	auto put1 = bind(put,ref(c1),_1,_2,_3,_4), put2 = bind(put,ref(c2),_1,_2,_3,_4);
	for(k=0;k<xtiles;k++)
		for(l=0;l<ytiles;l++) {
			fill(2*k,2*l+1,2*k+random_sign(),2*l+1,mark1,mov1,put1);
			fill(2*k,2*l+1,2*k+random_sign(),2*l+1,mark2,mov2,put2);
			fill(2*k+1,2*l,2*k+1,2*l+random_sign(),mark1,mov1,put1);
			fill(2*k+1,2*l,2*k+1,2*l+random_sign(),mark2,mov2,put2);
		}
	auto points = triangle(0,0,0,size,halfsize,halfsize);
	auto rel = [&] (int i, int j, int n1, int n2) {
		return 2*(dot_prod(c1(i,j),n1,n2)>0)+(dot_prod(c2(i,j),n1,n2)>0);
	};
	for(k=0;k<xtiles;k++)
		for(l=0;l<ytiles;l++) {
			int el=rel(2*k,2*l+1,1,0), er=rel(2*k+2,2*l+1,-1,0);
			int et=rel(2*k+1,2*l,0,-1), eb=rel(2*k+1,2*l+2,0,1);
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

tuple<int,int> pg_move1(int ke, int le, int kc, int)
{
	return make_tuple(2*kc-ke,le);
}

tuple<int,int> pg_move2(int ke, int le, int, int lc)
{
	return make_tuple(ke,2*lc-le);
}

void randomize_pg(int xtiles, int ytiles, int size, const copy_func_t &copy)
{
	int k, l;
	int halfsize=size/2;
	int v;
	wrap_canvas<int> z(xtiles*4,ytiles*4);
	wrap_canvas<tuple<int,int>> c1(xtiles*4,ytiles*4);
	wrap_canvas<int> c2(xtiles*4,ytiles*4);
	for(k=0;k<4*xtiles;k++)
		for(l=0;l<4*ytiles;l++) {
			if(((k+l)&3)==0&&((k-l)&3)==0) {
				z(k,l)=random_int(2);
			}
			else if(k&l&1)
				z(k,l)=0xC;
		}
	auto mark = [&z] (int b, int i, int j) { bool a = z(i,j)&b; z(i,j)&=~b; return a; };
	auto mark1 = bind(mark,0x4,_1,_2), mark2 = bind(mark,0x8,_1,_2);
	auto mov1 = [&z] (int ke, int le, int kc, int lc) {
		return (z(kc,lc)?pg_move1:pg_move2)(ke,le,kc,lc);
	};
	auto mov2 = [&z] (int ke, int le, int kc, int lc) {
		return (z(kc,lc)?pg_move2:pg_move1)(ke,le,kc,lc);
	};
	auto put1 = [&c1] (int ke, int le, int kc, int lc) {
		c1(ke,le)=make_tuple(ke-kc,le-lc);
	};
	auto put2 = [&c2,&v] (int ke, int le, int, int) {
		c2(ke,le)=v;
	};
	for(k=1;k<4*xtiles;k+=2)
		for(l=1;l<4*ytiles;l+=2) {
			v = random_sign();
			int kc = k+v*(2-(k&3));
			int lc = l+v*(2-(l&3));
			v = random_int(2);
			fill(k,l,kc,lc,mark1,mov1,put1);
			fill(k,l,kc,lc,mark2,mov2,put2);
		}
	auto points = triangle(0,0,0,halfsize,halfsize,0);
	auto rel = [&] (int i, int j, int n1, int n2) {
		return (dot_prod(c1(i,j),n1,n2)>0)+2*c2(i,j);
	};
	for(k=0;k<2*xtiles;k+=1)
		for(l=k&1;l<2*ytiles;l+=2) {
			int ey=rel(2*k-1,2*l-1,1,1), eu=rel(2*k+1,2*l-1,1,-1);
			int eb=rel(2*k-1,2*l+1,-1,1), en=rel(2*k+1,2*l+1,-1,-1);
			int flip;
			if(ey==en)
				flip=4*random_int(2);
			else
				flip=((ey^eu)==0x1||(ey^eb)==0x2)?0:4;
			copy(points,pt_pg_list[en^flip],painter_transformation(1,0,k*halfsize,0,1,l*halfsize));
			copy(points,pt_pg_list[eb^flip],painter_transformation(-1,0,k*halfsize,0,1,l*halfsize));
			copy(points,pt_pg_list[eu^flip],painter_transformation(1,0,k*halfsize,0,-1,l*halfsize));
			copy(points,pt_pg_list[ey^flip],painter_transformation(-1,0,k*halfsize,0,-1,l*halfsize));
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
	case symgroup::CM:
	case symgroup::P1:
	case symgroup::PM:
		randomize_p1(xtiles,ytiles,size,copy_func);
		break;
	case symgroup::PG:
		randomize_pg(xtiles,ytiles,size,copy_func);
		break;
	case symgroup::CMM:
	case symgroup::P2:
	case symgroup::PGG:
	case symgroup::PMG:
	case symgroup::PMM:
		randomize_p2(xtiles,ytiles,size,copy_func);
		break;
	case symgroup::P3:
	case symgroup::P3M1:
	case symgroup::P31M:
	case symgroup::P6:
	case symgroup::P6M:
		randomize_p3(xtiles,ytiles,size,copy_func);
		break;
	case symgroup::P4:
	case symgroup::P4G:
	case symgroup::P4M:
		randomize_p4(xtiles,ytiles,size,copy_func);
	}
}
