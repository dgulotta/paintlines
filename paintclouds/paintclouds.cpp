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

#include "paintclouds.h"

double randomnormal()
{
  return log(double(rand())/RAND_MAX)*cos((M_PI*rand())/RAND_MAX);
}

void paintclouds::paint(int sz, symgroup sym)
{
  painter::paint(sz,sym);
  drawfunc=get_sym_func<paintclouds>();
  switch(sym) {
  case SYM_CM_O:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,0,red2,green2,blue2);
    drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(halfsize,0,size,0);
    paint_border(0,0,halfsize,halfsize);
    paint_border(halfsize,halfsize,size,size);
    paint_triangle(0,0,size,0,size,size);
    break;
  case SYM_CMM_O:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,halfsize,red2,green2,blue2);
    drawpixelsymmetric(halfsize,0,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(0,0,halfsize,halfsize);
    paint_border(halfsize,halfsize,size,0);
    paint_triangle(0,0,size,0,halfsize,halfsize);
    break;
  case SYM_P1_O:
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
  case SYM_P2_O:
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
  case SYM_P4_O:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,halfsize,red2,green2,blue2);
    drawpixelsymmetric(halfsize,0,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(0,0,halfsize,halfsize);
    paint_triangle(0,0,size,0,halfsize,halfsize);
    break;
  case SYM_P4G_O:
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
  case SYM_P4M_O:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(halfsize,0,red2,green2,blue2);
    drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
    paint_border(0,0,halfsize,0);
    paint_border(0,0,halfsize,halfsize);
    paint_border(halfsize,0,halfsize,halfsize);
    paint_triangle(0,0,halfsize,0,halfsize,halfsize);
    break;
  case SYM_PGG_O:
    {
      int y1=(halfsize+1)/2;
      drawpixelsymmetric(-y1,y1,red1,green1,blue1);
      drawpixelsymmetric(y1,y1,red2,green2,blue2);
      drawpixelsymmetric(halfsize,halfsize,red3,green3,blue3);
      paint_border(-y1,y1,y1,y1);
      paint_border(y1,y1,halfsize,halfsize);
      paint_border(size-y1,size-y1,halfsize,halfsize);
      paint_triangle(-y1,size-y1,y1,y1,size-y1,size-y1);
    }
    break;
  case SYM_PM_O:
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
  case SYM_PMG_O:
    {
      int y1=(halfsize+1)/2;
      drawpixelsymmetric(0,y1,red1,green1,blue1);
      drawpixelsymmetric(halfsize-y1,halfsize,red2,green2,blue2);
      drawpixelsymmetric(halfsize-y1,0,red3,green3,blue3);
      paint_border(0,y1,0,size+y1);
      paint_border(0,y1,halfsize-y1,halfsize);
      paint_border(0,y1,halfsize-y1,0);
      paint_triangle(0,size+y1,0,y1,halfsize,halfsize+y1);
    }
    break;
  case SYM_PMM_O:
    drawpixelsymmetric(0,0,red1,green1,blue1);
    drawpixelsymmetric(0,halfsize,red2,green2,blue2);
    drawpixelsymmetric(halfsize,0,red3,green3,blue3);
    drawpixelsymmetric(halfsize,halfsize,red1,green1,blue1);
    paint_border(0,0,0,halfsize);
    paint_border(0,0,halfsize,0);
    paint_border(0,halfsize,halfsize,halfsize);
    paint_border(halfsize,0,halfsize,halfsize);
    paint_triangle(0,0,halfsize,0,0,halfsize);
    paint_triangle(0,halfsize,halfsize,0,halfsize,halfsize);
    break;
  }
}

void paintclouds::paint_border(int x1, int y1, int x2, int y2)
{
  int mx=(x1+x2)/2;
  int my=(y1+y2)/2;
  if(!((mx==x1||mx==x2)&&(my==y1||my==y2))) {
    double dx=x1-x2, dy=y1-y2;
    double norm=sqrt(dx*dx+dy*dy);
    drawpixelsymmetric
      (mx,my,colorchop(double(mi(red,x1,y1)+mi(red,x2,y2))/2.+norm*
		       randomnormal()+.5),
       colorchop(double(mi(green,x1,y1)+mi(green,x2,y2))/2.+norm*
		 randomnormal()+.5),
       colorchop(double(mi(blue,x1,y1)+mi(blue,x2,y2))/2.+norm*
		 randomnormal()+.5));
    paint_border(x1,y1,mx,my);
    paint_border(mx,my,x2,y2);
  }
}

void paintclouds::paint_triangle(int x1, int y1, int x2, int y2, int x3,
				 int y3)
{
  int area=x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2;
  // points must be in clockwise order or the area will be negative
  if(area>1) {
     int m1x=(x2+x3)/2, m1y=(y2+y3)/2, m2x=(x1+x3)/2, m2y=(y1+y3)/2;
     int m3x=(x1+x2)/2, m3y=(y1+y2)/2;
     paint_border(m1x,m1y,m2x,m2y);
     paint_border(m2x,m2y,m3x,m3y);
     paint_border(m3x,m3y,m1x,m1y);
     paint_triangle(x1,y1,m3x,m3y,m2x,m2y);
     paint_triangle(m3x,m3y,x2,y2,m1x,m1y);
     paint_triangle(m2x,m2y,m1x,m1y,x3,y3);
     paint_triangle(m1x,m1y,m2x,m2y,m3x,m3y);
  }
}
