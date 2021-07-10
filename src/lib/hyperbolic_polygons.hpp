/***************************************************************************
 *   Copyright (C) 2016 by Daniel Gulotta                                  *
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

#ifndef _HYPERBOLIC_POLYGONS_H
#define _HYPERBOLIC_POLYGONS_H

#include <vector>
#include "hyperbolic.hpp"

class hyperbolic_polygon
{
public:
	hyperbolic_polygon() {}
	hyperbolic_polygon(double a1, double a2, double a3);
	hyperbolic_polygon(double a1, double a2, double a3, double a4);
	hyperbolic_coord edge(int n) const;
	hyperbolic_coord vertex(int n) const;
	hyperbolic_transformation trans(int n1, int n2,bool flip) const;
	auto num_sides() const { return dirs.size(); }
	double cosh_radius() const { return rcosh; }
	double sinh_radius() const { return rsinh; }
	static hyperbolic_polygon regular_polygon(int num_sides, double a);
private:
	std::vector<planar_coord> dirs;
	double rcosh;
	double rsinh;
};

#endif
