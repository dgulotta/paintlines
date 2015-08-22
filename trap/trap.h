/***************************************************************************
 *   Copyright (C) 2014-2015 by Daniel Gulotta                             *
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

#ifndef _TRAP_H
#define _TRAP_H

#include <tuple>
#include <functional>
#include "../color.h"
#include "../symmetric_canvas.h"

typedef std::function<std::tuple<double,double>(double,double)> iterfunc;

iterfunc random_iterfunc(symgroup sg);

void drawtrap(symmetric_canvas<color_t> &c);
void drawquasitrap(canvas<color_t> &c,int symmetry,double quasiperiod);
void drawquasitrap_poly(canvas<color_t> &c,int symmetry,double quasiperiod);

#endif
