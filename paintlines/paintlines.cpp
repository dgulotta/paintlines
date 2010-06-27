/***************************************************************************
 *   Copyright (C) 2003-2005 by Daniel Gulotta                             *
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
#include <algorithm>
#include <cmath>
#include <cstdlib>

using std::lower_bound;
using std::cos;
using std::sin;
using std::log;
using std::sqrt;
using std::rand;

void randomnormal_hexagonal(double &x, double &y, double var)
{
  double z=(double)rand()/RAND_MAX;
  double r=sqrt(-var*log(z));
  z=(2.*M_PI*rand())/RAND_MAX;
  double s=r*cos(z)*.93060485910209959893;
  double d=r*sin(z)*.53728496591177095978;
  x+=s+d;
  y+=s-d;
}

void randomnormal_orthogonal(double &x, double &y, double var)
{
  double z=(double)rand()/RAND_MAX;
  double r=sqrt(-var*log(z));
  z=(2.*M_PI*rand())/RAND_MAX;
  x+=r*cos(z);
  y+=r*sin(z);
}

void randomcauchy(double &x, double &y, double var)
{
  double z=(double)rand()/RAND_MAX;
  double r=var*sqrt(1./(z*z)-1.);
  z=(2.*M_PI*rand())/RAND_MAX;
  x+=r*cos(z);
  y+=r*sin(z);
}

void paintlines::generate_color(int t) {
    int z=rand();
    int temp=z&255;
    z>>=8;
    switch(z%6) {
    case 0:
      red_brushes[t]=255;
      green_brushes[t]=temp;
      blue_brushes[t]=0;
      break;
    case 1:
      red_brushes[t]=0;
      green_brushes[t]=255;
      blue_brushes[t]=temp;
      break;
    case 2:
      red_brushes[t]=temp;
      green_brushes[t]=0;
      blue_brushes[t]=255;
      break;
    case 3:
      red_brushes[t]=temp;
      green_brushes[t]=255;
      blue_brushes[t]=0;
      break;
    case 4:
      red_brushes[t]=0;
      green_brushes[t]=temp;
      blue_brushes[t]=255;
      break;
    default:
      red_brushes[t]=255;
      green_brushes[t]=0;
      blue_brushes[t]=temp;
    }
}

void paintlines::generate_color_bw(int t) {
	int z = rand()%255;
	red_brushes[t]=z;
	green_brushes[t]=z;
	blue_brushes[t]=z;
}

void paintlines::paint(int sz, symgroup sym)
{
  painter::paint(sz,sym);
  drawfunc=get_sym_func<paintlines>();
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
  last.resize(size*size);
  alpha.resize(size*size);
  int z;
  double d1, d2;
  t=ncolors;
  fill(red.begin(),red.end(),0);
  fill(green.begin(),green.end(),0);
  fill(blue.begin(),blue.end(),0);
  fill(last.begin(),last.end(),-1);
  red_brushes.resize(ncolors);
  green_brushes.resize(ncolors);
  blue_brushes.resize(ncolors);
  pastel.resize(ncolors);
  unsigned char temp;
  while(t--) {
  	generate_color(t);
    vector<paintrule>::iterator it=
	    lower_bound(rules.begin(),rules.end(),rand()%rules.back().weight+1);
    pastel[t]=it->pastel;
    handle_rule(it->type);
  }
  int x, y, i;
  for(x=0;x<size;x++)
    for(y=0;y<size;y++) {
      i=x+y*size;
      if(last[i]!=-1) {
        if(pastel[last[i]]) {
	  double a=alpha[i]/255.;
	  red[i]+=a*(alpha[i]+(1-a)*red_brushes[last[i]]-red[i]);
	  green[i]+=a*(alpha[i]+(1-a)*green_brushes[last[i]]-green[i]);
	  blue[i]+=a*(alpha[i]+(1-a)*blue_brushes[last[i]]-blue[i]);
        }  
	else {
	  red[i]+=(alpha[i]/255.)*(red_brushes[last[i]]-red[i]);
	  green[i]+=(alpha[i]/255.)*(green_brushes[last[i]]-green[i]);
	  blue[i]+=(alpha[i]/255.)*(blue_brushes[last[i]]-blue[i]);
	}
      }
    }
}

void paintlines::handle_rule(ruletype rt)
{
  int i(rand()%size),j(rand()%size);
  switch(rt) {
  case RULE_SMOOTH_ARC:
    drawsmootharc(i,j,i+(rand()%5-2)*size, j+(rand()%5-2)*size,.8,2000.,1.);
    break;
  case RULE_SMOOTHLINE2_BEADS:
    drawsmoothline2(i,j,i+(rand()%5-2)*size, j+(rand()%5-2)*size,25000.,100.);
    break;
  case RULE_SMOOTHLINE2:
    drawsmoothline2(i,j,i+(rand()%5-2)*size, j+(rand()%5-2)*size,25000.,1.);
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
      double sigma=size*.1*log((1.+RAND_MAX)/(1.+rand()));
      double var=sigma*sigma;
      double x2=i,y2=j;
      (*randomnormal)(x2,y2,var);
      drawline(i,j,x2,y2,var/4.,1.);
    }
    break;
  }
}

void paintlines::drawpixel(int x, int y)
{
  int i=mod(x,size)+mod(y,size)*size;
  if(last[i]==t) {
    if(tempalpha>alpha[i]) alpha[i]=tempalpha;
  }
  else {
    if(last[i]!=-1) {
      if(pastel[last[i]]) {
	double a=alpha[i]/255.;
	red[i]+=a*(alpha[i]+(1-a)*red_brushes[last[i]]-red[i]);
	green[i]+=a*(alpha[i]+(1-a)*green_brushes[last[i]]-green[i]);
	blue[i]+=a*(alpha[i]+(1-a)*blue_brushes[last[i]]-blue[i]);
      }
      else {
	red[i]+=(alpha[i]/255.)*(red_brushes[last[i]]-red[i]);
	green[i]+=(alpha[i]/255.)*(green_brushes[last[i]]-green[i]);
	blue[i]+=(alpha[i]/255.)*(blue_brushes[last[i]]-blue[i]);
      }
    }
    last[i]=t;
    alpha[i]=tempalpha;
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
  int z=rand()%maxdepth, i, j;
  while(z--) {
    drawsmoothline2(x,y,mx,my,var/4.,1.);
    drawcluster(mx,my,var/4.,maxdepth-1);
  }
  drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster2(int x, int y, int d)
{
  if(d) {
    int z=rand()&3;
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
    int z=rand()&3;
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
    int z=rand()&3;
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
    if(rand()%3) drawcluster5(x,y,d);
    if(rand()%3) drawcluster5(x+d,y,d);
    if(rand()%3) drawcluster5(x,y+d,d);
    if(rand()%3) drawcluster5(x+d,y+d,d);
  }
  else drawdotsymmetric(x,y,5,1.);
}


void paintlines::drawcluster6(int x, int y, int d)
{
  if(d) {
    d/=2;
    if(rand()%3) drawcluster6(x,y,d);
    if(rand()%3) drawcluster6(x+d,y,d);
    if(rand()%3) drawcluster6(x,y+d,d);
    if(rand()%3) drawcluster6(x+d,y+d,-d);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster7(int x, int y, int d)
{
  if(d) {
    int d2=d/2;
    if(rand()%3) drawcluster7(x,y,d2);
    if(rand()%3) drawcluster7(x+d2,y+d2,d2);
    if(rand()%3) drawcluster7(x-d2,y+d2,d2);
    if(rand()%3) drawcluster7(x,y+d,-d2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster8(int x, int y, int d)
{
  if(d) {
    d/=2;
    if(rand()%3) drawcluster6(x,y,d);
    if(rand()%3) drawcluster8(x+d,y+d,-d);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster9(int x, int y, int d)
{
  if(d) {
    if(rand()%3) drawcluster9(x,y,d/2);
    if(rand()%3) drawcluster9(x+d/6,y-d/3,d/2);
    if(rand()%3) drawcluster9(x+d/3,y-d/6,d/2);
    if(rand()%3) drawcluster9(x+d/2,y-d/2,d/2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster10(int x, int y, int d)
{
  if(d) {
    if(rand()%3) drawcluster10(x,y,d/2);
    if(rand()%3) drawcluster10(x+d/6,y-d/3,d/2);
    if(rand()%3) drawcluster10(x+d/3,y-d/6,d/2);
    if(rand()%3) drawcluster10(x+d/2,y-d/2,-d/2);
  }
  else drawdotsymmetric(x,y,5,1.);
}

void paintlines::drawcluster11(double x, double y, double var, int maxdepth,
		   double dist)
{
  double xx=x, yy=y;
  (*randomnormal)(x,y,var);
  int z=rand()%maxdepth, i, j;
  while(z--) {
    drawcluster11(x,y,var/2.,maxdepth-1,dist);
    drawsmoothline2(x,y,xx,yy,var,dist);
  }
}

void paintlines::drawcluster12(int x, int y, int d, unsigned char myalpha)
{
  if(d) {
    d/=2;
    int z=rand();
    z|=(z>>8)|(z>>16);
    drawcluster12(x,y,d,myalpha&z);
    z=rand();
    z|=(z>>8)|(z>>16);
    drawcluster12(x+d,y,d,myalpha&z);
    z=rand();
    z|=(z>>8)|(z>>16);
    drawcluster12(x,y+d,d,myalpha&z);
    z=rand();
    z|=(z>>8)|(z>>16);
    drawcluster12(x+d,y+d,d,myalpha&z);
  }
  else {
    drawpixelsymmetric(x,y,myalpha);
  }
}

void paintlines::drawcluster13(double x, double y, double var, int maxdepth)
{
  (*randomnormal)(x,y,var);
  int z=rand()%maxdepth, i, j;
  while(z--) drawcluster13(x,y,var/2.,maxdepth-1);
  int s=sqrt(var);
  drawdotsymmetric(x,y,s,var/1275.);
}

void paintlines::drawcluster14(double x, double y, double var, int maxdepth)
{
  (*randomnormal)(x,y,var);
  int z=rand()%maxdepth, i, j;
  while(z--) drawcluster13(x,y,var/2.,maxdepth-1);
  double s=sqrt(var);
  drawcluster5(x-s/2.,y-s/2.,s);
}

void paintlines::drawcluster15(double x, double y, double m, double px,
			       double py, int maxdepth)
{
  if(maxdepth) {
    double m1=m*double(rand())/RAND_MAX, m2=m*double(rand())/RAND_MAX;
    if(m1+m2>m) {
      m1=m-m1;
      m2=m-m2;
    }
    double ms=m*m, m1s=m1*m1, m2s=m2*m2;
    double p=sqrt((ms*ms+m1s*m1s+m2s*m2s-2*(ms*m1s+ms*m2s+m1s*m2s))/(4*ms));
    double theta=(M_PI*rand())/RAND_MAX;
    double px1=px+p*cos(theta);
    double py1=py+p*sin(theta);
    double px2=px-p*cos(theta);
    double py2=py-p*sin(theta);
    double t1=-.1*log(double(rand())/RAND_MAX);
    double t2=-.1*log(double(rand())/RAND_MAX);
    drawcluster15(x+px1*t1/m1,y+py1*t1/m1,m1,px1,py1,maxdepth-1);
    drawcluster15(x+px2*t2/m2,y+py2*t2/m2,m2,px2,py2,maxdepth-1);
  }
  else {
    int s=sqrt(m), i, j;
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
    if(rand()%3) drawcluster16(x,y,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x1n,y+y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x2n,y+y2n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x3n,y+y3n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x-x1n,y-y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x-x2n,y-y2n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x-x3n,y-y3n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
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
    if(rand()%3) drawcluster17(x,y,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x1n,y+y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
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
    if(rand()%3) drawcluster18(x,y,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x1n,y+y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x2n,y+y2n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
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
    if(rand()%3) drawcluster19(x,y,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x1n,y+y1n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x2n,y+y2n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
    if(rand()%3) drawcluster16(x+x3n,y+y3n,x1n,y1n,x2n,y2n,x3n,y3n,depth);
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
    if(rand()&1) drawcluster20(x,y,x1n,y1n,x2n,y2n,depth);
    if(rand()&1) drawcluster20(x+x1n,y+y1n,x1n,y1n,x2n,y2n,depth);
    if(rand()&1) drawcluster20(x+x2n,y+y2n,x1n,y1n,x2n,y2n,depth);
    if(rand()&1) drawcluster20(x-x1n,y-y1n,x1n,y1n,x2n,y2n,depth);
    if(rand()&1) drawcluster20(x-x2n,y-y2n,x1n,y1n,x2n,y2n,depth);
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
  x=rand()%size;
  y=rand()%size;
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
    while(rand()&1) drawsmoothline3(x1,y1,mx,my,var/4.,dist);
  dx=mx-x2;
  dy=my-y2;
  if(dx*dx+dy*dy>=dist)
    while(rand()&1) drawsmoothline3(mx,my,x2,y2,var/4.,dist);
}

void paintlines::drawsmoothline4(double x1, double y1, double x2, double y2,
			  double var, double dist)
{
  double mx=(x1+x2)/2, my=(y1+y2)/2, dx, dy;
  (*randomnormal)(mx,my,var);
  var/=2.;
  int i, j;
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
  int n=3.+10.*log((RAND_MAX+1.)/(rand()+1.));
  int i;
  double offset=double(rand())/RAND_MAX;
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
    if(rand()&1) drawsmoothline2(x1,y1,mx2,my2,var/2.,dist);
    if(rand()&1) drawsmoothline2(x1,y1,mx3,my3,var/2.,dist);
    if(rand()&1) drawsmoothline2(x2,y2,mx1,my1,var/2.,dist);
    if(rand()&1) drawsmoothline2(x2,y2,mx3,my3,var/2.,dist);
    if(rand()&1) drawsmoothline2(x3,y3,mx1,my1,var/2.,dist);
    if(rand()&1) drawsmoothline2(x3,y3,mx2,my2,var/2.,dist);
  }
}
