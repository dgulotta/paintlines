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

#ifndef _RANDOMIZE_H
#define _RANDOMIZE_H

#include <functional>
#include "symmetric_canvas.hpp"

void randomize(int xtiles, int ytiles, symgroup sg, int size, const std::function<void(int,int,int,int)> &f);

template<typename T>
wrap_canvas<T> randomize(int xtiles, int ytiles, const symmetric_canvas<T> &orig)
{
	wrap_canvas<T> randomized(xtiles*orig.size(),ytiles*orig.size());
	randomize(xtiles,ytiles,orig.group(),orig.size(),[&](int xf,int yf,int xt,int yt) {randomized(xt,yt)=orig.get(xf,yf);});
	return randomized;
}

#endif
