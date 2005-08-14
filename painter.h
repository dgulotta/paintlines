/***************************************************************************
 *   Copyright (C) 2003-2005 by Daniel Gulotta                             *
 *   dgulotta@mit.edu                                                      *
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

#ifndef _PAINTER_H
#define _PAINTER_H

#include <vector>
using std::vector;

inline int mod(int x, int y) 
{
  int ans=x%y;
  return (ans<0)?(ans+y):ans;
}

enum symgroup {SYM_CM, SYM_CMM, SYM_P1, SYM_P2, SYM_P3, SYM_P3LM, SYM_P3ML,
	       SYM_P4, SYM_P4G, SYM_P4M, SYM_P6, SYM_P6M, SYM_PG_O, SYM_PGG,
	       SYM_PM, SYM_PMG, SYM_PMM, SYM_CM_2, SYM_P4M_2,SYM_PG_D};

class painter
{
 public:
  painter() : sg(SYM_P1), size(0), halfsize(0), size1(-1), halfsize1(-1) {}
  void paint(int sz, symgroup sym) {
    size=sz;
    size1=size-1;
    halfsize=size/2;
    halfsize1=halfsize-1;
    sg=sym;
  }
  int get_size() {return size;}
  void randomize(int xtiles, int ytiles, vector<unsigned char> &r,
		 vector<unsigned char> &g, vector<unsigned char> &b);
 protected:
  vector<unsigned char> red;
  vector<unsigned char> green;
  vector<unsigned char> blue;
  symgroup sg;
  int size;
  int halfsize;
  int size1;
  int halfsize1;
};

#endif
