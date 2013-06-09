/***************************************************************************
 *   Copyright (C) 2005 by Daniel Gulotta                                  *
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

double random_levy_1d_power_alpha(double alpha, double scale);

void random_levy_2d(double *d,double alpha,double scale);

class paintsquiggles : virtual public painter
{
 public:
  paintsquiggles() : fftplan(NULL), array(NULL), size2(2), levy_alpha(1.0) {}
  ~paintsquiggles() {
    if(fftplan) fftw_destroy_plan(fftplan);
    if(array) fftw_free((void *)array);
  }
  void paint(int sz, symgroup sym);
  void set_alpha(double alpha) {levy_alpha=alpha;}
  void set_ncolors(int n) {n_colors=n;}
 private:
  void accumulate(int x, int y) {
    sum+=array[mod(x,size)+mod(y,size)*size2];
  }
  void set(int x, int y) {
    array[mod(x,size)+mod(y,size)*size2]=sum;
  }
  double norm_square(int x, int y) {
    if(x||y)
      return 1./(2.-cos(dq*x)-cos(dq*y));
    else return 0.;
  }
  double norm_hexagonal(int x, int y) {
    if(x||y)
      return 1./(3.-cos(dq*x)-cos(dq*y)-cos(dq*(x+y)));
    else return 0;
  }
  void fill();
  double (paintsquiggles::*norm)(int,int);
  fftw_plan fftplan;
  double *array;
  int size2;
  int n_colors;
  unsigned char redbrush, greenbrush, bluebrush;
  double sum;
  double dq;
  double levy_alpha;
};

#endif
