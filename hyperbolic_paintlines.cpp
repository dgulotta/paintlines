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

void hyperbolic_paintlines::paint(int sz, hyperbolic_symmetry_group &sym)
{
  hyperbolic_painter::paint(sz,sym);
  fill(red.begin(),red.end(),0);
  fill(green.begin(),green.end(),0);
  fill(blue.begin(),blue.end(),0);
  switch(pt) {
  case POINCARE:
    proj=&poincare_projection;
    drawdot=&hyperbolic_paintlines::drawdot_poincare;
    break;
  case KLEIN:
    proj=&klein_projection;
    drawdot=&hyperbolic_paintlines::drawdot_klein;
  }
  sg.symmetrize(*this,drawdot,
		hyperbolic_coord(0,0,1.),5);
  sg.symmetrize(*this,drawdot,
		hyperbolic_coord(5./12.,0,13./12.),5);
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
    int r=radius/d;
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

void hyperbolic_paintlines::drawpixel(int x, int y, unsigned char alpha)
{
  if(x>=0&&x<size&&y>=0&&y<size) {
    int i=x+y*size;
    if(red[i]<alpha) red[i]=green[i]=blue[i]=alpha;
  }
}
