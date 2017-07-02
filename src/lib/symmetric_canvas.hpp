/***************************************************************************
 *   Copyright (C) 2013, 2015 by Daniel Gulotta                            *
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

#ifndef _SYMMETRIC_CANVAS_H
#define _SYMMETRIC_CANVAS_H

#include "canvas.hpp"
#include "symmetry.hpp"
#include <algorithm>
#include <tuple>
#include <vector>

template<typename T>
class symmetric_canvas_ref;

template<typename T>
class symmetric_canvas
{
public:
	symmetric_canvas() : symmetric_canvas(0,symgroup::P1) {}
	symmetric_canvas(int s, symgroup g) : _group(g), base_canvas(s,s), _transforms(transformation<int>::group(g,s)) {}
	symmetric_canvas(int s, symgroup g, const T &t) : symmetric_canvas(s,g) {
		std::fill(base_canvas.begin(),base_canvas.end(),t);
	}
	const T & operator () (int x, int y) const { return get(x,y); }
	symmetric_canvas_ref<T> operator () (int x, int y) { return symmetric_canvas_ref<T>(*this,x,y); }
	const T & get(int x, int y) const { return base_canvas(x,y); }
	void set(int x, int y, const T &t) {
		point<int> p(x,y);
		int xn, yn;
		for(auto &f : _transforms) {
			std::tie(xn,yn) = f(p);
			base_canvas(xn,yn) = t;
		}
	}
	const wrap_canvas<T> & as_wrap_canvas() const { return base_canvas; }
	const canvas<T> & as_canvas() const { return base_canvas.as_canvas(); }
	int size() const { return base_canvas.height(); }
	symgroup group() const { return _group; }
	const std::vector<transformation<int>> & transforms() const { return _transforms; }
	void unsafe_set_symmetry_group(symgroup g) {
		_group = g;
		_transforms = transformation<int>::group(g,size());
	}
	canvas<T> & unsafe_get_canvas() { return base_canvas.as_canvas(); }
	void fill(const T &t) { std::fill(base_canvas.begin(),base_canvas.end(),t); }
private:
	symgroup _group;
	wrap_canvas<T> base_canvas;
	std::vector<transformation<int>> _transforms;
};

template<typename T>
class symmetric_canvas_ref {
public:
	symmetric_canvas_ref(symmetric_canvas<T> &_c, int _x, int _y) : c(_c), x(_x), y(_y) {}
	operator const T & () const { return c.get(x,y); }
	symmetric_canvas_ref & operator = (const T &t) { c.set(x,y,t); return *this; }
private:
	symmetric_canvas<T> &c;
	int x, y;
};

#endif
