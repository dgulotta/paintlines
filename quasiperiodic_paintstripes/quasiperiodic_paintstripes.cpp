/***************************************************************************
 *   Copyright (C) 2005-2008, 2013, 2016 by Daniel Gulotta                 *
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
#include <cmath>
#include <memory>
#include "../randgen.h"
#include "../fftw_cxx.h"

using std::unique_ptr;

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

canvas<color_t> paint_quasiperiodic_stripes(int size, int fftsize, double alpha)
{
	int halfsize2=fftsize/2+1;
	int fftsize2=2*halfsize2;
	unique_ptr<double [],fftw_free_deleter<double>> array(check_alloc(
		fftw_alloc_real(fftsize*fftsize*fftsize*fftsize2)));
	int sizes[4]={fftsize,fftsize,fftsize,fftsize};
	unique_ptr<std::remove_pointer_t<fftw_plan>,fftw_plan_deleter> plan(
		fftw_plan_dft_c2r(4,sizes,reinterpret_cast<fftw_complex *>(array.get()),array.get(),FFTW_ESTIMATE));
	canvas<color_t> image(size,size);
	for(auto col : { red_part, green_part, blue_part }) {
		array[0]=array[1]=0.;
		for(int a=0;a<fftsize;a++)
			for(int b=0;b<fftsize;b++)
				for(int c=0;c<fftsize;c++)
					for(int d=0;d<halfsize2;d++)
						if(a||b||c||d) {
							double norm=5.-cos((2.*M_PI*a)/fftsize)-cos((2.*M_PI*b)/fftsize)-
								cos((2.*M_PI*c)/fftsize)-cos((2.*M_PI*d)/fftsize)-
								cos((2.*M_PI*(a+b+c+d))/fftsize);
								random_levy_2d(array.get()+a*fftsize*fftsize*fftsize2+b*fftsize*
									fftsize2+c*fftsize2+d*2,alpha,pow(norm,-1-2/alpha));
						}
		fftw_execute(plan.get());
		double total=0.;
		for(int a=0;a<fftsize;a++)
			for(int b=0;b<fftsize;b++)
				for(int c=0;c<fftsize;c++)
					for(int d=0;d<fftsize;d++) {
						double r = array[a*fftsize*fftsize*fftsize2+b*fftsize*fftsize2+c*fftsize2+d];
						total+=r*r;
					}
		double scale=64*fftsize*fftsize/sqrt(total);
		for(int j=0;j<size;j++)
			for(int i=0;i<size;i++) {
				int a=mod((px[0]*i+py[0]*j)/2.,fftsize);
				int b=mod((px[1]*i+py[1]*j)/2.,fftsize);
				int c=mod((px[2]*i+py[2]*j)/2.,fftsize);
				int d=mod((px[3]*i+py[3]*j)/2.,fftsize);
				col(image(i,j))=colorchop(128.+array[a*fftsize*fftsize*fftsize2+b*fftsize*fftsize2+c*fftsize2+d]*scale);
		}
	}
	return image;
}
