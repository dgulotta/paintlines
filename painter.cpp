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

#define PAINTER_COPY_RGB r[index2]=red[index];g[index2]=green[index];\
								   b[index2]=blue[index];

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

const painter_transformation pt_cmm_list[4] = 
{ { 1,0,0,0,1,0 }, { 0,1,0,1,0,0 }, { 0,-1,-1,-1,0,-1 }, {-1,0,-1,0,-1,-1}};

const painter_transformation pt_ident = {1,0,0,0,1,0};
const painter_transformation pt_hflip = {-1,0,-1,0,1,0};
const painter_transformation pt_vflip = {1,0,1,0,-1,-1};

void painter_transform::set_point(int x, int y) {
	int xnew=x, ynew=y;
	from(xnew,ynew);
	from_index=mod(xnew,size)+size*mod(ynew,size);
	xnew=x;
	ynew=y;
	to(xnew,ynew);
	to_index=mod(xnew,width)+width*mod(ynew,height);
}

void (painter::*(painter::symfuncs[36]))(const function<void(int,int)> &f, int x, int y) = {
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
	&painter::symmetrize_cm_o,
	&painter::symmetrize_cmm_o,
	&painter::symmetrize_p1_o,
	&painter::symmetrize_p2_o,
	&painter::symmetrize_p3_o,
	&painter::symmetrize_p31m_o,
	&painter::symmetrize_p3m1_o,
	&painter::symmetrize_p4_o,
	&painter::symmetrize_p4g_o,
	&painter::symmetrize_p4m_o,
	&painter::symmetrize_p6_o,
	&painter::symmetrize_p6m_o,
	&painter::symmetrize_pg_o,
	&painter::symmetrize_pgg_o,
	&painter::symmetrize_pm_o,
	&painter::symmetrize_pmg_o,
	&painter::symmetrize_pmm_o,
	&painter::symmetrize_cm_2,
	&painter::symmetrize_p4m_2
};

void painter::symmetrize_p1(const function<void(int,int)> &f, int x, int y)
{
	f(x,y);
}

void painter::symmetrize_p2(const function<void(int,int)> &f, int x, int y)
{
	f(x,y);
	f(size1-x,size1-y);
}

void painter::symmetrize_pm(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x,y);
}

void painter::symmetrize_cm(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,x);
}

void painter::symmetrize_pg(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(x+halfsize,size1-y);
}

void painter::symmetrize_p4(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,size1-x);
	f(size1-x,size1-y);
	f(size1-y,x);
}

void painter::symmetrize_pmm(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x,y);
	f(x,size1-y);
	f(size1-x,size1-y);
}

void painter::symmetrize_cmm(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,x);
	f(size1-y,size1-x);
	f(size1-x,size1-y);
}

void painter::symmetrize_pmg(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x,y);
	f(x+halfsize,size1-y);
	f(halfsize1-x,size1-y);
}

void painter::symmetrize_pgg(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x,y+halfsize);
	f(x+halfsize,size1-y);
	f(halfsize1-x,halfsize1-y);
}

void painter::symmetrize_p4g(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,size1-x);
	f(size1-x,size1-y);
	f(size1-y,x);
	f(halfsize1-y,halfsize1-x);
	f(x+halfsize,halfsize1-y);
	f(y+halfsize,x+halfsize);
	f(halfsize1-x,y+halfsize);
}

void painter::symmetrize_p4m(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x,y);
	f(x,size1-y);
	f(size1-x,size1-y);
	f(y,x);
	f(size1-y,x);
	f(y,size1-x);
	f(size1-y,size1-x);
}

void painter::symmetrize_p3(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x-y,x);
	f(y,size1-x-y);
}

void painter::symmetrize_p31m(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x-y,x);
	f(y,size1-x-y);
	f(size1-y,size1-x);
	f(size1-x,x+y);
	f(x+y,size1-y);
}

void painter::symmetrize_p3m1(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x-y,x);
	f(y,size1-x-y);
	f(y,x);
	f(x,size1-x-y);
	f(size1-x-y,y);
}

