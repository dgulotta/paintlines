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

#ifndef _BASIC_PAINTER_H
#define _BASIC_PAINTER_H

#include <vector>
#include <tuple>
using std::tie;
using std::tuple;
using std::vector;

class basic_painter
{
	public:
		basic_painter() : size(0) {}
		void paint(int sz) {
			size=sz;
			red.resize(size*size);
			green.resize(size*size);
			blue.resize(size*size);
		}
		tuple<unsigned char &,unsigned char &,unsigned char &> pixel(int x, int y) {
			int index=x+y*size;
			return tie(red[index],green[index],blue[index]);
		}
	protected:
		int size;
		vector<unsigned char> red;
		vector<unsigned char> green;
		vector<unsigned char> blue;
};

#endif
