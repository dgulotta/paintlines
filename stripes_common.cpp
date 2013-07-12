/***************************************************************************
 *   Copyright (C) 2013 by Daniel Gulotta                                  *
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
 
#include "randgen.h"
#include "stripes_common.h"

using std::polar;

complex<double> stripes_grid::get_symmetric(int x, int y) const {
	complex<double> sum(0,0);
	paint->symmetrize([&](int a, int b) { sum+=(*this)(a,b); })(x,y);
	return sum;
}

function<double(int,int)> stripes_grid::norm_hexagonal() {
	double p=phase;
	return [p] (int x, int y) {
		if(x==0&&y==0) x=1;
		return 1./(3.-cos(p*x)-cos(p*y)-cos(p*(x-y)));
	};
}

function<double(int,int)> stripes_grid::norm_orthogonal() {
	double p=phase;
	return [p] (int x, int y) {
		if(x==0&&y==0) x=1;
		return 1./(2.-cos(p*x)-cos(p*y));
	};
}

double stripes_grid::intensity() const {
	double s=0;
	int i, j;
	for(i=0;i<size;i++)
		for(j=0;j<size;j++) {
			double d=get_symmetric(i,j).real();
			s+=d*d;
		}
	return s;
}

double random_levy_1d(double alpha, double scale)
{
	double u, v, t, s;
	u = (random_angle()-M_PI)/2;
	v = random_exponential(1);
	t = scale*sin(alpha*u)/pow(cos(u),1/alpha);
	s = pow(cos((1-alpha)*u)/v,(1-alpha)/alpha);
	return t*s;
}

double random_levy_1d_power_alpha(double alpha, double scale)
{
	double u, v, t, s;
	u = random_angle()/4;
	v = random_exponential(1);
	t = pow(scale*sin(alpha*u),alpha)/cos(u);
	s = pow(cos((1-alpha)*u)/v,(1-alpha));
	return t*s;
}

// This actually isn't correct, but it doesn't matter much.
complex<double> random_levy_2d(double alpha, double scale)
{
	if(scale==0.)
		return complex<double>(0,0);
	else {
		double r=pow(random_levy_1d_power_alpha(alpha,scale)+
			random_levy_1d_power_alpha(alpha,scale),1./alpha);
		double q=random_angle();
		return polar(r,q);
	}
}


