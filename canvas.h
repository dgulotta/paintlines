/***************************************************************************
 *   Copyright (C) 2008, 2013 by Daniel Gulotta                            *
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

#ifndef _CANVAS_H
#define _CANVAS_H

#include <algorithm>
#include <functional>
#include <vector>

inline int mod(int x, int y) 
{
	int ans=x%y;
	return (ans<0)?(ans+y):ans;
}

inline int divide(int x, int y)
{
	return (x>=0)?(x/y):~((~x)/y);
}


template<typename T>
class canvas
{
public:
	canvas() : canvas(0,0) {}
	canvas(int w, int h) : _width(w), _height(h), data(w*h) {}
	canvas(int w, int h, const T &t) : _width(w), _height(h), data(w*h,t) {}
	canvas(int w, int h, const std::function<T()> &f) : _width(w), _height(h), data(w*h) {
		std::generate(data.begin(),data.end(),f);
	}
	T & operator () (int x, int y) {
		int index=x+y*_width;
		return data[index];
	}
	const T & operator () (int x, int y) const {
		int index=x+y*_width;
		return data[index];
	}
	int width() const { return _width; }
	int height() const { return _height; }
	typedef typename std::vector<T>::iterator iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.end(); }
protected:
	int _width, _height;
	std::vector<T> data;
};

template<typename T>
class wrap_canvas {
public:
	wrap_canvas() {}
	wrap_canvas(int w, int h) : data(w,h) {}
	wrap_canvas(int w, int h, const T &t) : data(w,h,t) {}
	wrap_canvas(int w, int h, const std::function<T()> &f) : data(w,h,f) {}
	int width() const { return data.width(); }
	int height() const { return data.height(); }
	T & operator () (int x, int y) { return data(mod(x,width()),mod(y,height())); }
	const T & operator () (int x, int y) const { return data(mod(x,width()),mod(y,height())); }
	canvas<T> & as_canvas() { return data; }
	const canvas<T> & as_canvas() const { return data; }
	typedef typename canvas<T>::iterator iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.end(); }
private:
	canvas<T> data;
};

#endif
