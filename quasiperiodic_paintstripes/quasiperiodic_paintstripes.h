/***************************************************************************
 *   Copyright (C) 2005-2008 by Daniel Gulotta                             *
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

#ifndef _QUASIPERIODIC_PAINTSTRIPES_H
#define _QUASIPERIODIC_PAINTSTRIPES_H

#include "../painter.h"
#include <fftw3.h>

double random_levy_1d_power_alpha(double alpha, double scale);

void random_levy_2d(double *d,double alpha,double scale);

class quasiperiodic_paintstripes : virtual public basic_painter
{
 public:
  quasiperiodic_paintstripes() : fftplan(NULL), array(NULL),
    levy_alpha(1.0), fftsize(0) {}
  ~quasiperiodic_paintstripes() {
    if(fftplan) fftw_destroy_plan(fftplan);
    if(array) fftw_free((void *)array);
  }
  void paint(int sz);
  void set_alpha(double alpha) {levy_alpha=alpha;}
 private:
  void fill(vector<unsigned char> &r);
  fftw_plan fftplan;
  double *array;
  int fftsize;
  int fftsize2;
  int halfsize2;
  double sum;
  double dq;
  double levy_alpha;
};

#endif
