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

#ifndef _HYPERBOLIC_PAINTLINES_H
#define _HYPERBOLIC_PAINTLINES_H

#include "hyperbolic_painter.h"

class hyperbolic_paintlines : public hyperbolic_painter
{
 public:
  hyperbolic_paintlines() : radius(5.99), brightness(5.), n_colors(0) {}
  void paint(int sz, hyperbolic_symmetry_group &sym);
  void set_ncolors(int n) {n_colors=n;}
 private:
  void (hyperbolic_paintlines::*drawdot)(const hyperbolic_coord &);
  planar_coord (*proj)(const hyperbolic_coord &);
  void drawdot_poincare(const hyperbolic_coord &pc);
  void drawdot_klein(const hyperbolic_coord &pc);
  void drawpixel(int x, int y, unsigned char alpha);
  double radius;
  double brightness;
  int n_colors;
  //vector<unsigned char> red_brushes;
  //vector<unsigned char> green_brushes;
  //vector<unsigned char> blue_brushes;
  //vector<int> last;
  //vector<unsigned char> alpha;
  //vector<bool> pastel;
};

#endif
