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

#include <tuple>
#include "paintclouds.h"
#include "../randgen.h"

using std::tie;

inline int midpt(int a, int b) {
  int c=a+b;
  if(a<b) c++;
  if(c<0) c--;
  return c/2;
}

// I don't think this corresponds to any well-known distribution.
// I think I used to be using a normal distribution here, but
// decided that fatter tails made nicer pictures.
double paintclouds::rand_exp_cos(double a)
{
	return random_exponential(a)*cos(random_angle());
}

double paintclouds::rand_cauchy(double a)
{
	return .01*random_cauchy(a*a);
}

double paintclouds::rand_normal(double a)
{
	return 1.2*random_normal(a);
}

double paintclouds::rand_sechsquare(double a) {
	return .3*random_sechsquare(a);
}

void paintclouds::paint(int size, symgroup sym)
{
  grid=symmetric_canvas<color_t>(size,sym);
  int halfsize=size/2;
  switch(sym) {
  case SYM_CM:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,0,red2,green2,blue2);
    drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(halfsize,0,size,0);
    paint_border(0,0,halfsize,halfsize);
    paint_border(halfsize,halfsize,size,size);
    paint_triangle(0,0,size,0,size,size);
    break;
  case SYM_CMM:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,halfsize,red2,green2,blue2);
    drawpixelsymmetric(halfsize,0,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(halfsize,halfsize,0,0);
    paint_border(size,0,halfsize,halfsize);
    paint_triangle(0,0,size,0,halfsize,halfsize);
    break;
  case SYM_P1:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,0,red2,green2,blue2);
    drawpixelsymmetric(0,halfsize,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(halfsize,0,size,0);
    paint_border(0,0,0,halfsize);
    paint_border(0,halfsize,0,size);
    paint_border(0,size,size,0);
    paint_triangle(0,0,size,0,0,size);
    paint_triangle(0,size,size,0,size,size);
    break;
  case SYM_P2:
    drawpixelsymmetric(0,0,((int)red1+red2+red3)/3,
		       ((int)green1+green2+green3)/3,
		       ((int)blue1+blue2+blue3)/3);
    drawpixelsymmetric(halfsize,0,red1,green1,blue1);
    drawpixelsymmetric(0,halfsize,red2,green2,blue2);
    drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
    paint_border(0,0,0,halfsize);
    paint_border(0,0,halfsize,0);
    paint_border(size,0,halfsize,halfsize);
    paint_triangle(0,0,size,0,0,size);
    break;
  case SYM_P3:
    {
      int x1=(size-2)/3;
      drawpixelsymmetric(-1,1,red1,green1,blue1);
      drawpixelsymmetric(1,-1,red1,green1,blue1);
      drawpixelsymmetric(x1,x1,red2,green2,blue2);
      drawpixelsymmetric(size-x1,size-x1,red3,green3,blue3);
      paint_border(x1,x1,-1,size+1);
      paint_border(size+1,-1,size-x1,size-x1);
      paint_border(size-x1,size-x1,x1,x1);
      copy_border(x1,x1,size+1,-1,x1,x1,-1,size+1);
      copy_border(-1,size+1,size-x1,size-x1,size+1,-1,size-x1,size-x1);
      paint_triangle(size+1,-1,size-x1,size-x1,x1,x1);
      paint_triangle(size-x1,size-x1,x1,x1,-1,size+1);
    }
    break;
  case SYM_P31M:
    {
      int x1=(size-1)/3;
      drawpixelsymmetric(-1,1,red1,green1,blue1);
      drawpixelsymmetric(1,-1,red1,green1,blue1);
      drawpixelsymmetric(x1,x1,red2,green2,blue2);
      drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
      paint_border(x1,x1,-1,size+1);
      copy_border_backward(size+1,-1,x1,x1,x1,x1,-1,size+1);
      paint_border(-1,size+1,halfsize,halfsize);
      paint_border(halfsize,halfsize,size+1,-1);
      paint_triangle(-1,size+1,size+1,-1,x1,x1);
    }
    break;
  case SYM_P3M1:
    {
      int x1=(size-2)/3;
      drawpixelsymmetric(-1,1,red1,green1,blue1);
      drawpixelsymmetric(x1,x1,red2,green2,blue2);
      drawpixelsymmetric(size-x1,size-x1,red3,green3,blue3);
      paint_border(x1,x1,-1,size+1);
      paint_border(size+1,-1,size-x1,size-x1);
      paint_border(size-x1,size-x1,x1,x1);
      paint_triangle(size+1,-1,size-x1,size-x1,x1,x1);
    }
    break;
  case SYM_P4:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,halfsize,red2,green2,blue2);
    drawpixelsymmetric(halfsize,0,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(0,0,halfsize,halfsize);
    paint_triangle(0,0,size,0,halfsize,halfsize);
    break;
  case SYM_P4G:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,0,red2,green2,blue2);
    paint_border(0,0,halfsize,0);
    {
      int mx=halfsize/2,my=halfsize-mx;
      drawpixelsymmetric(mx,my,red3,green3,blue3);
      paint_border(halfsize,0,mx,my);
      paint_border(0,halfsize,mx,my);
    }
    paint_triangle(0,0,halfsize,0,0,halfsize);
    break;
  case SYM_P4M:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,0,red2,green2,blue2);
    drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(0,0,halfsize,halfsize);
    paint_border(halfsize,0,halfsize,halfsize);
    paint_triangle(0,0,halfsize,0,halfsize,halfsize);
    break;
  case SYM_P6:
    {
      int x1=(size-1)/3;
      drawpixelsymmetric(-1,1,red1,green1,blue1);
      drawpixelsymmetric(x1,x1,red2,green2,blue2);
      drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
      paint_border(x1,x1,-1,size+1);
      copy_border_backward(-1,size+1,size-x1,size-x1,-1,size+1,x1,x1);
      paint_border(x1,x1,halfsize,halfsize);
      paint_triangle(-1,size+1,size-x1,size-x1,x1,x1);
    }
    break;
  case SYM_P6M:
    {
      int x1=(size-2)/3;
      drawpixelsymmetric(-1,1,red1,green1,blue1);
      drawpixelsymmetric(x1,x1,red2,green2,blue2);
      drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
      paint_border(-1,size+1,x1,x1);
      paint_border(x1,x1,halfsize,halfsize);
      paint_border(halfsize,halfsize,-1,size+1);
      paint_triangle(-1,size+1,x1,x1,halfsize,halfsize);
    }
    break;
  case SYM_PG:
    {
      int x1=midpt(halfsize,0);
      drawpixelsymmetric(0,0,red1,green1,blue1);
      drawpixelsymmetric(x1,halfsize,red2,green2,blue2);
      drawpixelsymmetric(x1,0,red3,green3,blue3);
      paint_border(x1,halfsize,0,0);
      paint_border(halfsize,size,x1,halfsize);
      paint_border(0,0,x1,0);
      paint_border(x1,0,halfsize,0);
      paint_triangle(0,0,size,0,halfsize,size);
    }
    break;
  case SYM_PGG:
    {
      int y1=(halfsize+1)/2;
      drawpixelsymmetric(-y1,y1,red1,green1,blue1);
      drawpixelsymmetric(y1,y1,red2,green2,blue2);
      drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
      paint_border(y1,y1,halfsize+y1,y1);
      copy_border_backward(halfsize+y1,y1,size+y1,y1,
			   halfsize+y1,y1,y1,y1);
      paint_border(halfsize,halfsize,y1,y1);
      copy_border(size,halfsize,size-y1,size-y1,halfsize,halfsize,y1,y1);
      paint_border(size-y1,size-y1,halfsize,halfsize);
      copy_border(size+y1,y1,size,halfsize,size-y1,size-y1,halfsize,halfsize);
      paint_triangle(size-y1,size-y1,y1,y1,size+y1,y1);
    }
    break;
  case SYM_PM:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(0,halfsize,red2,green2,blue2);
    drawpixelsymmetric(halfsize,0,red3,green3,blue3);
    paint_border(0,0,0,halfsize);
    paint_border(0,halfsize,0,size);
    paint_border(halfsize,halfsize,halfsize,size+halfsize);
    paint_border(0,0,halfsize,halfsize);
    paint_border(halfsize,halfsize,0,size);
    paint_triangle(0,size,0,0,halfsize,halfsize);
    paint_triangle(halfsize,halfsize,halfsize,size+halfsize,0,size);
    break;
  case SYM_PMG:
    {
      int y1=(halfsize+1)/2;
      drawpixelsymmetric(0,y1,red1,green1,blue1);
      drawpixelsymmetric(halfsize-y1,halfsize,red2,green2,blue2);
      drawpixelsymmetric(halfsize-y1,0,red3,green3,blue3);
      paint_border(0,size+y1,0,y1);
      paint_border(halfsize-y1,halfsize,halfsize,halfsize+y1);
      copy_border_backward(0,y1,halfsize-y1,halfsize,
			   halfsize,halfsize+y1,halfsize-y1,halfsize);
      paint_border(y1,0,0,y1);
      paint_triangle(0,size+y1,0,y1,halfsize,halfsize+y1);
    }
    break;
  case SYM_PMM:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(0,halfsize,red2,green2,blue2);
    drawpixelsymmetric(halfsize,0,red3,green3,blue3);
    drawpixelsymmetric(halfsize,halfsize,red1,green1,blue1);
    paint_border(0,0,0,halfsize);
    paint_border(0,0,halfsize,0);
    paint_border(0,halfsize,halfsize,halfsize);
    paint_border(halfsize,0,halfsize,halfsize);
    paint_border(0,halfsize,halfsize,0);
    paint_triangle(0,0,halfsize,0,0,halfsize);
    paint_triangle(0,halfsize,halfsize,0,halfsize,halfsize);
    break;
  }
}

