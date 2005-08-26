/***************************************************************************
 *   Copyright (C) 2005 by Daniel Gulotta                                  *
 *   dgulotta@mit.edu                                                      *
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

#include <iostream>
using namespace std;

hyperbolic_coord random_mid(const hyperbolic_coord &c1,
			    const hyperbolic_coord &c2, double var)
{
  hyperbolic_coord mid(normalize(c1+c2));
  hyperbolic_coord diff(normalize(c1-c2));
  hyperbolic_coord norm(cross(mid,diff));
  double z=1.-var*log((double)rand()/RAND_MAX);
  double r=sqrt(z*z-1.);
  double q=(2.*M_PI*rand())/RAND_MAX;
  return normalize(z*mid+r*cos(q)*diff+r*sin(q)*norm);
}

void hyperbolic_paintlines::paint(int sz, hyperbolic_symmetry_group &sym)
{
  hyperbolic_painter::paint(sz,sym);
  last.resize(size*size);
  alpha.resize(size*size);
  red_brushes.resize(ncolors);
  green_brushes.resize(ncolors);
  blue_brushes.resize(ncolors);
  fill(red.begin(),red.end(),0);
  fill(green.begin(),green.end(),0);
  fill(blue.begin(),blue.end(),0);
  fill(last.begin(),last.end(),-1);
  unsigned char temp;
  int k;
  double dist;
  switch(pt) {
  case POINCARE:
    proj=&poincare_projection;
    drawdot=&hyperbolic_paintlines::drawdot_poincare;
    dist=1.+.01/size;
    break;
  case KLEIN:
    proj=&klein_projection;
    drawdot=&hyperbolic_paintlines::drawdot_klein;
    dist=1.+.005/size;
  }
  t=ncolors;
  while(t--) {
    k=rand();
    temp=k&255;
    k>>=8;
    switch(k%6) {
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
    hyperbolic_coord c;
    hyperbolic_coord c2;
    do {
      double z=1.-log((double)rand()/RAND_MAX);
      double r=sqrt(z*z-1.);
      double q=(2.*M_PI*rand())/RAND_MAX;
      c=hyperbolic_coord(r*cos(q),r*sin(q),z);
      c2=sg.random_symmetry(c,rand()%3);
    } while(c.z>=25.||c2.z>=25.);
    k/=6;
    drawsmoothline(c,c2,.02,dist);
  }
  int x, y, i;
  for(x=0;x<size;x++)
    for(y=0;y<size;y++) {
      i=x+y*size;
      if(last[i]!=-1) {
        //if(pastel[last[i]]) {
	//  double a=alpha[i]/255.;
	//  red[i]+=a*(alpha[i]+(1-a)*red_brushes[last[i]]-red[i]);
	//  green[i]+=a*(alpha[i]+(1-a)*green_brushes[last[i]]-green[i]);
	//  blue[i]+=a*(alpha[i]+(1-a)*blue_brushes[last[i]]-blue[i]);
        //}  
	//else {
	  red[i]+=(alpha[i]/255.)*(red_brushes[last[i]]-red[i]);
	  green[i]+=(alpha[i]/255.)*(green_brushes[last[i]]-green[i]);
	  blue[i]+=(alpha[i]/255.)*(blue_brushes[last[i]]-blue[i]);
	//}
      }
    }
}

void hyperbolic_paintlines::drawdot_poincare(const hyperbolic_coord &hc)
{
  planar_coord pc=(*proj)(hc);
  double d=1.-(pc.x*pc.x+pc.y*pc.y);
  int r=radius*d;
  d=1./(d*d*brightness);
  int i,j;
  screen_coord sc=toscreen(pc);
  for(i=-r;i<=r;i++)
    for(j=-r;j<=r;j++) {
      drawpixel(sc.x+i,sc.y+j,255.99/(1.+d*(i*i+j*j)));
    }
}

void hyperbolic_paintlines::drawdot_klein(const hyperbolic_coord &hc)
{
  int i,j;
  if(!(hc.x||hc.y)) {
    int r=radius;
    int x=(size+1)/2;
    for(i=-r;i<r;i++)
      for(j=-r;j<r;j++) {
	drawpixel(x+i,x+j,255.99/(1.+(i*i+j*j)/brightness));
      }
  }
  else {
    planar_coord pc=(*proj)(hc);
    double n1=pc.x*pc.x+pc.y*pc.y;
    double d=1.-n1;
    n1=sqrt(n1);
    double c=pc.x/n1;
    double s=pc.y/n1;
    double a=1./(d*brightness);
    d=sqrt(d);
    int r=radius*d;
    d=1./(d*brightness);
    int i,j;
    screen_coord sc=toscreen(pc);
    for(i=-r;i<r;i++)
      for(j=-r;j<r;j++) {
	double dr=c*i-s*j;
	double dq=s*i+c*j;
	drawpixel(sc.x+i,sc.y+j,255.99/(1.+a*dr*dr+d*dq*dq));
      }
  }
}

void hyperbolic_paintlines::drawpixel(int x, int y, unsigned char myalpha)
{
  if(x>=0&&x<size&&y>=0&&y<size) {
    int i=x+y*size;
    if(last[i]==t) {
      if(myalpha>alpha[i]) alpha[i]=myalpha;
    }
    else {
      if(last[i]!=-1) {
	//if(pastel[last[i]]) {
	//double a=alpha[i]/255.;
	//red[i]+=a*(alpha[i]+(1-a)*red_brushes[last[i]]-red[i]);
	//green[i]+=a*(alpha[i]+(1-a)*green_brushes[last[i]]-green[i]);
	//blue[i]+=a*(alpha[i]+(1-a)*blue_brushes[last[i]]-blue[i]);
	//}
	//else {
	red[i]+=(alpha[i]/255.)*(red_brushes[last[i]]-red[i]);
	green[i]+=(alpha[i]/255.)*(green_brushes[last[i]]-green[i]);
	blue[i]+=(alpha[i]/255.)*(blue_brushes[last[i]]-blue[i]);
	//}
      }
    last[i]=t;
    alpha[i]=myalpha;
    }
  }
}

void hyperbolic_paintlines::drawsmoothline
(const hyperbolic_coord &end1, const hyperbolic_coord &end2, double var,
 double min)
{
  
  if(end1*end2>min) {
    hyperbolic_coord mid=random_mid(end1,end2,var);
    drawsmoothline(end1,mid,var/3.,min);
    sg.symmetrize(*this,drawdot,mid,8);
    drawsmoothline(mid,end2,var/3.,min);
  }
}
