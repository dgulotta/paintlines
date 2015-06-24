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

#ifndef _RANDGEN_H
#define _RANDGEN_H

#include <random>
#include <vector>

void seed();

int random_int(int n);

int random_range_inclusive(int a, int b);

int random_sign();

bool random_bool();

double random_uniform();

double random_angle();

double random_exponential(double mean);

double random_cauchy(double width);

double random_normal(double stdev);

double random_sechsquare(double width);

template<typename RNG>
double random_levy_1d(double alpha, double scale, RNG &r)
{
	double u, v, t, s;
	u = std::uniform_real_distribution<double>(-M_PI_2,M_PI_2)(r);
	v = std::exponential_distribution<double>(1)(r);
	t = scale*sin(alpha*u)/pow(cos(u),1/alpha);
	s = pow(cos((1-alpha)*u)/v,(1-alpha)/alpha);
	return t*s;
}

double random_levy_1d(double alpha, double scale);

double random_levy_skew_sqrt(double alpha);

bool random_bernoulli(double p);

int random_poisson(double mu);

template<typename T>
T random_choice(const std::vector<T> &v) { return v[random_int(v.size())]; }

#endif
