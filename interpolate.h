/***************************************************************************
 *   Copyright (C) 2014 by Daniel Gulotta                                  *
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

#ifndef _INTERPOLATE_H
#define _INTERPOLATE_H

#include "canvas.h"
#include "color.h"

typedef std::vector<std::tuple<int,int,double>> interpolation_list;

color_t combine_colors(const wrap_canvas<color_t> &c, const interpolation_list &l);

interpolation_list interpolate_linear(double x, double y);

interpolation_list interpolate_cubic(double x, double y);

#endif
