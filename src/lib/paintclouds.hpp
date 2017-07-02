/***************************************************************************
 *   Copyright (C) 2005-2007, 2013, 2016 by Daniel Gulotta                 *
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

#ifndef _PAINTCLOUDS_H
#define _PAINTCLOUDS_H

#include "color.hpp"
#include "hyperbolic_group.hpp"
#include "symmetric_canvas.hpp"

double clouds_rand_exp_cos(double a);
double clouds_rand_cauchy(double a);
double clouds_rand_normal(double a);
double clouds_rand_sechsquare(double a);

using clouds_randfunc = double (*)(double);

symmetric_canvas<color_t> paint_clouds(int size, symgroup sym, color_t col1, color_t col2,
	color_t col3,clouds_randfunc r);

canvas<color_t> paint_hyperbolic_clouds(int size, const fundamental_domain &fd,
	projtype pt, color_t col1, color_t col2, color_t col3,clouds_randfunc r);

std::vector<std::vector<fundamental_domain_family::side>> clouds_supported_groups();

#endif
