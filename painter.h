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

#include <vector>
#include <cmath>
#include <functional>

#include "basic_painter.h"

using std::vector;
using std::cos;
using std::function;

inline int mod(int x, int y) 
{
  int ans=x%y;
  return (ans<0)?(ans+y):ans;
}

inline unsigned char colorchop(double x)
{
  if(x<0.) return 0;
  else if(x>255.) return 255;
  else return (unsigned char) x;
}

enum symgroup {SYM_CM, SYM_CMM, SYM_P1, SYM_P2, SYM_P3, SYM_P31M, SYM_P3M1,
	       SYM_P4, SYM_P4G, SYM_P4M, SYM_P6, SYM_P6M, SYM_PG, SYM_PGG,
	       SYM_PM, SYM_PMG, SYM_PMM, SYM_CM_O, SYM_CMM_O, SYM_P1_O,
	       SYM_P2_O, SYM_P3_O, SYM_P31M_O, SYM_P3M1_O, SYM_P4_O, SYM_P4G_O,
	       SYM_P4M_O, SYM_P6_O, SYM_P6M_O, SYM_PG_O, SYM_PGG_O, SYM_PM_O,
	       SYM_PMG_O, SYM_PMM_O, SYM_CM_2,SYM_P4M_2};

struct painter_transformation {
  void operator () (int &i, int &j) const;
  int xx;
  int xy;
  int x1;
  int yx;
  int yy;
  int y1;
};

class painter_transform {
 public:
  painter_transform(int _size, int _xtiles, int _ytiles)
    : size(_size), xtiles(_xtiles), ytiles(_ytiles), width(_xtiles*_size),
      height(_ytiles*_size) {}
  void set_to_trans(int xx, int xy, int x1, int yx, int yy, int y1)
  { to.xx=xx; to.xy=xy; to.x1=x1; to.yx=yx; to.yy=yy; to.y1=y1; }
  void set_from_trans(int xx, int xy, int x1, int yx, int yy, int y1)
  { from.xx=xx; from.xy=xy; from.x1=x1; from.yx=yx; from.yy=yy; from.y1=y1; }
  void set_point(int x, int y);
  template <typename T>
  void copy(T &to, const T &from) { to[to_index]=from[from_index]; }
 private:
  int size;
  int xtiles;
  int ytiles;
  int width;
  int height;
  painter_transformation to;
  painter_transformation from;
  int from_index;
  int to_index;
};

class painter : virtual public basic_painter
{
 public:
  painter() : sg(SYM_P1), halfsize(0), size1(-1), halfsize1(-1) {}
  void paint(int sz, symgroup sym) {
    basic_painter::paint(sz);
    //size=sz;
    size1=size-1;
    halfsize=size/2;
    halfsize1=halfsize-1;
    sg=sym;
    //red.resize(size*size);
    //green.resize(size*size);
    //blue.resize(size*size);
  }
  int get_size() {return size;}
  void randomize(int xtiles, int ytiles, vector<unsigned char> &r,
		 vector<unsigned char> &g, vector<unsigned char> &b);
 protected:
  void randomize_p3m1_choose_from_trans(painter_transform &pt, int mt, int nt,
					int z);
  void randomize_p2_choose_from_trans(painter_transform &pt, int n);
  void randomize_pmg_choose_from_trans(painter_transform &pt, int n);
  void randomize_pgg_choose_from_trans(painter_transform &pt, int n);
  void randomize_p4_choose_from_trans(painter_transform &pt, bool a, bool b);
  inline unsigned char & mi(vector<unsigned char> &v,int i, int j) {
    return v[mod(i,size)+size*mod(j,size)];
  }
  template <typename F>
  function<void(int,int)> symmetrize(const F &f);
  template <typename F>
  void enumerate(const F &f);
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
  void symmetrize_p1_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p2_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pm_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_cm_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pg_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p4_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pmm_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_cmm_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pmg_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_pgg_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p4g_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p4m_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p3_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p31m_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p3m1_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p6_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p6m_o(const function<void(int,int)> &f, int x, int y);
  void symmetrize_cm_2(const function<void(int,int)> &f, int x, int y);
  void symmetrize_p4m_2(const function<void(int,int)> &f, int x, int y);
  static void (painter::*(symfuncs[36]))(const function<void(int,int)> &f, int x, int y);
  //vector<unsigned char> red;
  //vector<unsigned char> green;
  //vector<unsigned char> blue;
  symgroup sg;
  //int size;
  int halfsize;
  int size1;
  int halfsize1;
};

template<typename F>
function<void(int,int)> painter::symmetrize(const F &f) {
	auto p = symfuncs[(int)sg];
	return function<void(int,int)>([=](int x, int y) { (this->*p)(function<void(int,int)>(f),x,y); });
}

template<typename F>
void painter::enumerate(const F &f)
{
  int i,j;
  switch(sg) {
  case SYM_CM:
    for(i=0;i<size;i++)
      for(j=0;j<=i;j++)
	f(i,j);
    break;
  case SYM_CMM:
    for(i=0;i<halfsize;i++)
      for(j=0;j<=i;j++) {
	f(i,j);
	f(size1-i,j);
      }
    break;
  case SYM_P1:
    for(i=0;i<size;i++)
      for(j=0;j<size;j++)
	f(i,j);
    break;
  case SYM_P2:
  case SYM_PG:
   for(i=0;i<size;i++)
     for(j=0;j<halfsize;j++)
       f(i,j);
   break;
  case SYM_P3:
      for(i=0;i*3<size;i++)
	for(j=i;j+2*i<size1;j++)
	  f(i,j);
      for(i=2*size/3;i<size;i++)
	for(j=2*(size-i);j<=i;j++)
	  f(i,j);
    break;
  case SYM_P31M:
      for(i=0;i*3<size;i++)
	for(j=i;j+2*i<size1;j++)
	  f(i,j);
    break;
  case SYM_P3M1:
    for(i=0;i*3<size;i++)
      for(j=i;2*j+i<size;j++)
	f(i,j);
    for(i=0;i*3<size1;i++)
      for(j=i;2*j+i<size1;j++)
	f(size1-i,size1-j);
    break;
  case SYM_P4:
  case SYM_PMM:
    for(i=0;i<halfsize;i++)
      for(j=0;j<halfsize;j++)
	f(i,j);
    break;
  case SYM_P4G:
    for(i=0;i<halfsize;i++)
      for(j=0;i+j<halfsize;j++)
	f(i,j);
    break;
  case SYM_P4M:
    for(i=0;i<halfsize;i++)
      for(j=0;j<=i;j++)
	f(i,j);
    break;
  case SYM_P6:
    for(j=0;j<halfsize;j++)
      f(0,j);
    for(i=1;i*3<size;i++)
	for(j=i;j+2*i<size1;j++)
	  f(i,j);
    break;
  case SYM_P6M:
    for(i=0;3*i<size;i++) {
      for(j=i;i+2*j<size;j++) {
	f(i,j);
      }
    }
    break;
  case SYM_PGG:
    {
      int begin=halfsize/2, end=halfsize+begin;
      for(i=begin;i<end;i++)
	for(j=begin;j<end;j++)
	f(i,j);
    }
    break;
  case SYM_PM:
  case SYM_PMG:
    for(i=0;i<halfsize;i++)
      for(j=0;j<size;j++)
	f(i,j);
    break;
  }
}

#endif
