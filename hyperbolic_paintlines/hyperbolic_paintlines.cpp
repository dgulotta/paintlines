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
  hyperbolic_painter::paint(sz,sym);
  alpha.resize(size*size);
  red_brushes.resize(ncolors);
  green_brushes.resize(ncolors);
  blue_brushes.resize(ncolors);
  fill(red.begin(),red.end(),0);
  fill(green.begin(),green.end(),0);
  fill(blue.begin(),blue.end(),0);
  double dist;
  switch(pt) {
  case POINCARE:
    proj=&poincare_projection;
    dist=1.+.01/size;
    break;
  case KLEIN:
    proj=&klein_projection;
    dist=1.+.005/size;
  }
  drawdot_symmetric=sym.symmetrize(bind(&hyperbolic_paintlines::drawdot,this,_1));
  t=ncolors;
  while(t--) {
    switch(random_int(6)) {
    case 0:
      red_brushes[t]=255;
      green_brushes[t]=random_int(256);
      blue_brushes[t]=0;
      break;
    case 1:
      red_brushes[t]=0;
      green_brushes[t]=255;
      blue_brushes[t]=random_int(256);
      break;
    case 2:
      red_brushes[t]=random_int(256);
      green_brushes[t]=0;
      blue_brushes[t]=255;
      break;
    case 3:
      red_brushes[t]=random_int(256);
      green_brushes[t]=255;
      blue_brushes[t]=0;
      break;
    case 4:
      red_brushes[t]=0;
      green_brushes[t]=random_int(256);
      blue_brushes[t]=255;
      break;
    default:
      red_brushes[t]=255;
      green_brushes[t]=0;
      blue_brushes[t]=random_int(256);
    }
    hyperbolic_coord c;
    hyperbolic_coord c2;
    do {
      double z=1.+random_exponential(1);
      double r=sqrt(z*z-1.);
      double q=random_angle();
      c=hyperbolic_coord(r*cos(q),r*sin(q),z);
      c2=sg->random_symmetry(c);
    } while(c.z>=25.||c2.z>=25.);
	fill(alpha.begin(),alpha.end(),0);
    drawsmoothline(c,normalize(c2),.02,dist);
	if(pt==POINCARE)
		fill_poincare(t);
	else
		fill_klein(t);
  }
}

void hyperbolic_paintlines::drawdot(const hyperbolic_coord &hc)
{
	screen_coord sc = toscreen((*proj)(hc));
	alpha[sc.x+sc.y*size]=255;
}

void hyperbolic_paintlines::fill_poincare(int t) {
	vector<float> mask(size*size,std::numeric_limits<float>::infinity());
	int x,y,i,j;
	for(x=0;x<size;x++)
		for(y=0;y<size;y++)
			if(alpha[x+y*size]) {
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
						int index = (x+i)+(y+j)*size;
						float val = d2*(i*i+j*j);
						if(mask[index]>val) mask[index]=val;
					}
			}
	fill_color(mask,t);
}

void hyperbolic_paintlines::fill_klein(int t)
{
	vector<float> mask(size*size,std::numeric_limits<float>::infinity());
	int x,y,i,j;
	for(x=0;x<size;x++)
		for(y=0;y<size;y++)
			if(alpha[x+y*size]) {
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
				for(i=-r;i<=r;i++)
					for(j=-r;j<=r;j++) {
						int index = (x+i)+(y+j)*size;
						float val = xx*i*i+yy*j*j+xy*i*j;
						if(mask[index]>val) mask[index]=val;
					}
			}
	fill_color(mask,t);
}

void hyperbolic_paintlines::fill_color(const vector<float> &mask, int t) {
	int r=red_brushes[t], g=green_brushes[t], b=blue_brushes[t], i;
	for(i=size*size-1;i>=0;i--) {
		float f = mask[i];
		if(isinf(f)) continue;
		if(f==0) {
			red[i]=r;
			green[i]=g;
			blue[i]=b;
		}
		else {
			unsigned short a = 255.99/(1.+pow(f,sharpness));
			red[i]+=a*(r-red[i])/255;
			green[i]+=a*(g-green[i])/255;
			blue[i]+=a*(b-blue[i])/255;
		}
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
