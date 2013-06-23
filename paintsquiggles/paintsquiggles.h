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

#ifndef _PAINTSTRIPES_H
#define _PAINTSTRIPES_H

#include "../painter.h"
#include <fftw3.h>

class stripes_grid;

class paintsquiggles : virtual public painter
{
 public:
  paintsquiggles() : levy_alpha(1.0), thickness(1.), sharpness(2.) {}
  void paint(int sz, symgroup sym);
  void set_alpha(double alpha) {levy_alpha=alpha;}
  void set_ncolors(int n) {n_colors=n;}
  void set_thickness(double t) {thickness=t;}
  void set_sharpness(double s) {sharpness=s;}
 private:
  void fill(const stripes_grid &grid);
  int n_colors;
  unsigned char redbrush, greenbrush, bluebrush;
  double levy_alpha;
  double thickness;
  double sharpness;
};

#endif
