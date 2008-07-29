/***************************************************************************
 *   Copyright (C) 2005-2008 by Daniel Gulotta                             *
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

#include "quasiperiodic_paintstripes.h"
#include <algorithm>
#include <math.h>

const double unitlength=30.;
const double px[4] = {1.,.30901699437494742411,-.80901699437494742409,
		      -.80901699437494742409};
const double py[4] = {0.,.95105651629515357211,.58778525229247312918,
		      -.58778525229247312918};

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

void quasiperiodic_paintstripes::paint(int sz, symgroup sym)
{
  int newfftsize=sqrt(sz);
  if(newfftsize!=fftsize) {
    fftsize=newfftsize;
    halfsize2=newfftsize/2+1;
    fftsize2=2*halfsize2;
    if(array) fftw_free(array);
    array=(double *)fftw_malloc(sizeof(double)*fftsize*fftsize*fftsize*
				fftsize2);
    if(fftplan) fftw_destroy_plan(fftplan);
    int sizes[4]={fftsize,fftsize,fftsize,fftsize};
    fftplan=fftw_plan_dft_c2r(4,sizes,(fftw_complex*)array,array,FFTW_MEASURE);
  }
  painter::paint(sz,sym);
  dq=M_PI/halfsize;
  fill(red);
  fill(green);
  fill(blue);
}

#include <stdio.h>

void quasiperiodic_paintstripes::fill(vector<unsigned char> &arr)
{
  double mag[2],  n(0.);
  double norm;
  int a,b,c,d, i, j;
  array[0]=array[1]=0.;
  for(a=0;a<fftsize;a++)
    for(b=0;b<fftsize;b++)
      for(c=0;c<fftsize;c++)
	for(d=0;d<halfsize2;d++)
	  if(a||b||c||d) {
	    norm=5.-cos((2.*M_PI*a)/fftsize)-cos((2.*M_PI*b)/fftsize)-
	      cos((2.*M_PI*c)/fftsize)-cos((2.*M_PI*d)/fftsize)-
	      cos((2.*M_PI*(a+b+c+d))/fftsize);
	    random_levy_2d(array+a*fftsize*fftsize*fftsize2+b*fftsize*fftsize2+
			   c*fftsize2+d*2,levy_alpha,1./(norm*norm));
	  }
  fftw_execute(fftplan);
  for(i=0;i<size;i++)
    for(j=0;j<size;j++) {
      a=mod((px[0]*i+py[0]*j)/2.,fftsize);
      b=mod((px[1]*i+py[1]*j)/2.,fftsize);
      c=mod((px[2]*i+py[2]*j)/2.,fftsize);
      d=mod((px[3]*i+py[3]*j)/2.,fftsize);
      arr[i+j*size]=colorchop(128.+6000.*levy_alpha*array[a*fftsize*fftsize*fftsize2+b*fftsize*fftsize2+c*fftsize2+d]/(size*size));
      /*
      arr[i+j*size]=255.99/
	(1.+.0005*pow(fabs(array[a*fftsize*fftsize*fftsize2+b*fftsize*
				 fftsize2+c*fftsize2+d]),2));
      */
    }
}
