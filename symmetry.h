/***************************************************************************
 *   Copyright (C) 2013, 2015-2016 by Daniel Gulotta                       *
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

#ifndef _SYMMETRY_H
#define _SYMMETRY_H

template<typename T>
using point = std::tuple<T,T>;

enum class symgroup {CM, CMM, P1, P2, P3, P31M, P3M1,
	P4, P4G, P4M, P6, P6M, PG, PGG, PM,
	PMG, PMM};

static const int _num_symmetries[17] = { 2, 4, 1,
	2, 3, 6, 6, 4, 8, 8, 6, 12, 2, 4, 2, 4, 4};

inline int num_symmetries(symgroup g) { return _num_symmetries[static_cast<int>(g)]; }

static const bool _sym_is_hexagonal[17] = { false, false, false, false, true, true,
	true, false, false, false, true, true, false, false, false, false, false };

inline bool sym_is_hexagonal(symgroup g) { return _sym_is_hexagonal[static_cast<int>(g)]; }

template<typename T>
struct transformation {
	point<T> operator () (const point<T> &p) const {
		return std::make_tuple(
			xx*std::get<0>(p)+xy*std::get<1>(p)+x1,
			yx*std::get<0>(p)+yy*std::get<1>(p)+y1);
	}
	T xx, xy, x1, yx, yy, y1;
	static const transformation<T> id;
	static const transformation<T> rot60;
	static const transformation<T> rot90;
	static const transformation<T> rot120;
	static const transformation<T> rot180;
	static const transformation<T> rot240;
	static const transformation<T> rot270;
	static const transformation<T> rot300;
	static const transformation<T> flipH;
	static const transformation<T> flipV;
	static const transformation<T> flipD1;
	static const transformation<T> flipD2;
	static const transformation<T> flipD3;
	static const transformation<T> flipD4;
	static const transformation<T> flipD5;
	static const transformation<T> flipD6;
	static transformation<T> flipD1Off(T offset) { return {0,1,offset,1,0,offset}; }
	static transformation<T> flipD2Off(T offset) { return {0,-1,offset,-1,0,offset}; }
	static transformation<T> glideX(T glide, T offset) { return {1,0,glide,0,-1,offset}; }
	static transformation<T> glideY(T glide, T offset) { return {-1,0,offset,0,1,glide}; }
	static std::vector<transformation<T>> group(symgroup g, T sz);
};

template<typename T>
const transformation<T> transformation<T>::id{1,0,0,0,1,0};

template<typename T>
const transformation<T> transformation<T>::rot60{0,-1,0,1,1,0};

template<typename T>
const transformation<T> transformation<T>::rot90{0,-1,0,1,0,0};

template<typename T>
const transformation<T> transformation<T>::rot120{-1,-1,0,1,0,0};

template<typename T>
const transformation<T> transformation<T>::rot180{-1,0,0,0,-1,0};

template<typename T>
const transformation<T> transformation<T>::rot240{0,1,0,-1,-1,0};

template<typename T>
const transformation<T> transformation<T>::rot270{0,1,0,-1,0,0};

template<typename T>
const transformation<T> transformation<T>::rot300{1,1,0,-1,0,0};

template<typename T>
const transformation<T> transformation<T>::flipH{-1,0,0,0,1,0};

template<typename T>
const transformation<T> transformation<T>::flipV{1,0,0,0,-1,0};

template<typename T>
const transformation<T> transformation<T>::flipD1{0,1,0,1,0,0};

template<typename T>
const transformation<T> transformation<T>::flipD2{0,-1,0,-1,0,0};

template<typename T>
const transformation<T> transformation<T>::flipD3{-1,-1,0,0,1,0};

template<typename T>
const transformation<T> transformation<T>::flipD4{1,1,0,0,-1,0};

template<typename T>
const transformation<T> transformation<T>::flipD5{1,0,0,-1,-1,0};

template<typename T>
const transformation<T> transformation<T>::flipD6{-1,0,0,1,1,0};

template<typename T>
std::vector<transformation<T>> transformation<T>::group(symgroup sg, T sz)
{
	T h = sz/2;
	switch(sg) {
	case symgroup::CM:
		return {id,flipD1};
	case symgroup::CMM:
		return {id,rot180,flipD1,flipD2};
	case symgroup::P1:
		return {id};
	case symgroup::P2:
		return {id,rot180};
	case symgroup::P3:
		return {id,rot120,rot240};
	case symgroup::P31M:
		return {id,rot120,rot240,flipD2,flipD4,flipD6};
	case symgroup::P3M1:
		return {id,rot120,rot240,flipD1,flipD3,flipD5};
	case symgroup::P4:
		return {id,rot90,rot180,rot270};
	case symgroup::P4G:
		return {id,rot90,rot180,rot270,glideX(h,h),glideY(h,h),flipD1Off(h),flipD2Off(h)};
	case symgroup::P4M:
		return {id,rot90,rot180,rot270,flipV,flipH,flipD1,flipD2};
	case symgroup::P6:
		return {id,rot60,rot120,rot180,rot240,rot300};
	case symgroup::P6M:
		return {id,rot60,rot120,rot180,rot240,rot300,flipD1,flipD2,flipD3,flipD4,flipD5,flipD6};
	case symgroup::PG:
		return {id,glideX(h,h)};
	case symgroup::PGG:
		return {id,rot180,glideX(h,h),glideY(h,h)};
	case symgroup::PM:
		return {id,flipH};
	case symgroup::PMG:
		return {id,rot180,glideX(h,0),glideY(0,h)};
	case symgroup::PMM:
		return {id,rot180,flipV,flipH};
	}
}

#endif
