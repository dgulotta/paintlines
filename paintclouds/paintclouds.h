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

#ifndef _PAINTCLOUDS_H
#define _PAINTCLOUDS_H

#include "../painter.h"

double randomnormal();

class paintclouds : virtual public painter
{
 public:
  void paint(int size, symgroup sg);
  void set_colors(unsigned char r1, unsigned char g1, unsigned char b1,
		  unsigned char r2, unsigned char g2, unsigned char b2,
		  unsigned char r3, unsigned char g3, unsigned char b3)
    {red1=r1;red2=r2;red3=r3;blue1=b1;blue2=b2;blue3=b3;green1=g1;green2=g2;
    green3=g3;}
  void set_color1(unsigned char r, unsigned char g, unsigned char b)
    {red1=r;green1=g;blue1=b;}
  void set_color2(unsigned char r, unsigned char g, unsigned char b)
    {red2=r;green2=g;blue2=b;}
  void set_color3(unsigned char r, unsigned char g, unsigned char b)
    {red3=r;green3=g;blue3=b;}
 private:
  void (painter::*drawfunc)(paintclouds &,void (paintclouds::*)(int,int),
			    int,int);
  inline void drawpixelsymmetric(int x, int y, unsigned char r,
				 unsigned char g, unsigned char b) {
    tempr=r;
    tempg=g;
    tempb=b;
    (((painter *)this)->*drawfunc)(*this,&paintclouds::drawpixel,x,y);
  }
  void drawpixel(int x, int y) {
    mi(red,x,y)=tempr;
    mi(green,x,y)=tempg;
    mi(blue,x,y)=tempb;
  }
  void paint_border(int x1, int y1, int x2, int y2);
  void paint_border0(int x1, int y1, int x2, int y2);
  void paint_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
  unsigned char red1, red2, red3, blue1, blue2, blue3, green1, green2, green3;
  unsigned char tempr,tempg,tempb;
};

#endif
