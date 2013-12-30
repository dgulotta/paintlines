/***************************************************************************
 *   Copyright (C) 2003-2005, 2013 by Daniel Gulotta                       *
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

void randomnormal_hexagonal(double &x, double &y, double var)
{
  double v = sqrt(var/2);
  double s=random_normal(v)*.93060485910209959893;
  double d=random_normal(v)*.53728496591177095978;
  x+=s+d;
  y+=s-d;
}

void randomnormal_orthogonal(double &x, double &y, double var)
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
	switch(sym) {
		case SYM_P3:
		case SYM_P3M1:
		case SYM_P31M:
		case SYM_P6:
		case SYM_P6M:
			randomnormal=&randomnormal_hexagonal;
			break;
		default:
			randomnormal=&randomnormal_orthogonal;
	}
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
	merge(const_cast<canvas<color_t> &>(image.as_canvas()),layers);
}

void paintlines::handle_rule(ruletype rt)
{
	int i(random_int(size)),j(random_int(size));
	switch(rt) {
		case RULE_SMOOTH_ARC:
			drawsmootharc(i,j,i+random_range_inclusive(-2,2)*size, j+random_range_inclusive(-2,2)*size,.8,2000.,1.);
			break;
		case RULE_SMOOTHLINE2_BEADS:
			drawsmoothline2(i,j,i+random_range_inclusive(-2,2)*size, j+random_range_inclusive(-2,2)*size,25000.,100.);
			break;
		case RULE_SMOOTHLINE2:
			drawsmoothline2(i,j,i+random_range_inclusive(-2,2)*size, j+random_range_inclusive(-2,2)*size,25000.,1.);
			break;
		case RULE_CLUSTER:
			drawcluster(i,j,4000.,4);
			break;
		case RULE_FLOWER:
			drawflower(i,j,.1,50);
			break;
		case RULE_CLUSTER2:
			drawcluster5(i,j,size);
			break;
		case RULE_OPEN_STRING:
			{
				double sigma=random_exponential(size*.1);
				double var=sigma*sigma;
				double x2=i,y2=j;
				(*randomnormal)(x2,y2,var);
				drawline(i,j,x2,y2,var/4.,1.);
			}
			break;
		case RULE_SWIRL:
			do {
				drawswirl();
			} while(random_int(20)>=num_symmetries[sg]);
			break;
	}
}

void paintlines::drawdotsymmetric(int x, int y, int radius,double brightness)
{
  int i,j;
  double num, denom;
  switch(sg) {
  case SYM_P3:
  case SYM_P3M1:
  case SYM_P31M:
  case SYM_P6:
  case SYM_P6M:
    denom=brightness*4.33;
    num=brightness*1104.;
    for(i=-radius;i<=radius;i++)
      for(j=-radius;j<=radius;j++) {
	drawpixelsymmetric(x+i,y+j,num/(denom+i*i+i*j+j*j));
      }
    break;
  default:
    denom=brightness*5.;
    num=brightness*1275.;
    for(i=-radius;i<=radius;i++)
      for(j=-radius;j<=radius;j++) {
	drawpixelsymmetric(x+i,y+j,num/(denom+i*i+j*j));
      }
  } 
}

void paintlines::drawcluster(double x, double y, double var, int maxdepth)
{
  double mx=x,my=y;
  (*randomnormal)(mx,my,var);
  int z=random_int(maxdepth);
  while(z--) {
    drawsmoothline2(x,y,mx,my,var/4.,1.);
    drawcluster(mx,my,var/4.,maxdepth-1);
  }
  drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster2(int x, int y, int d)
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

void paintlines::drawcluster3(int x, int y, int d)
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

void paintlines::drawcluster4(int x, int y, int d)
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

void paintlines::drawcluster5(int x, int y, int d)
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


void paintlines::drawcluster6(int x, int y, int d)
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

void paintlines::drawcluster7(int x, int y, int d)
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

void paintlines::drawcluster8(int x, int y, int d)
{
  if(d) {
    d/=2;
    if(random_int(3)) drawcluster6(x,y,d);
    if(random_int(3)) drawcluster8(x+d,y+d,-d);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster9(int x, int y, int d)
{
  if(d) {
    if(random_int(3)) drawcluster9(x,y,d/2);
    if(random_int(3)) drawcluster9(x+d/6,y-d/3,d/2);
    if(random_int(3)) drawcluster9(x+d/3,y-d/6,d/2);
    if(random_int(3)) drawcluster9(x+d/2,y-d/2,d/2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster10(int x, int y, int d)
{
  if(d) {
    if(random_int(3)) drawcluster10(x,y,d/2);
    if(random_int(3)) drawcluster10(x+d/6,y-d/3,d/2);
    if(random_int(3)) drawcluster10(x+d/3,y-d/6,d/2);
    if(random_int(3)) drawcluster10(x+d/2,y-d/2,-d/2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster11(double x, double y, double var, int maxdepth,
		   double dist)
{
  double xx=x, yy=y;
  (*randomnormal)(x,y,var);
  int z=random_int(maxdepth);
  while(z--) {
    drawcluster11(x,y,var/2.,maxdepth-1,dist);
    drawsmoothline2(x,y,xx,yy,var,dist);
  }
}

void paintlines::drawcluster12(int x, int y, int d, unsigned char myalpha)
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

void paintlines::drawcluster13(double x, double y, double var, int maxdepth)
{
  (*randomnormal)(x,y,var);
  int z=random_int(maxdepth);
  while(z--) drawcluster13(x,y,var/2.,maxdepth-1);
  int s=sqrt(var);
  drawdotsymmetric(x,y,s,var/1275.);
}

void paintlines::drawcluster14(double x, double y, double var, int maxdepth)
{
  (*randomnormal)(x,y,var);
  int z=random_int(maxdepth);
  while(z--) drawcluster13(x,y,var/2.,maxdepth-1);
  double s=sqrt(var);
  drawcluster5(x-s/2.,y-s/2.,s);
}

void paintlines::drawcluster15(double x, double y, double m, double px,
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

void paintlines::drawcluster16(double x, double y, double x1, double y1,
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

void paintlines::drawcluster17(double x, double y, double x1, double y1,
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

void paintlines::drawcluster18(double x, double y, double x1, double y1,
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

void paintlines::drawcluster19(double x, double y, double x1, double y1,
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

void paintlines::drawcluster20(double x, double y, double x1, double y1,
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

void paintlines::drawline(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  (*randomnormal)(mx,my,var);
  var/=2.;
  drawdotsymmetric(mx,my,5,1.);
  dx=mx-x1;
  dy=my-y1;
  if(dx*dx+dy*dy>=dist) drawline(x1,y1,mx,my,var,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist) drawline(mx,my,x2,y2,var,dist);
}


void paintlines::drawsmoothline(double var, double steps)
{
  double x, y;
  x=random_int(size);
  y=random_int(size);
  double vx(0.), vy(0.);
  (*randomnormal)(vx,vy,var*sqrt(steps));
  while(steps--) {
    (*randomnormal)(vx,vy,var);
    x+=vx;
    y+=vy;
    drawdotsymmetric(x,y,5,1.);
  }
}

void paintlines::drawsmoothline2(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  (*randomnormal)(mx,my,var);
  var/=4.;
  drawdotsymmetric(mx,my,5,1.);
  dx=mx-x1;
  dy=my-y1;
  if(dx*dx+dy*dy>=dist) drawsmoothline2(x1,y1,mx,my,var,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist) drawsmoothline2(mx,my,x2,y2,var,dist);
}

void paintlines::drawsmoothline3(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  (*randomnormal)(mx,my,var);
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

void paintlines::drawsmoothline4(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  (*randomnormal)(mx,my,var);
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

void paintlines::drawsmootharc(double x1, double y1, double x2, double y2,
			       double k, double var, double dist)
{
  double mx=(x1+x2)/2+k*(y2-y1), my=(y1+y2)/2+k*(x1-x2), dx, dy;
  (*randomnormal)(mx,my,var);
  var/=4.;
  drawdotsymmetric(mx,my,5,1.);
  dx=mx-x1;
  dy=my-y1;
  if(dx*dx+dy*dy>=dist) drawsmootharc(x1,y1,mx,my,k/2.,var,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist) drawsmootharc(mx,my,x2,y2,k/2.,var,dist);
}

void paintlines::drawflower(double x, double y, double var, int steps)
{
  int n=3.+random_exponential(10);
  int i;
  double offset=random_uniform();
  for(i=0.;i<n;i++)
  {
    double xn(x), yn(y);
    double vx(cos(2.*M_PI*(i+offset)/n)), vy(sin(2.*M_PI*(i+offset)/n));
    int st(steps);
    (*randomnormal)(vx,vy,.01);
    while(st--) {
      (*randomnormal)(vx,vy,.01);
      xn+=vx;
      yn+=vy;
      drawdotsymmetric(xn,yn,5,1.);
    }
  }
}


void paintlines::drawtriangle(double x1, double y1, double x2, double y2,
			      double x3, double y3, double var, double dist)
{
  double mx3((x1+x2)/2.), my3((y1+y2)/2.), mx1((x2+x3)/2.), my1((y2+y3)/2.),
    mx2((x3+x1)/2.), my2((y3+y1)/2.);
  (*randomnormal)(mx1,my1,var);
  (*randomnormal)(mx2,my2,var);
  (*randomnormal)(mx3,my3,var);
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

void paintlines::drawswirl() {
	double l = random_exponential(1.);
	double k1 = random_normal(3.), k2 = random_normal(3.);
	double q = random_angle();
	double x = random_uniform()*size, y = random_uniform()*size;
	double t;
	for(t=0;t<l;t+=swirl_eps) {
		drawdotsymmetric(x,y,5,1.);
		q+=swirl_eps*(t*k1+(l-t)*k2)/l;
		x+=cos(q);
		y+=sin(q);
	}
}
