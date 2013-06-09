/***************************************************************************
 *   Copyright (C) 2005, 2013 by Daniel Gulotta                            *
 *   dgulotta@mit.edu                                                      *
 *   Portions copyright (C)  1996, 1997, 1998, 1999, 2000 James Theiler,   *
 *   Brian Gough                                                           *
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

#include "paintsquiggles.h"
#include <algorithm>
#include <math.h>

using std::copy;

/*
  This function is adapted from gsl_ran_levy function from the GNU Scientific
  Library.
*/
double random_levy_1d_power_alpha(double alpha,double scale)
{
  double u, v, t, s;
  u=(M_PI_2*rand())/RAND_MAX;
  v=log((RAND_MAX+1.)/(rand()+.5));
  t=pow(scale*sin(alpha*u),alpha)/cos(u);
  s=pow(cos((1-alpha)*u)/v,(1-alpha));
  return t*s;
}

void random_levy_2d(double *d,double alpha,double scale)
{
  if(scale==0.) {
    *d=0.;
    *(d+1)=0.;
  }
  else {
    double r=pow(random_levy_1d_power_alpha(alpha,scale)+
		 random_levy_1d_power_alpha(alpha,scale),1./alpha);
    double q=(rand()*(2.*M_PI))/RAND_MAX;
    *d=r*cos(q);
    *(d+1)=r*sin(q);
  }
}

void paintsquiggles::paint(int sz, symgroup sym)
{
  size2=sz+2;
  if(sz!=size) {
    if(array) fftw_free(array);
    array=(double *)fftw_malloc(sizeof(double)*sz*size2);
    if(fftplan) fftw_destroy_plan(fftplan);
    fftplan=fftw_plan_dft_c2r_2d(sz,sz,(fftw_complex *)array,array,
				 FFTW_MEASURE);
  }
  painter::paint(sz,sym);
  switch(sym) {
  case SYM_P3:
  case SYM_P31M:
  case SYM_P3M1:
  case SYM_P6:
  case SYM_P6M:
    norm=&paintsquiggles::norm_hexagonal;
    break;
  default:
    norm=&paintsquiggles::norm_square;
  }
  std::fill(red.begin(),red.end(),0);
  std::fill(green.begin(),green.end(),0);
  std::fill(blue.begin(),blue.end(),0);
  dq=M_PI/halfsize;
  int z;
  int n(n_colors);
  unsigned char temp;
  while(n--) {
    z=rand();
    temp=z&255;
    z>>=8;
    switch(z%6) {
    case 0:
      redbrush=255;
      greenbrush=temp;
      bluebrush=0;
      break;
    case 1:
      redbrush=0;
      greenbrush=255;
      bluebrush=temp;
      break;
    case 2:
      redbrush=temp;
      greenbrush=0;
      bluebrush=255;
      break;
    case 3:
      redbrush=temp;
      greenbrush=255;
      bluebrush=0;
      break;
    case 4:
      redbrush=0;
      greenbrush=temp;
      bluebrush=255;
      break;
    default:
      redbrush=255;
      greenbrush=0;
      bluebrush=temp;
    }
    fill();
  }
}

void paintsquiggles::fill()
{
  int i,j;
  for(i=0;i<=halfsize;i++)
    for(j=0;j<size;j++)
      random_levy_2d(array+2*i+j*size2,levy_alpha,(this->*norm)(i,j));
  fftw_execute(fftplan);
  function<void(int,int)> accum(symmetrize([&](int x, int y) {accumulate(x,y);}));
  function<void(int,int)> st(symmetrize([&](int x, int y) {set(x,y);}));
  enumerate([&](int x,int y) {
  	sum=0.;
	accum(x,y);
	st(x,y);
  });
  double norm(0.);
  for(i=0;i<size;i++)
    for(j=0;j<size;j++) {
      double d=array[i+size2*j];
      norm+=d*d;
    }
  norm=sqrt(norm)/(size*64);
  double height, alpha;
  for(i=0;i<size;i++)
    for(j=0;j<size;j++) {
      height=array[i+size2*j]/norm;
      alpha=1./(pow(abs(height)/(10.*thickness),sharpness)+1.);
      red[i+size*j]=((1-alpha)*red[i+size*j]+alpha*redbrush);
      green[i+size*j]=((1-alpha)*green[i+size*j]+alpha*greenbrush);
      blue[i+size*j]=((1-alpha)*blue[i+size*j]+alpha*bluebrush);
    }
}
