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

#include "generators.hpp"
#include "randgen.hpp"

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

void drawsmoothline(const function<void(const hyperbolic_coord &)> &draw,
	const hyperbolic_coord &end1, const hyperbolic_coord &end2,
	double var, double min)
{
	if(end1*end2>min) {
		hyperbolic_coord mid=random_mid(end1,end2,var);
		draw(mid);
		drawsmoothline(draw,end1,mid,var/3,min);
		drawsmoothline(draw,mid,end2,var/3,min);
	}
}

void fill_color(canvas<uint8_t> &grid, const canvas<float> &mask,float sharpness) {
	for(int j=0;j<grid.height();j++)
		for(int i=0;i<grid.width();i++) {
			float f = mask(i,j);
			if(isinf(f)||f==0) continue;
			grid(i,j)=255.99/(1.+pow(f,sharpness));
		}
}

void fill(canvas<uint8_t> &src, const hyperbolic_lines_param &p)
{
	int size=src.height();
	canvas<float> mask(size,size,std::numeric_limits<float>::infinity());
	if(p.proj==projtype::POINCARE) {
		for(int y=0;y<size;y++)
			for(int x=0;x<size;x++)
				if(src(x,y)) {
					planar_coord pc = coord_converter(size).fromscreen(screen_coord(x,y));
					double z = 1-pc.x*pc.x-pc.y*pc.y;
					if(z<=0) continue;
					int r = int(p.radius*z);
					double d2=1/(z*z*p.brightness);
					if(r>x) r=x;
					if(r>y) r=y;
					if(r+x>=size) r=size-x-1;
					if(r+y>=size) r=size-y-1;
					for(int i=-r;i<=r;i++)
						for(int j=-r;j<=r;j++) {
							float val = d2*(i*i+j*j);
							if(mask(x+i,y+j)>val) mask(x+i,y+j)=val;
						}
				}
	}
	else {
		for(int y=0;y<size;y++)
			for(int x=0;x<size;x++)
				if(src(x,y)) {
					planar_coord pc = coord_converter(size).fromscreen(screen_coord(x,y));
					double x2 = pc.x*pc.x, y2 = pc.y*pc.y;
					double z = 1-pc.x*pc.x-pc.y*pc.y;
					if(z<=0) continue;
					double d2 = 1/(z*z*p.brightness);
					double xx = d2*(1-y2)/2;
					double yy = d2*(1-x2)/2;
					double xy = d2*pc.x*pc.y;
					int r = int(2*p.radius*sqrt(z));
					if(r>x) r=x;
					if(r>y) r=y;
					if(r+x>=size) r=size-x-1;
					if(r+y>=size) r=size-y-1;
					for(int j=-r;j<=r;j++)
						for(int i=-r;i<=r;i++) {
							float val = xx*i*i+yy*j*j+xy*i*j;
							if(mask(x+i,y+j)>val) mask(x+i,y+j)=val;
						}
				}
	}
	fill_color(src,mask,p.sharpness);
}

canvas<uint8_t> paint_hyperbolic_lines(const hyperbolic_symmetry_group &sg,const hyperbolic_lines_param &params)
{
	double dist=(params.proj==projtype::POINCARE)?(1+.01/params.size):(1+.005/params.size);
	canvas<uint8_t> grid(params.size,params.size);
	auto drawfunc = sg.symmetrize([&grid,proj=params.proj,size=params.size] (const hyperbolic_coord &hc) {
		screen_coord sc = coord_converter(size).toscreen(projection(hc,proj));
		grid(sc.x,sc.y)=255;
	});	
	hyperbolic_coord c(0,0,1);
	hyperbolic_coord c2(0,0,1);
	do {
		double z=1.+random_exponential(1);
		double r=sqrt(z*z-1.);
		double q=random_angle();
		c=hyperbolic_coord(r*cos(q),r*sin(q),z);
		c2=sg.random_symmetry(c);
	} while(c.z>=25.||c2.z>=25.);
	drawsmoothline(drawfunc,c,normalize(c2),.02,dist);
	fill(grid,params);
	return grid;
}