void paintclouds::paint_border(int x1, int y1, int x2, int y2)
{
  int mx=midpt(x1,x2);
  int my=midpt(y1,y2);
  short r1,g1,b1,r2,g2,b2;
  if(!((mx==x1||mx==x2)&&(my==y1||my==y2))) {
    double dx=x1-x2, dy=y1-y2;
    double norm=sqrt(dx*dx+dy*dy);
	tie(r1,g1,b1)=pixel(x1,y1).as_tuple();
	tie(r2,g2,b2)=pixel(x2,y2).as_tuple();
    drawpixelsymmetric(mx,my,
		colorchop((r1+r2)/2.+randfunc(norm)+.5),
    	colorchop((g1+g2)/2.+randfunc(norm)+.5),
    	colorchop((b1+b2)/2.+randfunc(norm)+.5));
    paint_border(x1,y1,mx,my);
    paint_border(mx,my,x2,y2);
  }
}

void paintclouds::copy_border(int dx1, int dy1, int dx2, int dy2,
			      int sx1, int sy1, int sx2, int sy2)
{
  int dmx=midpt(dx1,dx2);
  int dmy=midpt(dy1,dy2);
  if(!((dmx==dx1||dmx==dx2)&&(dmy==dy1||dmy==dy2))) {
    int smx=midpt(sx1,sx2);
    int smy=midpt(sy1,sy2);
    drawpixelsymmetric(dmx,dmy,pixel(smx,smy));
    copy_border(dx1,dy1,dmx,dmy,sx1,sy1,smx,smy);
    copy_border(dmx,dmy,dx2,dy2,smx,smy,sx2,sy2);
  }
}

