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

#include "symmetric_canvas.h"

using std::get;
using std::make_tuple;

#define TR(xn,yn) [=] (const point &t) { int x=get<0>(t), y=get<1>(t); return make_tuple(xn,yn); }

const transformation id = TR(x,y);
const transformation rot60 = TR(-y,x+y);
const transformation rot90 = TR(-y,x);
const transformation rot120 = TR(-x-y,x);
const transformation rot180 = TR(-x,-y);
const transformation rot240 = TR(y,-x-y);
const transformation rot270 = TR(y,-x);
const transformation rot300 = TR(x+y,-x);
const transformation flipH = TR(-x,y);
const transformation flipV = TR(x,-y);
const transformation flipD1 = TR(y,x);
const transformation flipD2 = TR(-y,-x);
const transformation flipD3 = TR(-x-y,y);
const transformation flipD4 = TR(x+y,-y);
const transformation flipD5 = TR(x,-x-y);
const transformation flipD6 = TR(-x,x+y);

transformation flipD1Off(int offset) { return TR(offset+y,offset+x); }
transformation flipD2Off(int offset) { return TR(offset-y,offset-x); }
transformation glideX(int glide, int offset) { return TR(glide+x,offset-y); }
transformation glideY(int glide, int offset) { return TR(offset-x,glide+y); }

std::vector<transformation> generate_transforms(symgroup sg, int sz)
{
	int h = sz/2;
	switch(sg) {
	case SYM_CM:
		return {id,flipD1};
	case SYM_CMM:
		return {id,rot180,flipD1,flipD2};
	case SYM_P1:
		return {id};
	case SYM_P2:
		return {id,rot180};
	case SYM_P3:
		return {id,rot120,rot240};
	case SYM_P31M:
		return {id,rot120,rot240,flipD2,flipD4,flipD6};
	case SYM_P3M1:
		return {id,rot120,rot240,flipD1,flipD3,flipD5};
	case SYM_P4:
		return {id,rot90,rot180,rot270};
	case SYM_P4G:
		return {id,rot90,rot180,rot270,glideX(h,h),glideY(h,h),flipD1Off(h),flipD2Off(h)};
	case SYM_P4M:
		return {id,rot90,rot180,rot270,flipV,flipH,flipD1,flipD2};
	case SYM_P6:
		return {id,rot60,rot120,rot180,rot240,rot300};
	case SYM_P6M:
		return {id,rot60,rot120,rot180,rot240,rot300,flipD1,flipD2,flipD3,flipD4,flipD5,flipD6};
	case SYM_PG:
		return {id,glideX(h,h)};
	case SYM_PGG:
		return {id,rot180,glideX(h,h),glideY(h,h)};
	case SYM_PM:
		return {id,flipV};
	case SYM_PMG:
		return {id,rot180,glideX(h,0),glideY(0,h)};
	case SYM_PMM:
		return {id,rot180,flipV,flipH};
	}
}