void painter::symmetrize_p6(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x-y,x);
	f(y,size1-x-y);
	f(size1-x,size1-y);
	f(x+y,size1-x);
	f(size1-y,x+y);
}

void painter::symmetrize_p6m(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x-y,x);
	f(y,size1-x-y);
	f(size1-x,size1-y);
	f(x+y,size1-x);
	f(size1-y,x+y);
	f(y,x);
	f(size1-x-y,y);
	f(x,size1-x-y);
	f(size1-y,size1-x);
	f(x+y,size1-y);
	f(size1-x,x+y);
}

void painter::symmetrize_p1_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
}

void painter::symmetrize_p2_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,size-y);
}

void painter::symmetrize_pm_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y);
}

void painter::symmetrize_cm_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,x);
}

void painter::symmetrize_pg_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(x+halfsize,size-y);
}

void painter::symmetrize_p4_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,size-x);
	f(size-x,size-y);
	f(size-y,x);
}

void painter::symmetrize_pmm_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y);
	f(x,size-y);
	f(size-x,size-y);
}

void painter::symmetrize_cmm_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,x);
	f(size-y,size-x);
	f(size-x,size-y);
}

void painter::symmetrize_pmg_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y);
	f(x+halfsize,size-y);
	f(halfsize-x,size-y);
}

void painter::symmetrize_pgg_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y+halfsize);
	f(x+halfsize,size-y);
	f(halfsize-x,halfsize-y);
}

void painter::symmetrize_p4g_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(y,size-x);
	f(size-x,size-y);
	f(size-y,x);
	f(halfsize-y,halfsize-x);
	f(x+halfsize,halfsize-y);
	f(y+halfsize,x+halfsize);
	f(halfsize-x,y+halfsize);
}

void painter::symmetrize_p4m_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x,y);
	f(x,size-y);
	f(size-x,size-y);
	f(y,x);
	f(size-y,x);
	f(y,size-x);
	f(size-y,size-x);
}

void painter::symmetrize_p3_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
}

void painter::symmetrize_p31m_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
	f(size-y,size-x);
	f(size-x,x+y);
	f(x+y,size-y);
}

void painter::symmetrize_p3m1_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
	f(y,x);
	f(x,size-x-y);
	f(size-x-y,y);
}

void painter::symmetrize_p6_o(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size-x-y,x);
	f(y,size-x-y);
	f(size-x,size-y);
	f(x+y,size-x);
	f(size-y,x+y);
}

void painter::symmetrize_p6m_o(const function<void(int,int)> &f, int x, int y) {
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

void painter::symmetrize_cm_2(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-y,size1-x);
}

void painter::symmetrize_p4m_2(const function<void(int,int)> &f, int x, int y) {
	f(x,y);
	f(size1-x,y);
	f(x,size1-y);
	f(size1-x,size1-y);
	f(halfsize1-y,halfsize1-x);
	f(y+halfsize,halfsize1-x);
	f(halfsize1-y,x+halfsize);
	f(y+halfsize,x+halfsize);
}

vector<tuple<int,int>> rectangle(int xmin, int ymin, int xmax, int ymax) {
	vector<tuple<int,int>> v;
	int x,y;
	for(x=xmin;x<=xmax;x++)
		for(y=ymin;y<=ymax;y++)
			v.push_back(make_tuple(x,y));
	return v;
}

vector<tuple<int,int>> triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	vector<tuple<int,int>> v;
	int xmin=min({x1,x2,x3}), ymin=min({y1,y2,y3});
	int xmax=max({x1,x2,x3}), ymax=max({y1,y2,y3});
	int x,y;
	for(x=xmin;x<=xmax;x++)
		for(y=ymin;y<=ymax;y++) {
			int a = x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2);
			int a1 = x*(y2-y3)+x2*(y3-y)+x3*(y-y2);
			int a2 = x1*(y-y3)+x*(y3-y1)+x3*(y1-y);
			int a3 = x1*(y2-y)+x2*(y-y1)+x*(y1-y2);
			if(a1*a2>=0&&a1*a3>=0&&a2*a3>=0)
				v.push_back(make_tuple(x,y));
		}
	return v;

}

