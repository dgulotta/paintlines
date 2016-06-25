/***************************************************************************
 *   Copyright (C) 2005-2008, 2013 by Daniel Gulotta                       *
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

#include "quasiperiodic_paintstripes.h"
#include <algorithm>
#include <cmath>
#include "../randgen.h"
#include "../stripes_common.h"

const double unitlength=30.;
const double px[4] = {1.,.30901699437494742411,-.80901699437494742409,
		      -.80901699437494742409};
const double py[4] = {0.,.95105651629515357211,.58778525229247312918,
		      -.58778525229247312918};

void random_levy_2d(double *d,double alpha,double scale)
{
	if(scale==0.) {
		d[0]=0;
		d[1]=0;
	}
	else {
		double r = random_levy_skew_sqrt(alpha/2);
		d[0] = r*random_normal(scale);
		d[1] = r*random_normal(scale);
	}
}

void quasiperiodic_paintstripes::paint(int sz, int fftsz)
{
	size=sz;
	if(fftsz!=fftsize) {
		fftsize=fftsz;
		halfsize2=fftsz/2+1;
		fftsize2=2*halfsize2;
		if(array) fftw_free(array);
		array=(double *)fftw_malloc(sizeof(double)*fftsize*fftsize*fftsize*
				fftsize2);
		if(fftplan) fftw_destroy_plan(fftplan);
		int sizes[4]={fftsize,fftsize,fftsize,fftsize};
		fftplan=fftw_plan_dft_c2r(4,sizes,(fftw_complex*)array,array,FFTW_MEASURE);
	}
	dq=2.*M_PI/size;
	image=canvas<color_t>(size,size);
	fill([&](int x, int y, uint8_t c) { image(x,y).red=c; });
	fill([&](int x, int y, uint8_t c) { image(x,y).green=c; });
	fill([&](int x, int y, uint8_t c) { image(x,y).blue=c; });
}

void quasiperiodic_paintstripes::fill(const std::function<void(int,int,uint8_t)> &set)
{
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
								c*fftsize2+d*2,levy_alpha,pow(norm,-1-2/levy_alpha));
					}
	fftw_execute(fftplan);
	norm=0.;
	for(a=0;a<fftsize;a++)
		for(b=0;b<fftsize;b++)
			for(c=0;c<fftsize;c++)
				for(d=0;d<fftsize;d++) {
					double r = array[a*fftsize*fftsize*fftsize2+b*fftsize*fftsize2+c*fftsize2+d];
					norm+=r*r;
				}
	norm = 64*fftsize*fftsize/sqrt(norm);
	for(j=0;j<size;j++)
		for(i=0;i<size;i++) {
			a=mod((px[0]*i+py[0]*j)/2.,fftsize);
			b=mod((px[1]*i+py[1]*j)/2.,fftsize);
			c=mod((px[2]*i+py[2]*j)/2.,fftsize);
			d=mod((px[3]*i+py[3]*j)/2.,fftsize);
			set(i,j,colorchop(128.+array[a*fftsize*fftsize*fftsize2+b*fftsize*fftsize2+c*fftsize2+d]*norm));
		}
}
