/***************************************************************************
 *   Copyright (C) 2005-2007, 2013 by Daniel Gulotta                       *
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

#include "hyperbolic_paintlines.h"
#include "../randgen.h"

#include <iostream>
#include <limits>
using namespace std;
using namespace std::placeholders;

hyperbolic_coord random_mid(const hyperbolic_coord &c1,
			    const hyperbolic_coord &c2, double var)
{
  hyperbolic_coord mid(normalize(c1+c2));
  hyperbolic_coord diff(normalize(c1-c2));
  hyperbolic_coord norm(cross(mid,diff));
  double z=1.+random_exponential(var);
  double r=sqrt(z*z-1.);
  double q=random_angle();
  return normalize(z*mid+r*cos(q)*diff+r*sin(q)*norm);
}

void hyperbolic_paintlines::paint(int sz, hyperbolic_symmetry_group &sym)
{
	double dist;
	image=canvas<color_t>(sz,sz);
	switch(pt) {
	case projtype::POINCARE:
		proj=&poincare_projection;
		dist=1.+.01/sz;
		break;
	case projtype::KLEIN:
		proj=&klein_projection;
		dist=1.+.005/sz;
	}
	drawdot_symmetric=sym.symmetrize(bind(&hyperbolic_paintlines::drawdot,this,_1));
	grids.resize(ncolors);
	layers.resize(ncolors);
	for(int i=0;i<ncolors;i++) {
		grids[i]=canvas<uint8_t>(sz,sz,0);
		layers[i].pixels=active_grid=&(grids[i]);
		layers[i].pastel=false;
		layers[i].color=default_color_generator();
		hyperbolic_coord c(0,0,1);
		hyperbolic_coord c2(0,0,1);
		do {
			double z=1.+random_exponential(1);
			double r=sqrt(z*z-1.);
			double q=random_angle();
			c=hyperbolic_coord(r*cos(q),r*sin(q),z);
			c2=sym.random_symmetry(c);
		} while(c.z>=25.||c2.z>=25.);
		drawsmoothline(c,normalize(c2),.02,dist);
		if(pt==projtype::POINCARE)
			fill_poincare();
		else
			fill_klein();
	}
	merge(image,layers);
}

void hyperbolic_paintlines::drawdot(const hyperbolic_coord &hc)
{
	screen_coord sc = toscreen((*proj)(hc));
	(*active_grid)(sc.x,sc.y)=255;
}

void hyperbolic_paintlines::fill_poincare() {
	int size=this->size();
	canvas<float> mask(size,size,std::numeric_limits<float>::infinity());
	int x,y,i,j;
	for(y=0;y<size;y++)
		for(x=0;x<size;x++)
			if((*active_grid)(x,y)) {
				planar_coord pc = fromscreen(screen_coord(x,y));
				double z = 1-pc.x*pc.x-pc.y*pc.y;
				if(z<=0) continue;
				int r = int(radius*z);
				double d2=1/(z*z*brightness);
				if(r>x) r=x;
				if(r>y) r=y;
				if(r+x>=size) r=size-x-1;
				if(r+y>=size) r=size-y-1;
				for(i=-r;i<=r;i++)
					for(j=-r;j<=r;j++) {
						float val = d2*(i*i+j*j);
						if(mask(x+i,y+j)>val) mask(x+i,y+j)=val;
					}
			}
	fill_color(mask);
}

void hyperbolic_paintlines::fill_klein()
{
	int size=this->size();
	canvas<float> mask(size,size,std::numeric_limits<float>::infinity());
	int x,y,i,j;
	for(y=0;y<size;y++)
		for(x=0;x<size;x++)
			if((*active_grid)(x,y)) {
				planar_coord pc = fromscreen(screen_coord(x,y));
				double x2 = pc.x*pc.x, y2 = pc.y*pc.y;
				double z = 1-pc.x*pc.x-pc.y*pc.y;
				if(z<=0) continue;
				double d2 = 1/(z*z*brightness);
				double xx = d2*(1-y2)/2;
				double yy = d2*(1-x2)/2;
				double xy = d2*pc.x*pc.y;
				int r = int(2*radius*sqrt(z));
				if(r>x) r=x;
				if(r>y) r=y;
				if(r+x>=size) r=size-x-1;
				if(r+y>=size) r=size-y-1;
				for(j=-r;j<=r;j++)
					for(i=-r;i<=r;i++) {
						float val = xx*i*i+yy*j*j+xy*i*j;
						if(mask(x+i,y+j)>val) mask(x+i,y+j)=val;
					}
			}
	fill_color(mask);
}

void hyperbolic_paintlines::fill_color(const canvas<float> &mask) {
	for(int j=0;j<size();j++)
		for(int i=0;i<size();i++) {
			float f = mask(i,j);
			if(isinf(f)||f==0) continue;
			(*active_grid)(i,j)=255.99/(1.+pow(f,sharpness));
		}
}

void hyperbolic_paintlines::drawsmoothline
(const hyperbolic_coord &end1, const hyperbolic_coord &end2, double var,
 double min)
{
  if(end1*end1<.99||end2*end2<.99) {
    cout << "bad point!" << endl;
  }
  if(end1*end2>min) {
    hyperbolic_coord mid=random_mid(end1,end2,var);
    drawsmoothline(end1,mid,var/3.,min);
    drawdot_symmetric(mid);
    drawsmoothline(mid,end2,var/3.,min);
  }
}