/* This function is approaching a near-IOCCC level of unreadability.
 * I really should clean it up someday.
 */
void painter::randomize(int xtiles, int ytiles, vector<unsigned char> &r,
		vector<unsigned char> &g, vector<unsigned char> &b)
{
	int width=xtiles*size;
	int height=ytiles*size;
	int i,j, k, l;
	int qsize=halfsize/2, sizeq1=size1+halfsize-qsize, sizesq=size*size;
	int bigsize=width*height;
	r.resize(bigsize);
	g.resize(bigsize);
	b.resize(bigsize);
	painter_transform pt(size,xtiles,ytiles);
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
	auto PAINTER_NEW_COPY_RGB = [&]() {
		pt.copy(r,red);
		pt.copy(g,green);
		pt.copy(b,blue);
	};
	auto PAINTER_RANDOMIZE_P2_LOOP = [&]() {
		for(i=0;i<=qsize;i++)
			for(j=-i;j<=i;j++) {
				pt.set_point(i,j);
				PAINTER_NEW_COPY_RGB();
			}
	};
	vector<tuple<int,int>> points;
	symgroup sg_basic = (symgroup)((int)sg%17);
	switch(sg_basic) {
		case SYM_P1:
		case SYM_PM:
		case SYM_PG:
		case SYM_CM:
			{
				vector<int> ev(xtiles*ytiles,0xF), eh(xtiles*ytiles,0xF), *es;
				int z;
				bool back=true;
				for(l=0;l<ytiles;l++) {
					for(k=0;k<xtiles;k++) {
						if(rand()&1) {
							ev[k+xtiles*l]|=0x10;
							eh[k+xtiles*l]|=0x10;
							ev[(k+1)%xtiles+xtiles*l]|=0x20;
							eh[k%xtiles+xtiles*((l+1)%ytiles)]|=0x20;
						}
					}
				}
				k=0;
				l=0;
				es=&ev;
				while(l<ytiles) {
					z=(~0x4)&(rand()|~0x1);
					do {
						(*es)[k+xtiles*l]&=z;
						if(es==&ev) {
							if(back) {
								if(ev[k+xtiles*l]&0x20) {
									back=true;
								}
								else {
									l=(l+1)%ytiles;
									back=false;
									z^=0x1;
								}
								k=mod(k-1,xtiles);
							}
							else {
								if(ev[k+xtiles*l]&0x10) {
									l=(l+1)%ytiles;
									back=false;
								}
								else {
									back=true;
									z^=0x1;
								}
							}
							es=&eh;
						}
						else {
							if(back) {
								if(eh[k+xtiles*l]&0x20) {
									back=true;
								}
								else {
									k=(k+1)%xtiles;
									back=false;
									z^=0x1;
								}
								l=mod(l-1,ytiles);
							}
							else {
								if(eh[k+xtiles*l]&0x10) {
									k=(k+1)%xtiles;
									back=false;
								}
								else {
									back=true;
									z^=0x1;
								}
							}
							es=&ev;
						}
					} while((*es)[k+xtiles*l]&0x4);
					while(!((*es)[k+xtiles*l]&0x4)) {
						k++;
						if(k>=xtiles) {
							k=0;
							l++;
						}
						if(l>=ytiles) {
							if(es==&ev) {
								l=0;
								es=&eh;
							}
							else break;
						}
					}
				}
				k=0;
				l=0;
				es=&ev;
				int *oe, oz;
				while(l<ytiles) {
					z=(~0x38)&(rand()|~0x2);
					do {
						oe=&((*es)[k+xtiles*l]);
						oz=z;
						if(es==&ev) {
							if(back) {
								if(ev[k+xtiles*l]&0x20) {
									l=(l+1)%ytiles;
									back=false;
									z^=0x2;
								}
								else {
									back=true;
								}
								k=mod(k-1,xtiles);
							}
							else {
								if(ev[k+xtiles*l]&0x10) {
									back=true;
									z^=0x2;
								}
								else {
									l=(l+1)%ytiles;
									back=false;
								}
							}
							es=&eh;
						}
						else {
							if(back) {
								if(eh[k+xtiles*l]&0x20) {
									k=(k+1)%xtiles;
									back=false;
									z^=0x2;
								}
								else {
									back=true;
								}
								l=mod(l-1,ytiles);
							}
							else {
								if(eh[k+xtiles*l]&0x10) {
									back=true;
									z^=0x2;
								}
								else {
									k=(k+1)%xtiles;
									back=false;
								}
							}
							es=&ev;
						}
						(*oe)&=oz;
					} while((*es)[k+xtiles*l]&0x8);
					while(!((*es)[k+xtiles*l]&0x8)) {
						k++;
						if(k>=xtiles) {
							k=0;
							l++;
						}
						if(l>=ytiles) {
							if(es==&ev) {
								l=0;
								es=&eh;
							}
							else break;
						}
					}
				}
				int newi, newj;
				for(k=0;k<xtiles;k++) {
					for(l=0;l<ytiles;l++) {
						int el=ev[k+l*xtiles], er=ev[(k+1)%xtiles+l*xtiles]^0x3;
						int et=eh[k+l*xtiles]^0x3, eb=eh[k+((l+1)%ytiles)*xtiles];
						if(el==er) {
							const painter_transformation &flip=(rand()&1?pt_ident:pt_hflip);
							for(i=0;i<halfsize;i++) {
								for(j=i;i+j<size;j++) {
									newi=i;
									newj=j;
									flip(newi,newj);
									pt_cmm_list[el](newi,newj);
									int index=mod(newi,size)+size*mod(newj,size);
									int index2=i+k*size+width*(j+l*size);
									PAINTER_COPY_RGB;
									index2=j+k*size+width*(i+l*size);
									PAINTER_COPY_RGB;
									index2=(size1-i)+k*size+width*(size1-j+l*size);
									PAINTER_COPY_RGB;
									index2=(size1-j)+k*size+width*(size1-i+l*size);
									PAINTER_COPY_RGB;
								}
							}
						}
						else {
							const painter_transformation &flip=(((el^et)==0x1||(el^eb)==0x2)?pt_ident:pt_hflip);
							for(i=0;i<halfsize;i++) {
								for(j=i;i+j<size;j++) {
									newi=i;
									newj=j;
									flip(newi,newj);
									pt_cmm_list[el](newi,newj);
									int index=mod(newi,size)+size*mod(newj,size);
									int index2=i+k*size+width*(j+l*size);
									PAINTER_COPY_RGB;
									newi=i;
									newj=j;
									flip(newi,newj);
									pt_cmm_list[et](newi,newj);
									index=mod(newi,size)+size*mod(newj,size);
									index2=j+k*size+width*(i+l*size);
									PAINTER_COPY_RGB;
									newi=i;
									newj=j;
									flip(newi,newj);
									pt_cmm_list[er](newi,newj);
									index=mod(newi,size)+size*mod(newj,size);
									index2=(size1-i)+k*size+width*((size1-j)+l*size);
									PAINTER_COPY_RGB;
									newi=i;
									newj=j;
									flip(newi,newj);
									pt_cmm_list[eb](newi,newj);
									index=mod(newi,size)+size*mod(newj,size);
									index2=(size1-j)+k*size+width*((size1-i)+l*size);
									PAINTER_COPY_RGB;
								}
							}
						}
					}
				}
			}
			break;
		case SYM_CMM:
		{
			points = rectangle(0,0,halfsize-1,halfsize-1);
			auto random_trans = [&]() { return random_bool()?pt_hflip:pt_ident; };
			for(k=0;k<xtiles;k++)
				for(l=0;l<ytiles;l++) {
					copy(points,random_trans(),painter_transformation(1,0,k*size,0,1,l*size));
					copy(points,random_trans(),painter_transformation(-1,0,k*size-1,0,1,l*size));
					copy(points,random_trans(),painter_transformation(1,0,k*size,0,-1,l*size-1));
					copy(points,random_trans(),painter_transformation(-1,0,k*size-1,0,-1,l*size-1));
			}
		} break;
		case SYM_P2:
		case SYM_PMM:
		case SYM_P4M:
		case SYM_PMG:
		case SYM_PGG:
		{
			points = triangle(0,0,qsize,qsize,qsize,-qsize);
			int xt2=xtiles*2, yt2=ytiles*2, t2=xt2*yt2;
			vector<int> z(t2);
			for(k=0;k<t2;k++)
				z[k]=random_int(2);
			int xoff = (sg_basic==SYM_PMG||sg_basic==SYM_PGG)?halfsize/2:0;
			int yoff = (sg_basic==SYM_PGG)?halfsize/2:0;
			painter_transformation from_trans[8] = {
				{1,0,xoff,0,1,yoff},
				{0,1,xoff,1,0,yoff},
				{0,1,xoff+halfsize,1,0,yoff+halfsize},
				{1,0,xoff+halfsize,0,1,yoff+halfsize},
				{1,0,xoff+halfsize,0,1,yoff},
				{0,1,xoff+halfsize,1,0,yoff},
				{0,1,xoff,1,0,yoff+halfsize},
				{1,0,xoff,0,1,yoff+halfsize}};
			for(k=0;k<xt2;k++)
				for(l=0;l<yt2;l++) {
					copy(points,from_trans[4*((l^k)&1)+2*z[k*yt2+l]+z[((k+1)%xt2)*yt2+l]],
						painter_transformation(1,0,halfsize*k,0,1,halfsize*l));
					copy(points,from_trans[4*((l^k)&1)+2*z[k*yt2+l]+z[k*yt2+((l+1)%yt2)]],
						painter_transformation(0,1,halfsize*k,1,0,halfsize*l));
					copy(points,from_trans[4*((l^k)&1)+2*z[k*yt2+l]+z[mod(k-1,xt2)*yt2+l]],
						painter_transformation(-1,0,halfsize*k,0,-1,halfsize*l));
					copy(points,from_trans[4*((l^k)&1)+2*z[k*yt2+l]+z[k*yt2+mod(l-1,yt2)]],
						painter_transformation(0,-1,halfsize*k,-1,0,halfsize*l));
				}
		} break;
		case SYM_P4:
			{
				int ntiles=xtiles*ytiles;
				vector<int> z(ntiles);
				for(k=0;k<ntiles;k++)
					z[k]=rand();
				auto PAINTER_RANDOMIZE_P4_LOOP = [&]() {
					for(i=0;i<qsize;i++)
						for(j=i;i+j<halfsize;j++) {
							pt.set_point(i,j);
							PAINTER_NEW_COPY_RGB();
						}
				};
				for(k=0;k<xtiles;k++)
					for(l=0;l<ytiles;l++) {
						pt.set_to_trans(1,0,size*k,0,1,size*l);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x1,z[ytiles*k+l]&0x4);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(0,1,size*k,1,0,size*l);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x1,z[ytiles*k+l]&0x8);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(0,1,size*k,-1,0,size*l-1);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x1,z[ytiles*k+l]&0x10);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(1,0,size*k,0,-1,size*l-1);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x1,z[ytiles*k+l]&0x20);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(-1,0,size*k-1,0,-1,size*l-1);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x1,z[ytiles*k+l]&0x40);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(0,-1,size*k-1,-1,0,size*l-1);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x1,z[ytiles*k+l]&0x80);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(0,-1,size*k-1,1,0,size*l);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x1,z[ytiles*k+l]&0x100);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(-1,0,size*k-1,0,1,size*l);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x1,z[ytiles*k+l]&0x200);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(1,0,size*k+halfsize,0,1,size*l+halfsize);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x2,z[ytiles*((k+1)%xtiles)+((l+1)%ytiles)]&0x80);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(0,1,size*k+halfsize,1,0,size*l+halfsize);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x2,z[ytiles*((k+1)%xtiles)+((l+1)%ytiles)]&0x40);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(0,1,size*k+halfsize,-1,0,size*l+halfsize1);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x2,z[ytiles*((k+1)%xtiles)+l]&0x200);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(1,0,size*k+halfsize,0,-1,size*l+halfsize1);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x2,z[ytiles*((k+1)%xtiles)+l]&0x100);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(-1,0,size*k+halfsize1,0,-1,size*l+halfsize1);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x2,z[ytiles*k+l]&0x8);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(0,-1,size*k+halfsize1,-1,0,size*l+halfsize1);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x2,z[ytiles*k+l]&0x4);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(0,-1,size*k+halfsize1,1,0,size*l+halfsize);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x2,z[ytiles*k+((l+1)%ytiles)]&0x20);
						PAINTER_RANDOMIZE_P4_LOOP();
						pt.set_to_trans(-1,0,size*k+halfsize1,0,1,size*l+halfsize);
						randomize_p4_choose_from_trans(pt,z[ytiles*k+l]&0x2,z[ytiles*k+((l+1)%ytiles)]&0x10);
						PAINTER_RANDOMIZE_P4_LOOP();
					}
			}
			break;
		case SYM_P4G:
			for(k=0;k<xtiles;k++)
				for(l=0;l<ytiles;l++) {
					int z=rand();
					for(i=0;i<halfsize;i++)
						for(j=0;j<=i;j++) {
							int index=(z&0x1)?(i+size*j):(j+size*i);
							int index2=i+k*size+width*(j+l*size);
							PAINTER_COPY_RGB;
							index=(z&0x2)?(i+size*j):(j+size*i);
							index2=j+k*size+width*(i+l*size);
							PAINTER_COPY_RGB;
							index=(z&0x4)?(i+size*j):(j+size*i);
							index2=size1-i+k*size+width*(j+l*size);
							PAINTER_COPY_RGB;
							index=(z&0x8)?(i+size*j):(j+size*i);
							index2=j+k*size+width*(size1-i+l*size);
							PAINTER_COPY_RGB;
							index=(z&0x10)?(i+size*j):(j+size*i);
							index2=size1-i+k*size+width*(size1-j+l*size);
							PAINTER_COPY_RGB;
							index=(z&0x20)?(i+size*j):(j+size*i);
							index2=size1-j+k*size+width*(size1-i+l*size);
							PAINTER_COPY_RGB;
							index=(z&0x40)?(i+size*j):(j+size*i);
							index2=i+k*size+width*(size1-j+l*size);
							PAINTER_COPY_RGB;
							index=(z&0x80)?(i+size*j):(j+size*i);
							index2=size1-j+k*size+width*(i+l*size);
							PAINTER_COPY_RGB;
						}
				}
			break; 
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
				auto PAINTER_RANDOMIZE_P3M1_LOOP = [&]() {
					for(i=0;3*i<=size+2;i++)
						for(j=0;3*(i+j)<=size+2;j++) {
							pt.set_point(i,j);
							PAINTER_NEW_COPY_RGB();
						}
				};
				int yt=ytiles*3;
				int ntiles=xtiles*yt;
				vector<unsigned char>types(ntiles);
				for(i=0;i<ntiles;i++)
					types[i]=rand()%3;
				for(k=0;k<xtiles;k++)
					for(l=0;l<ytiles;l++) {
						int z=rand();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k],types[3*l+yt*k+1],z&0x1);
						pt.set_to_trans(1,0,size*k,0,1,size*l);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k+1],types[3*l+yt*k],z&0x1);
						pt.set_to_trans(0,-1,size*k+(size+1)/3,-1,0,size*l+(size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k],types[3*l+yt*mod(k-1,xtiles)+2],z&0x2);
						pt.set_to_trans(-1,0,size*k,1,1,size*l);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*mod(k-1,xtiles)+2],types[3*l+yt*k],z&0x2);
						pt.set_to_trans(0,1,size*k-(size+1)/3,-1,-1,size*l+(2*size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k],types[3*l+yt*mod(k-1,xtiles)+1],z&0x4);
						pt.set_to_trans(-1,-1,size*k,1,0,size*l);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*mod(k-1,xtiles)+1],types[3*l+yt*k],z&0x4);
						pt.set_to_trans(1,1,size*k-(2*size+1)/3,0,-1,size*l+(size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k],types[3*mod(l-1,ytiles)+yt*mod(k-1,xtiles)+2],z&0x8);
						pt.set_to_trans(0,-1,size*k,-1,0,size*l);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*mod(l-1,ytiles)+yt*mod(k-1,xtiles)+2],types[3*l+yt*k],z&0x8);
						pt.set_to_trans(1,0,size*k-(size+1)/3,0,1,size*l-(size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k],types[3*mod(l-1,ytiles)+yt*k+1],z&0x10);
						pt.set_to_trans(0,1,size*k,-1,-1,size*l);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*mod(l-1,ytiles)+yt*k+1],types[3*l+yt*k],z&0x10);
						pt.set_to_trans(-1,0,size*k+(size+1)/3,1,1,size*l-(2*size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k],types[3*mod(l-1,ytiles)+yt*k+2],z&0x20);
						pt.set_to_trans(1,1,size*k,0,-1,size*l);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*mod(l-1,ytiles)+yt*k+2],types[3*l+yt*k],z&0x20);
						pt.set_to_trans(-1,-1,size*k+(2*size+1)/3,1,0,size*l-(size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k+1],types[3*l+yt*k+2],z&0x40);
						pt.set_to_trans(1,0,size*k+(size+1)/3,0,1,size*l+(size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*k+2],types[3*l+yt*k+1],z&0x40);
						pt.set_to_trans(0,-1,size*k+(2*size+1)/3,-1,0,size*l+(2*size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*mod(k-1,xtiles)+1],types[3*l+yt*mod(k-2,xtiles)+2],z&0x80);
						pt.set_to_trans(-1,-1,size*k-(2*size+1)/3,1,0,size*l+(size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*l+yt*mod(k-2,xtiles)+2],types[3*l+yt*mod(k-1,xtiles)+1],z&0x80);
						pt.set_to_trans(1,1,size*k-(4*size+1)/3,0,-1,size*l+(2*size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*mod(l-1,ytiles)+yt*k+1],types[3*mod(l-2,ytiles)+yt*k+2],z&0x100);
						pt.set_to_trans(0,1,size*k+(size+1)/3,-1,-1,size*l-(2*size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
						randomize_p3m1_choose_from_trans
							(pt,types[3*mod(l-2,ytiles)+yt*k+2],types[3*mod(l-1,ytiles)+yt*k+1],z&0x100);
						pt.set_to_trans(-1,0,size*k+(2*size+1)/3,1,1,size*l-(4*size+1)/3);
						PAINTER_RANDOMIZE_P3M1_LOOP();
					}
			}
	}
}

