/***************************************************************************
 *   Copyright (C) 2005 by Daniel Gulotta                             *
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

#include "painter.h"

class paintclouds : public painter
{
 public:
  void paint(int size, symgroup sg);
  void set_colors(unsigned char r1, unsigned char b1, unsigned char g1,
		  unsigned char r2, unsigned char b2, unsigned char g2,
		  unsigned char r3, unsigned char b3, unsigned char g3)
    {red1=r1;red2=r2;red3=r3;blue1=b1;blue2=b2;blue3=b3;green1=g1;green2=g2;
    green3=g3;}
 private:
  void paint_border(int x1, int y1, int x2, int y2);
  void paint_border0(int x1, int y1, int x2, int y2);
  void paint_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
  unsigned char red1, red2, red3, blue1, blue2, blue3, green1, green2, green3;
};

#endif
