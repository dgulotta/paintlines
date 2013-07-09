/***************************************************************************
 *   Copyright (C) 2005, 2013 by Daniel Gulotta                            *
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

#ifndef _HYPERBOLIC_PAINTLINES_H
#define _HYPERBOLIC_PAINTLINES_H

#include "../hyperbolic_painter.h"

hyperbolic_coord random_mid(const hyperbolic_coord &c1,
			    const hyperbolic_coord &c2, double var);

class hyperbolic_paintlines : virtual public hyperbolic_painter
{
 public:
  hyperbolic_paintlines() : radius(4.99), brightness(5.), sharpness(1.5), ncolors(0) {}
  void paint(int sz, hyperbolic_symmetry_group &sym);
  void set_ncolors(int n) {ncolors=n;}
  void set_thickness(double r, double b, double s) { radius=r; brightness=b; sharpness=s; }
 private:
  function<void(const hyperbolic_coord &)> drawdot_symmetric;
  void drawdot(const hyperbolic_coord & hc);
  void fill_poincare(int t);
  void fill_klein(int t);
  void fill_color(const vector<float> &mask, int t);
  planar_coord (*proj)(const hyperbolic_coord &);
  void drawsmoothline(const hyperbolic_coord &end1, 
		      const hyperbolic_coord &end2, double var, double min);
  double radius;
  double brightness;
  double sharpness;
  int ncolors;
  int t;
  vector<unsigned char> red_brushes;
  vector<unsigned char> green_brushes;
  vector<unsigned char> blue_brushes;
  vector<unsigned char> alpha;
  vector<bool> pastel;
};

#endif
