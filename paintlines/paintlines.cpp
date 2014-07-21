/***************************************************************************
 *   Copyright (C) 2003-2005, 2013-2014 by Daniel Gulotta                  *
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

#include "paintlines.h"
#include "../randgen.h"
#include <algorithm>
#include <cmath>

using std::lower_bound;
using std::cos;
using std::sin;
using std::log;
using std::sqrt;
using std::rand;
using std::tuple;
using std::make_tuple;
using std::get;

void paintlines_layer_generator::randomnormal_hexagonal(double &x, double &y, double var)
{
  double v = sqrt(var/2);
  double s=random_normal(v)*.93060485910209959893;
  double d=random_normal(v)*.53728496591177095978;
  x+=s+d;
  y+=s-d;
}

void paintlines_layer_generator::randomnormal_orthogonal(double &x, double &y, double var)
{
  double v = sqrt(var/2);
  x+=random_normal(v);
  y+=random_normal(v);
}

void paintlines::paint(int sz, symgroup sym)
{
	size=sz;
	sg=sym;
	grids.clear();
	grids.resize(ncolors,symmetric_canvas<uint8_t>(sz,sym,0));
	layers.resize(ncolors);
	for(int i=0;i<ncolors;i++) {
		layer &l = layers[i];
		auto it=lower_bound(rules.begin(),rules.end(),random_int(rules.back().weight)+1);
		active_grid=&grids[i];
		l.pixels=&(grids[i].as_canvas());
		l.color=generate_color();
		l.pastel=it->pastel;
		handle_rule(it->type);
	}
	image=symmetric_canvas<color_t>(sz,sym,black);
	merge(image.unsafe_get_canvas(),layers);
}

tuple<int,int,int,int> random_endpoints(int size,int wrap=2)
{
	int i = random_int(size), j = random_int(size);
	return make_tuple(i,j,i+random_range_inclusive(-wrap,wrap)*size,j+random_range_inclusive(-wrap,wrap)*size);
}

void paintlines_layer_generator::generate_smootharc(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	auto t = random_endpoints(g.size());
	gen.drawsmootharc(get<0>(t),get<1>(t),get<2>(t),get<3>(t),.8,2000.,1.);
}

void paintlines_layer_generator::generate_smoothline2_beads(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	auto t = random_endpoints(g.size());
	gen.drawsmoothline2(get<0>(t),get<1>(t),get<2>(t),get<3>(t),25000.,100.);
}

void paintlines_layer_generator::generate_smoothline2(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	auto t = random_endpoints(g.size());
	gen.drawsmoothline2(get<0>(t),get<1>(t),get<2>(t),get<3>(t),25000.,1.);
}

void paintlines_layer_generator::generate_cluster(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	gen.drawcluster(random_int(g.size()),random_int(g.size()),4000.,4);
}

void paintlines_layer_generator::generate_flower(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	gen.drawflower(random_int(g.size()),random_int(g.size()),.1,50);
}

static const int fractal_prob[13] = { 0, 40, 40, 38, 37, 0, 35, 0, 34, 0, 0, 0, 32 };

void paintlines_layer_generator::generate_cluster2(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	gen.drawfractal(random_int(g.size()),random_int(g.size()),g.size(),fractal_prob[num_symmetries[g.group()]]);
}

void paintlines_layer_generator::generate_open_string(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	gen.drawopenstring();
}

void paintlines_layer_generator::generate_swirl(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	do {
		gen.drawswirl();
	} while(random_int(20)>=num_symmetries[g.group()]);
}

void paintlines_layer_generator::generate_orbit(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	gen.draworbit();
}

void paintlines_layer_generator::generate_tree(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	gen.drawtree();
}

void paintlines_layer_generator::generate_smoothline5(symmetric_canvas<uint8_t> &g)
{
	paintlines_layer_generator gen(g);
	auto t = random_endpoints(g.size(),1);
	double vx = random_normal(g.size()), vy = random_normal(g.size());
	gen.drawsmoothline5(get<0>(t),get<1>(t),vx,vy,get<2>(t),get<3>(t),
		vx,vy,75000.,1.);
}

const std::function<void(symmetric_canvas<uint8_t> &)> paintlines::rulefuncs[] = {
	paintlines_layer_generator::generate_smootharc,
	paintlines_layer_generator::generate_smoothline2_beads,
	paintlines_layer_generator::generate_cluster,
	paintlines_layer_generator::generate_flower,
	paintlines_layer_generator::generate_cluster2,
	paintlines_layer_generator::generate_smoothline2,
	paintlines_layer_generator::generate_open_string,
	paintlines_layer_generator::generate_swirl,
	paintlines_layer_generator::generate_orbit,
	paintlines_layer_generator::generate_tree,
	paintlines_layer_generator::generate_smoothline5,
};

void paintlines::handle_rule(ruletype rt)
{
	rulefuncs[rt](*active_grid);
}

void paintlines_layer_generator::drawdotsymmetric(int x, int y, int radius,double brightness)
{
	int i,j;
	double num, denom;
	if(is_hexagonal()) {
		denom=brightness*4.33;
		num=brightness*1104.;
		for(i=-radius;i<=radius;i++)
			for(j=-radius;j<=radius;j++) {
				drawpixelsymmetric(x+i,y+j,num/(denom+i*i+i*j+j*j));
			}
	}
	else {
		denom=brightness*5.;
		num=brightness*1275.;
		for(i=-radius;i<=radius;i++)
			for(j=-radius;j<=radius;j++) {
				drawpixelsymmetric(x+i,y+j,num/(denom+i*i+j*j));
			}
	} 
}

void paintlines_layer_generator::drawcluster(double x, double y, double var, int maxdepth)
{
  double mx=x,my=y;
  randomnormal(mx,my,var);
  int z=random_int(maxdepth);
  while(z--) {
    drawsmoothline2(x,y,mx,my,var/4.,1.);
    drawcluster(mx,my,var/4.,maxdepth-1);
  }
  drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster2(int x, int y, int d)
{
  if(d) {
    int z=random_int(4);
    d>>=1;
    if(z) drawcluster2(x,y,d);
    if(z^1) drawcluster2(x+d,y,d);
    if(z^2) drawcluster2(x,y+d,d);
    if(z^3) drawcluster2(x+d,y+d,d);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster3(int x, int y, int d)
{
  if(d) {
    d/=2;
    int z=random_int(4);
    if(z) drawcluster3(x,y,d);
    if(z^1) drawcluster3(x+d,y,d);
    if(z^2) drawcluster3(x,y+d,d);
    if(z^3) drawcluster3(x+d,y+d,-d);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster4(int x, int y, int d)
{
  if(d) {
    int z=random_int(4);
    int d2=d/2;
    if(z) drawcluster4(x,y,d2);
    if(z^1) drawcluster4(x+d2,y+d2,d2);
    if(z^2) drawcluster4(x-d2,y+d2,d2);
    if(z^3) drawcluster4(x,y+d,-d2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster5(int x, int y, int d)
{
  if(d) {
    d/=2;
    if(random_int(3)) drawcluster5(x,y,d);
    if(random_int(3)) drawcluster5(x+d,y,d);
    if(random_int(3)) drawcluster5(x,y+d,d);
    if(random_int(3)) drawcluster5(x+d,y+d,d);
  }
  else drawdotsymmetric(x,y,5,1.);
}


void paintlines_layer_generator::drawcluster6(int x, int y, int d)
{
  if(d) {
    d/=2;
    if(random_int(3)) drawcluster6(x,y,d);
    if(random_int(3)) drawcluster6(x+d,y,d);
    if(random_int(3)) drawcluster6(x,y+d,d);
    if(random_int(3)) drawcluster6(x+d,y+d,-d);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster7(int x, int y, int d)
{
  if(d) {
    int d2=d/2;
    if(random_int(3)) drawcluster7(x,y,d2);
    if(random_int(3)) drawcluster7(x+d2,y+d2,d2);
    if(random_int(3)) drawcluster7(x-d2,y+d2,d2);
    if(random_int(3)) drawcluster7(x,y+d,-d2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster8(int x, int y, int d)
{
  if(d) {
    d/=2;
    if(random_int(3)) drawcluster6(x,y,d);
    if(random_int(3)) drawcluster8(x+d,y+d,-d);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster9(int x, int y, int d)
{
  if(d) {
    if(random_int(3)) drawcluster9(x,y,d/2);
    if(random_int(3)) drawcluster9(x+d/6,y-d/3,d/2);
    if(random_int(3)) drawcluster9(x+d/3,y-d/6,d/2);
    if(random_int(3)) drawcluster9(x+d/2,y-d/2,d/2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster10(int x, int y, int d)
{
  if(d) {
    if(random_int(3)) drawcluster10(x,y,d/2);
    if(random_int(3)) drawcluster10(x+d/6,y-d/3,d/2);
    if(random_int(3)) drawcluster10(x+d/3,y-d/6,d/2);
    if(random_int(3)) drawcluster10(x+d/2,y-d/2,-d/2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster11(double x, double y, double var, int maxdepth,
		   double dist)
{
  double xx=x, yy=y;
  randomnormal(x,y,var);
  int z=random_int(maxdepth);
  while(z--) {
    drawcluster11(x,y,var/2.,maxdepth-1,dist);
    drawsmoothline2(x,y,xx,yy,var,dist);
  }
}

void paintlines_layer_generator::drawcluster12(int x, int y, int d, unsigned char myalpha)
{
  if(d) {
    d/=2;
	auto randmask = [] () { return random_int(256)|random_int(256)|random_int(256); };
    drawcluster12(x,y,d,myalpha&randmask());
    drawcluster12(x+d,y,d,myalpha&randmask());
    drawcluster12(x,y+d,d,myalpha&randmask());
    drawcluster12(x+d,y+d,d,myalpha&randmask());
  }
  else {
    drawpixelsymmetric(x,y,myalpha);
  }
}

void paintlines_layer_generator::drawcluster13(double x, double y, double var, int maxdepth)
{
  randomnormal(x,y,var);
  int z=random_int(maxdepth);
  while(z--) drawcluster13(x,y,var/2.,maxdepth-1);
  int s=sqrt(var);
  drawdotsymmetric(x,y,s,var/1275.);
}

void paintlines_layer_generator::drawcluster14(double x, double y, double var, int maxdepth)
{
  randomnormal(x,y,var);
  int z=random_int(maxdepth);
  while(z--) drawcluster13(x,y,var/2.,maxdepth-1);
  double s=sqrt(var);
  drawcluster5(x-s/2.,y-s/2.,s);
}

void paintlines_layer_generator::drawcluster15(double x, double y, double m, double px,
			       double py, int maxdepth)
{
  if(maxdepth) {
    double m1=m*random_uniform(), m2=m*random_uniform();
    if(m1+m2>m) {
      m1=m-m1;
      m2=m-m2;
    }
    double ms=m*m, m1s=m1*m1, m2s=m2*m2;
    double p=sqrt((ms*ms+m1s*m1s+m2s*m2s-2*(ms*m1s+ms*m2s+m1s*m2s))/(4*ms));
    double theta=random_angle()/2;
    double px1=px+p*cos(theta);
    double py1=py+p*sin(theta);
    double px2=px-p*cos(theta);
    double py2=py-p*sin(theta);
    double t1=random_exponential(.1);
    double t2=random_exponential(.1);
    drawcluster15(x+px1*t1/m1,y+py1*t1/m1,m1,px1,py1,maxdepth-1);
    drawcluster15(x+px2*t2/m2,y+py2*t2/m2,m2,px2,py2,maxdepth-1);
  }
  else {
    int s=sqrt(m);
    drawdotsymmetric(x,y,s,m/1275.);
  }
}

void paintlines_layer_generator::drawcluster16(double x, double y, double x1, double y1,
			       double x2, double y2, double x3, double y3,
			       int depth)
{
  if(depth--) {
    double x1n=3./7.*x1+1./7.*y1;
    double y1n=-1./7.*x1+2./7.*y1;
    double x2n=3./7.*x2+1./7.*y2;
    double y2n=-1./7.*x2+2./7.*y2;
    double x3n=3./7.*x3+1./7.*y3;
    double y3n=-1./7.*x3+2./7.*y3;
    if(random_int(3)) drawcluster16(x,y,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x1n,y+y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x2n,y+y2n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x3n,y+y3n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x-x1n,y-y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x-x2n,y-y2n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x-x3n,y-y3n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster17(double x, double y, double x1, double y1,
			       double x2, double y2, double x3, double y3,
			       int depth)
{
  if(depth--) {
    double x1n=3./7.*x1+1./7.*y1;
    double y1n=-1./7.*x1+2./7.*y1;
    double x2n=3./7.*x2+1./7.*y2;
    double y2n=-1./7.*x2+2./7.*y2;
    double x3n=3./7.*x3+1./7.*y3;
    double y3n=-1./7.*x3+2./7.*y3;
    if(random_int(3)) drawcluster17(x,y,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x1n,y+y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster18(double x, double y, double x1, double y1,
			       double x2, double y2, double x3, double y3,
			       int depth)
{
  if(depth--) {
    double x1n=3./7.*x1+1./7.*y1;
    double y1n=-1./7.*x1+2./7.*y1;
    double x2n=3./7.*x2+1./7.*y2;
    double y2n=-1./7.*x2+2./7.*y2;
    double x3n=3./7.*x3+1./7.*y3;
    double y3n=-1./7.*x3+2./7.*y3;
    if(random_int(3)) drawcluster18(x,y,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x1n,y+y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x2n,y+y2n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster19(double x, double y, double x1, double y1,
			       double x2, double y2, double x3, double y3,
			       int depth)
{
  if(depth--) {
    double x1n=3./7.*x1+1./7.*y1;
    double y1n=-1./7.*x1+2./7.*y1;
    double x2n=3./7.*x2+1./7.*y2;
    double y2n=-1./7.*x2+2./7.*y2;
    double x3n=3./7.*x3+1./7.*y3;
    double y3n=-1./7.*x3+2./7.*y3;
    if(random_int(3)) drawcluster19(x,y,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x1n,y+y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x2n,y+y2n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(random_int(3)) drawcluster16(x+x3n,y+y3n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawcluster20(double x, double y, double x1, double y1,
			       double x2, double y2, int depth)
{
  if(depth--) {
    double x1n=x1*.4-y1*.2;
    double y1n=x1*.2+y1*.4;
    double x2n=x2*.4-y2*.2;
    double y2n=x2*.2+y2*.4;
    if(random_bool()) drawcluster20(x,y,x1n,y1n,x2n,y2n,depth);
    if(random_bool()) drawcluster20(x+x1n,y+y1n,x1n,y1n,x2n,y2n,depth);
    if(random_bool()) drawcluster20(x+x2n,y+y2n,x1n,y1n,x2n,y2n,depth);
    if(random_bool()) drawcluster20(x-x1n,y-y1n,x1n,y1n,x2n,y2n,depth);
    if(random_bool()) drawcluster20(x-x2n,y-y2n,x1n,y1n,x2n,y2n,depth);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines_layer_generator::drawline(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  randomnormal(mx,my,var);
  var/=2.;
  drawdotsymmetric(mx,my,5,1.);
  dx=mx-x1;
  dy=my-y1;
  if(dx*dx+dy*dy>=dist) drawline(x1,y1,mx,my,var,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist) drawline(mx,my,x2,y2,var,dist);
}


void paintlines_layer_generator::drawsmoothline(double var, double steps)
{
  double x, y;
  x=random_int(size());
  y=random_int(size());
  double vx(0.), vy(0.);
  randomnormal(vx,vy,var*sqrt(steps));
  while(steps--) {
    randomnormal(vx,vy,var);
    x+=vx;
    y+=vy;
    drawdotsymmetric(x,y,5,1.);
  }
}

void paintlines_layer_generator::drawsmoothline2(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  randomnormal(mx,my,var);
  var/=4.;
  drawdotsymmetric(mx,my,5,1.);
  dx=mx-x1;
  dy=my-y1;
  if(dx*dx+dy*dy>=dist) drawsmoothline2(x1,y1,mx,my,var,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist) drawsmoothline2(mx,my,x2,y2,var,dist);
}

void paintlines_layer_generator::drawsmoothline3(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  randomnormal(mx,my,var);
  var/=2.;
  drawdotsymmetric(mx,my,5,1.);
  dx=mx-x1;
  dy=my-y1;
  if(dx*dx+dy*dy>=dist)
    while(random_bool()) drawsmoothline3(x1,y1,mx,my,var/4.,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist)
    while(random_bool()) drawsmoothline3(mx,my,x2,y2,var/4.,dist);
}

void paintlines_layer_generator::drawsmoothline4(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  randomnormal(mx,my,var);
  var/=2.;
  double a=15.*sqrt(var);
  if(a<50.) a=50.;
  int s=sqrt(a);
  drawdotsymmetric(mx,my,s,a/125.);
  dx=mx-x1;
  dy=my-y1;
  if(dx*dx+dy*dy>=dist) drawsmoothline4(x1,y1,mx,my,var/4.,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist) drawsmoothline4(mx,my,x2,y2,var/4.,dist);
}

void paintlines_layer_generator::drawsmoothline5(double x1, double y1, double dx1, double dy1,
	double x2, double y2, double dx2, double dy2, double var, double dist)
{
	double mx = (x1+x2)*.5+(dx1-dx2)*.25, my = (y1+y2)*.5+(dy1-dy2)*.25;
	double mdx = .375*(x2-x1)-.125*(dx1+dx2), mdy = .375*(y2-y1)-.125*(dy1+dy2);
	randomnormal(mx,my,var);
	randomnormal(mdx,mdy,var*.75);
	dx1/=2.; dy1/=2.; dx2/=2.; dy2/=2.;
	var/=8.;
	drawdotsymmetric(mx,my,5,1.);
	double dx=mx-x1, dy=my-y1;
	if(dx*dx+dy*dy>=dist) drawsmoothline5(x1,y1,dx1,dy1,mx,my,mdx,mdy,var,dist);
	dx=mx-x2;
	dy=my-y2;
	if(dx*dx+dy*dy>=dist) drawsmoothline5(mx,my,mdx,mdy,x2,y2,dx2,dy2,var,dist);
}

void paintlines_layer_generator::drawsmootharc(double x1, double y1, double x2, double y2,
			       double k, double var, double dist)
{
  double mx=(x1+x2)/2+k*(y2-y1), my=(y1+y2)/2+k*(x1-x2), dx, dy;
  randomnormal(mx,my,var);
  var/=4.;
  drawdotsymmetric(mx,my,5,1.);
  dx=mx-x1;
  dy=my-y1;
  if(dx*dx+dy*dy>=dist) drawsmootharc(x1,y1,mx,my,k/2.,var,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist) drawsmootharc(mx,my,x2,y2,k/2.,var,dist);
}

void paintlines_layer_generator::drawflower(double x, double y, double var, int steps)
{
  int n=3.+random_exponential(10);
  int i;
  double offset=random_uniform();
  for(i=0.;i<n;i++)
  {
    double xn(x), yn(y);
    double vx(cos(2.*M_PI*(i+offset)/n)), vy(sin(2.*M_PI*(i+offset)/n));
    int st(steps);
    randomnormal(vx,vy,.01);
    while(st--) {
      randomnormal(vx,vy,.01);
      xn+=vx;
      yn+=vy;
      drawdotsymmetric(xn,yn,5,1.);
    }
  }
}


void paintlines_layer_generator::drawtriangle(double x1, double y1, double x2, double y2,
			      double x3, double y3, double var, double dist)
{
  double mx3((x1+x2)/2.), my3((y1+y2)/2.), mx1((x2+x3)/2.), my1((y2+y3)/2.),
    mx2((x3+x1)/2.), my2((y3+y1)/2.);
  randomnormal(mx1,my1,var);
  randomnormal(mx2,my2,var);
  randomnormal(mx3,my3,var);
  if(mx1*mx1+my1*my1+mx2*mx2+my2*my2+mx3*mx3+my3*my3-mx1*mx2-mx2*mx3-mx3*mx1-
     my1*my2-my2*my3-my3*my1>dist) {
    drawtriangle(mx1,my1,mx2,my2,mx3,my3,var/2.,dist);
    if(random_bool()) drawsmoothline2(x1,y1,mx2,my2,var/2.,dist);
    if(random_bool()) drawsmoothline2(x1,y1,mx3,my3,var/2.,dist);
    if(random_bool()) drawsmoothline2(x2,y2,mx1,my1,var/2.,dist);
    if(random_bool()) drawsmoothline2(x2,y2,mx3,my3,var/2.,dist);
    if(random_bool()) drawsmoothline2(x3,y3,mx1,my1,var/2.,dist);
    if(random_bool()) drawsmoothline2(x3,y3,mx2,my2,var/2.,dist);
  }
}

static const double swirl_eps = .005;

void paintlines_layer_generator::drawswirl() {
	double l = random_exponential(1.);
	double k1 = random_normal(3.), k2 = random_normal(3.);
	double q = random_angle();
	double x = random_uniform()*size(), y = random_uniform()*size();
	double t;
	for(t=0;t<l;t+=swirl_eps) {
		drawdotsymmetric(x,y,5,1.);
		q+=swirl_eps*(t*k1+(l-t)*k2)/l;
		x+=cos(q);
		y+=sin(q);
	}
}

void paintlines_layer_generator::drawopenstring()
{
	double sigma=random_exponential(size()*.1);
	double var=sigma*sigma;
	int i = random_int(size()), j = random_int(size());
	double x2=i,y2=j;
	randomnormal(x2,y2,var);
	drawline(i,j,x2,y2,var/4.,1.);
}

static const double orbit_stdev = sqrt(1.5);

void paintlines_layer_generator::draworbit() {
	double q[3][2], dq[3][2];
	double t;
	int i, j;
	for(i=0;i<3;i++)
		for(j=0;j<2;j++) {
			q[i][j]=random_angle();
			dq[i][j]=random_normal(orbit_stdev);
		}
	for(t=12./num_symmetries[group()];t>=0;t-=swirl_eps) {
		for(i=0;i<3;i++) {
			drawdotsymmetric(q[i][0]*size()/(2*M_PI),q[i][1]*size()/(2*M_PI),5,1.);
		}
		for(i=1;i<3;i++)
			for(j=0;j<i;j++) {
				double dx = q[i][0]-q[j][0];
				double dy = q[i][1]-q[j][1];
				double d = 2.-cos(dx)-cos(dy);
				double sx = sin(dx)*swirl_eps/d;
				double sy = sin(dy)*swirl_eps/d;
				dq[i][0]-=sx;
				dq[i][1]-=sy;
				dq[j][0]+=sx;
				dq[j][1]+=sy;
			}
		for(i=0;i<3;i++)
			for(j=0;j<2;j++) {
				q[i][j]+=dq[i][j]*swirl_eps;
			}
	}
}

void paintlines_layer_generator::drawtree()
{
	drawtree_split(random_int(size()),random_int(size()),random_angle(),0);
}

void paintlines_layer_generator::drawtree(double x, double y, double q, int depth)
{
	int n = random_poisson(20);
	double vx = cos(q);
	double vy = sin(q);
	while(--n>=0) {
		x+=vx;
		y+=vy;
		depth++;
		drawdotsymmetric(x,y,5,1);
	}
	if(random_bernoulli(1/(1+depth/100.)))
		drawtree_split(x,y,q,depth);
}

void paintlines_layer_generator::drawtree_split(double x, double y, double q, int depth)
{
	double dq = random_normal(M_PI/6.);
	drawtree(x,y,q+dq,depth);
	drawtree(x,y,q-dq,depth);
}

void paintlines_layer_generator::drawfractal(int x, int y, int d, int prob)
{
	if(d) {
		d/=2;
		if(random_int(60)<prob) drawfractal(x,y,d,prob);
		if(random_int(60)<prob) drawfractal(x+d,y,d,prob);
		if(random_int(60)<prob) drawfractal(x,y+d,d,prob);
		if(random_int(60)<prob) drawfractal(x+d,y+d,d,prob);
	}
	else drawdotsymmetric(x,y,5,1.);
}
