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

#ifndef _SYMMETRY_H
#define _SYMMETRY_H

template<typename T>
using point = std::tuple<T,T>;

template<typename T>
using transformation = std::function<point<T>(const point<T> &)>;

enum symgroup {SYM_CM, SYM_CMM, SYM_P1, SYM_P2, SYM_P3, SYM_P31M, SYM_P3M1,
	SYM_P4, SYM_P4G, SYM_P4M, SYM_P6, SYM_P6M, SYM_PG, SYM_PGG, SYM_PM,
	SYM_PMG, SYM_PMM};

static const int num_symmetries[17] = { 2, 4, 1,
	2, 3, 6, 6, 4, 8, 8, 6, 12, 2, 4, 2, 4, 4};

static const bool sym_is_hexagonal[17] = { false, false, false, false, true, true,
	true, false, false, false, true, true, false, false, false, false, false };

#define TRB(xn,yn) (const point<T> &t) { T x=std::get<0>(t), y=std::get<1>(t); return std::make_tuple(xn,yn); }
#define TR(xn,yn) [=] TRB(xn,yn)
#define TRDEC(name,xn,yn) template<typename T> point<T> name TRB(xn,yn)

TRDEC(id,x,y);
TRDEC(rot60,-y,x+y);
TRDEC(rot90,-y,x);
TRDEC(rot120,-x-y,x);
TRDEC(rot180,-x,-y);
TRDEC(rot240,y,-x-y);
TRDEC(rot270,y,-x);
TRDEC(rot300,x+y,-x);
TRDEC(flipH,-x,y);
TRDEC(flipV,x,-y);
TRDEC(flipD1,y,x);
TRDEC(flipD2,-y,-x);
TRDEC(flipD3,-x-y,y);
TRDEC(flipD4,x+y,-y);
TRDEC(flipD5,x,-x-y);
TRDEC(flipD6,-x,x+y);

template<typename T>
transformation<T> flipD1Off(T offset) { return TR(offset+y,offset+x); }
template<typename T>
transformation<T> flipD2Off(T offset) { return TR(offset-y,offset-x); }
template<typename T>
transformation<T> glideX(T glide, T offset) { return TR(glide+x,offset-y); }
template<typename T>
transformation<T> glideY(T glide, T offset) { return TR(offset-x,glide+y); }

#undef TRB
#undef TR
#undef TRDEC

template<typename T>
std::vector<transformation<T>> generate_transforms(symgroup sg, T sz)
{
	T h = sz/2;
	switch(sg) {
	case SYM_CM:
		return {id<T>,flipD1<T>};
	case SYM_CMM:
		return {id<T>,rot180<T>,flipD1<T>,flipD2<T>};
	case SYM_P1:
		return {id<T>};
	case SYM_P2:
		return {id<T>,rot180<T>};
	case SYM_P3:
		return {id<T>,rot120<T>,rot240<T>};
	case SYM_P31M:
		return {id<T>,rot120<T>,rot240<T>,flipD2<T>,flipD4<T>,flipD6<T>};
	case SYM_P3M1:
		return {id<T>,rot120<T>,rot240<T>,flipD1<T>,flipD3<T>,flipD5<T>};
	case SYM_P4:
		return {id<T>,rot90<T>,rot180<T>,rot270<T>};
	case SYM_P4G:
		return {id<T>,rot90<T>,rot180<T>,rot270<T>,glideX(h,h),glideY(h,h),flipD1Off(h),flipD2Off(h)};
	case SYM_P4M:
		return {id<T>,rot90<T>,rot180<T>,rot270<T>,flipV<T>,flipH<T>,flipD1<T>,flipD2<T>};
	case SYM_P6:
		return {id<T>,rot60<T>,rot120<T>,rot180<T>,rot240<T>,rot300<T>};
	case SYM_P6M:
		return {id<T>,rot60<T>,rot120<T>,rot180<T>,rot240<T>,rot300<T>,flipD1<T>,flipD2<T>,flipD3<T>,flipD4<T>,flipD5<T>,flipD6<T>};
	case SYM_PG:
		return {id<T>,glideX(h,h)};
	case SYM_PGG:
		return {id<T>,rot180<T>,glideX(h,h),glideY(h,h)};
	case SYM_PM:
		return {id<T>,flipH<T>};
	case SYM_PMG:
		return {id<T>,rot180<T>,glideX(h,(T)0),glideY((T)0,h)};
	case SYM_PMM:
		return {id<T>,rot180<T>,flipV<T>,flipH<T>};
	}
}

#endif
