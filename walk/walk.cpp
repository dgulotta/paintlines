/***************************************************************************
 *   Copyright (C) 2017 by Daniel Gulotta                                  *
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
#include "../randgen.h"
#include "walk.h"

using std::make_tuple;
using std::tuple;

int sgn(long x)
{
	if(x>0)
		return 1;
	else if (x<0)
		return -1;
	else
		return 0;
}

wrap_canvas<uint8_t> levels(const wrap_canvas<int> &c)
{
	int w=c.width(), h=c.height();
	wrap_canvas<int> mark(2*w,2*h,-1);
	wrap_canvas<tuple<int8_t,int8_t>> dirs(2*w,2*h);
	std::vector<long> areas;
	for(int i=0;i<2*w;i++)
		for(int j=0;j<2*h;j++) {
			if (!((i^j)&1)) continue;
			if (mark(i,j)>=0) continue;
			int x=i,y=j,dx=j&1,dy=i&1,n=areas.size();
			long area=0;
			while(mark(x,y)<0) {
				mark(x,y)=n;
				dirs(x,y)=make_tuple((int8_t)dx,(int8_t)dy);
				int cc=c(divide(x+dx,2),divide(y+dy,2));
				int newdx, newdy;
				switch(cc) {
				case 0:
					newdx=dy;
					newdy=dx;
					break;
				case 1:
					newdx=-dy;
					newdy=-dx;
					break;
				default:
					newdx=dx;
					newdy=dy;
				}
				area+=(2*y+dy+newdy)*(dx+newdx);
				x+=dx+newdx;
				y+=dy+newdy;
				dx=newdx;
				dy=newdy;
			}
			if (x==i&&y==j)
				areas.push_back(area);
			else
				areas.push_back(0);
		}
	int maxval=-1,minval=-1;
	for(int i=0;i<w;i++) {
		int s = sgn(areas[mark(2*i-1,0)]*std::get<1>(dirs(2*i-1,0)));
		int newm = mark(2*i-2,0)-s;
		if(newm>maxval)
			maxval=newm;
		if(newm<minval)
			minval=newm;
		mark(2*i,0)=newm;
		for(int j=1;j<h;j++) {
			int s =	sgn(areas[mark(2*i,2*j-1)]*std::get<0>(dirs(2*i,2*j-1)));
			int newm=mark(2*i,2*j-2)+s;
			if(newm>maxval)
				maxval=newm;
			if(newm<minval)
				minval=newm;
			mark(2*i,2*j)=newm;
		}
	}
	wrap_canvas<uint8_t> sc(w,h);
	for(int i=0;i<w;i++)
		for(int j=0;j<h;j++) {
			double ll = 255.99*(mark(2*i,2*j)-minval)/(maxval-minval);
			uint8_t col = colorchop(ll);
			sc(i,j)=col;
		}
	return sc;
}

wrap_canvas<color_t> draw_walk(int width, int height, bool balanced)
{
	wrap_canvas<int> cr(width,height);
	wrap_canvas<int> cg(width,height);
	wrap_canvas<int> cb(width,height);
	std::vector<double> probs(6);
	if(balanced) {
		std::vector<double> tprobs(6);
		do {
			for(int i=0;i<6;i++)
				tprobs[i]=random_normal(1.);
			double p1=(tprobs[0]+tprobs[1]-tprobs[2]-tprobs[3])/4;
			double p2=(tprobs[0]-tprobs[1]-tprobs[2]
				+tprobs[3]-2*tprobs[4]+2*tprobs[5])/12;
			probs[0]=tprobs[0]-p1-p2;
			probs[1]=tprobs[1]-p1+p2;
			probs[2]=tprobs[2]+p1+p2;
			probs[3]=tprobs[3]+p1-p2;
			probs[4]=tprobs[4]+2*p2;
			probs[5]=tprobs[5]-2*p2;
		} while(any_of(probs.begin(),probs.end(),[] (double x) { return x<0; }));
	}
	else {
		for(int i=0;i<6;i++)
			probs[i]=fabs(random_normal(1.));
	}
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++) {
			switch(random_weighted(probs)) {
				case 0:
					cr(i,j)=0;
					cg(i,j)=1;
					cb(i,j)=2;
					break;
				case 1:
					cr(i,j)=0;
					cg(i,j)=2;
					cb(i,j)=1;
					break;
				case 2:
					cr(i,j)=1;
					cg(i,j)=0;
					cb(i,j)=2;
					break;
				case 3:
					cr(i,j)=1;
					cg(i,j)=2;
					cb(i,j)=0;
					break;
				case 4:
					cr(i,j)=2;
					cg(i,j)=0;
					cb(i,j)=1;
					break;
				case 5:
					cr(i,j)=2;
					cg(i,j)=1;
					cb(i,j)=0;
					break;
			}
		}
	auto sr = levels(cr);
	auto sg = levels(cg);
	auto sb = levels(cb);
	wrap_canvas<color_t> sc(width,height);
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			sc(i,j)=color_t(sr(i,j),sg(i,j),sb(i,j));
	return sc;
}

