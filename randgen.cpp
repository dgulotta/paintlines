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
#include <chrono>
#include <random>

using namespace std;
using std::chrono::system_clock;

static default_random_engine rnd;

void seed() {
	rnd.seed(system_clock::now().time_since_epoch().count());
}

int random_int(int n)
{
	return uniform_int_distribution<int>(0,n-1)(rnd);
}

bool random_bool()
{
	return random_int(2)!=0;
}

double random_uniform()
{
	return uniform_real_distribution<double>(0,1)(rnd);
}