void painter::randomize_p3m1_choose_from_trans(painter_transform &pt, int mt,
		int nt, int z)
{
	if(mt==0) {
		if((nt==0&&z)||nt==1) {
			pt.set_from_trans(1,0,0,0,1,0);
		}
		else {
			pt.set_from_trans(0,-1,0,-1,0,0);
		}
	}
	else if(mt==1) {
		if((nt==1&&z)||nt==2) {
			pt.set_from_trans(1,0,(size+1)/3,0,1,(size+1)/3);
		}
		else{
			pt.set_from_trans(0,-1,(size+1)/3,-1,0,(size+1)/3);
		}
	}
	else {
		if((nt==2&&z)||nt==0) {
			pt.set_from_trans(1,0,-((size+1)/3),0,1,-((size+1)/3));
		}
		else {
			pt.set_from_trans(0,-1,-((size+1)/3),-1,0,-((size+1)/3));
		}
	}
}

void painter::randomize_p4_choose_from_trans(painter_transform &pt, bool a, bool b)
{
	if(a)
		if(b)
			pt.set_from_trans(1,0,0,0,1,0);
		else
			pt.set_from_trans(0,1,0,1,0,0);
	else
		if(b)
			pt.set_from_trans(0,-1,halfsize1,-1,0,halfsize1);
		else
			pt.set_from_trans(-1,0,halfsize1,0,-1,halfsize1);
}
