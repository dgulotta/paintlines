/***************************************************************************
 *   Copyright (C) 2013, 2016 by Daniel Gulotta                            *
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
#include <chrono>
#include <random>

using namespace std;

struct seeded_rng : public default_random_engine {
	seeded_rng() : default_random_engine(std::random_device()()) {}
};

thread_local seeded_rng rnd;

int random_int(int n)
{
	return uniform_int_distribution<int>(0,n-1)(rnd);
}

int random_range_inclusive(int a, int b)
{
	return uniform_int_distribution<int>(a,b)(rnd);
}

int random_sign() {
	return 2*random_int(2)-1;
}

bool random_bool()
{
	return random_int(2)!=0;
}

double random_uniform()
{
	return uniform_real_distribution<double>(0,1)(rnd);
}

double random_angle()
{
	return 2.*M_PI*random_uniform();
}

double random_exponential(double mean)
{
	return mean*exponential_distribution<double>(1)(rnd);
}

double random_cauchy(double width)
{
	return cauchy_distribution<double>(0,width)(rnd);
}

double random_normal(double stdev)
{
	return normal_distribution<double>(0,stdev)(rnd);
}

double random_sechsquare(double width)
{
	double a = random_uniform();
	return width*log(a/(1-a));
}

double random_levy_skew_sqrt(double alpha) {
	if(alpha==1) return 1;
	double u, v;
	u = random_exponential(1.);
	v = M_PI*(random_uniform()-.5);
	// the usual normalization has a factor of pow(sec(alpha*pi/2),1/(2*alpha)),
	// but paintstripes rescales everything anyway, so it doesn't matter
	return sqrt(sin(alpha*(v+M_PI_2))) / pow(cos(v),1/(2*alpha))
		* pow(cos(v-alpha*(v+M_PI_2))/u,(1-alpha)/(2*alpha));
}

bool random_bernoulli(double p)
{
	return bernoulli_distribution(p)(rnd);
}

int random_poisson(double mu)
{
	return poisson_distribution<>(mu)(rnd);
}

double random_levy_1d(double alpha, double scale)
{
	double u, v, t, s;
	u = std::uniform_real_distribution<double>(-M_PI_2,M_PI_2)(rnd);
	v = std::exponential_distribution<double>(1)(rnd);
	t = scale*sin(alpha*u)/pow(cos(u),1/alpha);
	s = pow(cos((1-alpha)*u)/v,(1-alpha)/alpha);
	return t*s;
}