void paintclouds::copy_border_backward(int dx1, int dy1, int dx2, int dy2,
				       int sx1, int sy1, int sx2, int sy2)
{
  int dmx=midpt(dx1,dx2);
  int dmy=midpt(dy1,dy2);
  if(!((dmx==dx1||dmx==dx2)&&(dmy==dy1||dmy==dy2))) {
    int smx=midpt(sx2,sx1);
    int smy=midpt(sy2,sy1);
    drawpixelsymmetric(dmx,dmy,pixel(smx,smy));
    copy_border_backward(dx1,dy1,dmx,dmy,sx1,sy1,smx,smy);
    copy_border_backward(dmx,dmy,dx2,dy2,smx,smy,sx2,sy2);
  }
}

void paintclouds::paint_triangle(int x1, int y1, int x2, int y2, int x3,
				 int y3)
{
  int area=x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2;
  if(area>1||area<-1) {
    int m1x=midpt(x2,x3), m1y=midpt(y2,y3);
    int m2x=midpt(x3,x1), m2y=midpt(y3,y1);
    int m3x=midpt(x1,x2), m3y=midpt(y1,y2);
    paint_border(m1x,m1y,m3x,m3y);
    paint_border(m2x,m2y,m1x,m1y);
    paint_border(m3x,m3y,m2x,m2y);
    paint_triangle(x1,y1,m3x,m3y,m2x,m2y);
    paint_triangle(m3x,m3y,x2,y2,m1x,m1y);
    paint_triangle(m2x,m2y,m1x,m1y,x3,y3);
    paint_triangle(m1x,m1y,m3x,m3y,m2x,m2y);
  }
}
