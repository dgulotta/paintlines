/***************************************************************************
 *   Copyright (C) 2003-2008, 2013 by Daniel Gulotta                       *
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

#ifndef _PAINTER_H
#define _PAINTER_H

#include <algorithm>
#include <vector>
#include <cmath>
#include <functional>

#include "basic_painter.h"

using std::vector;
using std::function;
using std::bind;
using namespace std::placeholders;

inline int mod(int x, int y) 
{
  int ans=x%y;
  return (ans<0)?(ans+y):ans;
}

inline int divide(int x, int y)
{
	return (x>0)?(x/y):(-(-x)/y);
}

inline unsigned char colorchop(double x)
{
  if(x<0.) return 0;
  else if(x>255.) return 255;
  else return (unsigned char) x;
}

enum symgroup {SYM_CM, SYM_CMM, SYM_P1, SYM_P2, SYM_P3, SYM_P31M, SYM_P3M1,
	       SYM_P4, SYM_P4G, SYM_P4M, SYM_P6, SYM_P6M, SYM_PG, SYM_PGG,
	       SYM_PM, SYM_PMG, SYM_PMM};

struct painter_transformation {
	painter_transformation() {}
	painter_transformation(int _xx, int _xy, int _x1, int _yx, int _yy, int _y1)
		: xx(_xx), xy(_xy), x1(_x1), yx(_yx), yy(_yy), y1(_y1) {}
	void operator () (int &i, int &j) const;
	int xx;
	int xy;
	int x1;
	int yx;
	int yy;
	int y1;
};

template<typename T>
class wrap_grid
{
public:
	wrap_grid(int w, int h) : v(w*h), width(w), height(h) {}
	template<typename F>
	wrap_grid(int w, int h, F f) : wrap_grid(w,h) { generate(v.begin(),v.end(),f); }
	T & operator () (int x, int y) { return v[mod(x,width)+width*mod(y,height)]; }
	const T & operator () (int x, int y) const { return v[mod(x,width)+width*mod(y,height)]; }
private:
	vector<T> v;
	int width, height;
};

class painter : virtual public basic_painter
{
 public:
  painter() : sg(SYM_P1), halfsize(0) {}
  void paint(int sz, symgroup sym) {
    basic_painter::paint(sz);
    //size=sz;
    halfsize=size/2;
    sg=sym;
    //red.resize(size*size);
    //green.resize(size*size);
    //blue.resize(size*size);
  }
  int get_size() {return size;}
  tuple<unsigned char &,unsigned char &,unsigned char &> pixel(int x, int y) {
  	return basic_painter::pixel(mod(x,size),mod(y,size));
  }
  void randomize(int xtiles, int ytiles, vector<unsigned char> &r,
		 vector<unsigned char> &g, vector<unsigned char> &b);
  template <typename F>
  function<void(int,int)> symmetrize(const F &f);
 protected:
  void symmetrize_p1(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p2(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pm(const function<void(int,int)> &f, int x, int y);
  void symmetrize_cm(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pg(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p4(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pmm(const function<void(int,int)> &f, int x, int y);
  void symmetrize_cmm(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pmg(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pgg(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p4g(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p4m(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p3(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p31m(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p3m1(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p6(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p6m(const function<void(int,int)> &f, int x, int y);
  static void (painter::*(symfuncs[17]))(const function<void(int,int)> &f, int x, int y);
  //vector<unsigned char> red;
  //vector<unsigned char> green;
  //vector<unsigned char> blue;
  symgroup sg;
  //int size;
  int halfsize;
};

template<typename F>
function<void(int,int)> painter::symmetrize(const F &f) {
	auto p = symfuncs[(int)sg];
	return bind(p,this,function<void(int,int)>(f),_1,_2);
}

#endif
