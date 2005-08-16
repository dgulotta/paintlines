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

inline unsigned char colorchop(double x)
{
  if(x<0.) return 0;
  else if(x>255.) return 255;
  else return (unsigned char) x;
}

enum symgroup {SYM_CM, SYM_CMM, SYM_P1, SYM_P2, SYM_P3, SYM_P3LM, SYM_P3ML,
	       SYM_P4, SYM_P4G, SYM_P4M, SYM_P6, SYM_P6M, SYM_PG, SYM_PGG,
	       SYM_PM, SYM_PMG, SYM_PMM, SYM_CM_O, SYM_CMM_O, SYM_P1_O,
	       SYM_P2_O, SYM_P3_O, SYM_P3LM_O, SYM_P3ML_O, SYM_P4_O, SYM_P4G_O,
	       SYM_P4M_O, SYM_P6_O, SYM_P6M_O, SYM_PG_O, SYM_PGG_O, SYM_PM_O,
	       SYM_PMG_O, SYM_PMM_O, SYM_CM_2,SYM_P4M_2};

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
    red.resize(size*size);
    green.resize(size*size);
    blue.resize(size*size);
  }
  int get_size() {return size;}
  void randomize(int xtiles, int ytiles, vector<unsigned char> &r,
		 vector<unsigned char> &g, vector<unsigned char> &b);
 protected:
  inline unsigned char & mi(vector<unsigned char> &v,int i, int j) {
    return v[mod(i,size)+size*mod(j,size)];
  }
  template <typename T>
  void symmetrize(T &t, void (T::*p)(int,int), int x, int y);
  //template <typename T>
  //void (*(g(T &,(painter::*)(int,int))))() {}
  template <typename T>
  void (painter::*(get_sym_func()))(T &, void (T::*)(int,int),int, int);
  template <typename T>
  void symmetrize_p1(T &t, void (T::*p)(int,int), int x, int y);
  template <typename T>
  void symmetrize_p2(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pm(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_cm(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pg(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p4(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pmm(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_cmm(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pmg(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pgg(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p4g(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p4m(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p3(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p3lm(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p3ml(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p6(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p6m(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p1_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p2_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pm_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_cm_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pg_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p4_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pmm_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_cmm_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pmg_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_pgg_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p4g_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p4m_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p3_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p3lm_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p3ml_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p6_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p6m_o(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_cm_2(T &t, void (T::*p)(int,int), int x, int y);
  template<typename T>
  void symmetrize_p4m_2(T &t, void (T::*p)(int,int), int x, int y);
  vector<unsigned char> red;
  vector<unsigned char> green;
  vector<unsigned char> blue;
  symgroup sg;
  int size;
  int halfsize;
  int size1;
  int halfsize1;
};

template <typename T>
void painter::symmetrize(T &t, void (T::*p)(int,int), int x, int y)
{
  switch(sg) {
  case SYM_P1:
    (t.*p)(x,y);
    break;
  case SYM_P2:
    (t.*p)(x,y);
    (t.*p)(size1-x,size1-y);
    break;
  case SYM_PM:
    (t.*p)(x,y);
    (t.*p)(size1-x,y);
    break;
  case SYM_CM:
    (t.*p)(x,y);
    (t.*p)(y,x);
    break;
  case SYM_PG:
    (t.*p)(x,y);
    (t.*p)(x+halfsize,size1-y);
    break;
  case SYM_P4:
    (t.*p)(x,y);
    (t.*p)(y,size1-x);
    (t.*p)(size1-x,size1-y);
    (t.*p)(size1-y,x);
    break;
  case SYM_PMM:
    (t.*p)(x,y);
    (t.*p)(size1-x,y);
    (t.*p)(x,size1-y);
    (t.*p)(size1-x,size1-y);
    break;
  case SYM_CMM:
    (t.*p)(x,y);
    (t.*p)(y,x);
    (t.*p)(size1-y,size1-x);
    (t.*p)(size1-x,size1-y);
    break;
  case SYM_PMG:
    (t.*p)(x,y);
    (t.*p)(size1-x,y);
    (t.*p)(x+halfsize,size1-y);
    (t.*p)(halfsize1-x,size1-y);
    break;
  case SYM_PGG:
    (t.*p)(x,y);
    (t.*p)(size1-x,y+halfsize);
    (t.*p)(x+halfsize,size1-y);
    (t.*p)(halfsize1-x,halfsize1-y);
    break;
  case SYM_P4G:
    (t.*p)(x,y);
    (t.*p)(y,size1-x);
    (t.*p)(size1-x,size1-y);
    (t.*p)(size1-y,x);
    (t.*p)(halfsize1-y,halfsize1-x);
    (t.*p)(x+halfsize,halfsize1-y);
    (t.*p)(y+halfsize,x+halfsize);
    (t.*p)(halfsize1-x,y+halfsize);
    break;
  case SYM_P4M:
    (t.*p)(x,y);
    (t.*p)(size1-x,y);
    (t.*p)(x,size1-y);
    (t.*p)(size1-x,size1-y);
    (t.*p)(y,x);
    (t.*p)(size1-y,x);
    (t.*p)(y,size1-x);
    (t.*p)(size1-y,size1-x);
    break;
  case SYM_P3:
    (t.*p)(x,y);
    (t.*p)(size1-x-y,x);
    (t.*p)(y,size1-x-y);
    break;
  case SYM_P3LM:
    (t.*p)(x,y);
    (t.*p)(size1-x-y,x);
    (t.*p)(y,size1-x-y);
    (t.*p)(size1-y,size1-x);
    (t.*p)(size1-x,x+y);
    (t.*p)(x+y,size1-y);
    break;
  case SYM_P3ML:
    (t.*p)(x,y);
    (t.*p)(size1-x-y,x);
    (t.*p)(y,size1-x-y);
    (t.*p)(y,x);
    (t.*p)(x,size1-x-y);
    (t.*p)(size1-x-y,y);
    break;
  case SYM_P6:
    (t.*p)(x,y);
    (t.*p)(size1-x-y,x);
    (t.*p)(y,size1-x-y);
    (t.*p)(size1-x,size1-y);
    (t.*p)(x+y,size1-x);
    (t.*p)(size1-y,x+y);
    break;
  case SYM_P6M:
    (t.*p)(x,y);
    (t.*p)(size1-x-y,x);
    (t.*p)(y,size1-x-y);
    (t.*p)(size1-x,size1-y);
    (t.*p)(x+y,size1-x);
    (t.*p)(size1-y,x+y);
    (t.*p)(y,x);
    (t.*p)(size1-x-y,y);
    (t.*p)(x,size1-x-y);
    (t.*p)(size1-y,size1-x);
    (t.*p)(x+y,size1-y);
    (t.*p)(size1-x,x+y);
    break;
  case SYM_P1_O:
    (t.*p)(x,y);
    break;
  case SYM_P2_O:
    (t.*p)(x,y);
    (t.*p)(size-x,size-y);
    break;
  case SYM_PM_O:
    (t.*p)(x,y);
    (t.*p)(size-x,y);
    break;
  case SYM_CM_O:
    (t.*p)(x,y);
    (t.*p)(y,x);
    break;
  case SYM_PG_O:
    (t.*p)(x,y);
    (t.*p)(x+halfsize,size-y);
    break;
  case SYM_P4_O:
    (t.*p)(x,y);
    (t.*p)(y,size-x);
    (t.*p)(size-x,size-y);
    (t.*p)(size-y,x);
    break;
  case SYM_PMM_O:
    (t.*p)(x,y);
    (t.*p)(size-x,y);
    (t.*p)(x,size-y);
    (t.*p)(size-x,size-y);
    break;
  case SYM_CMM_O:
    (t.*p)(x,y);
    (t.*p)(y,x);
    (t.*p)(size-y,size-x);
    (t.*p)(size-x,size-y);
    break;
  case SYM_PMG_O:
    (t.*p)(x,y);
    (t.*p)(size-x,y);
    (t.*p)(x+halfsize,size-y);
    (t.*p)(halfsize-x,size-y);
    break;
  case SYM_PGG_O:
    (t.*p)(x,y);
    (t.*p)(size-x,y+halfsize);
    (t.*p)(x+halfsize,size-y);
    (t.*p)(halfsize-x,halfsize-y);
    break;
  case SYM_P4G_O:
    (t.*p)(x,y);
    (t.*p)(y,size-x);
    (t.*p)(size-x,size-y);
    (t.*p)(size-y,x);
    (t.*p)(halfsize-y,halfsize-x);
    (t.*p)(x+halfsize,halfsize-y);
    (t.*p)(y+halfsize,x+halfsize);
    (t.*p)(halfsize-x,y+halfsize);
    break;
  case SYM_P4M_O:
    (t.*p)(x,y);
    (t.*p)(size-x,y);
    (t.*p)(x,size-y);
    (t.*p)(size-x,size-y);
    (t.*p)(y,x);
    (t.*p)(size-y,x);
    (t.*p)(y,size-x);
    (t.*p)(size-y,size-x);
    break;
  case SYM_P3_O:
    (t.*p)(x,y);
    (t.*p)(size-x-y,x);
    (t.*p)(y,size-x-y);
    break;
  case SYM_P3LM_O:
    (t.*p)(x,y);
    (t.*p)(size-x-y,x);
    (t.*p)(y,size-x-y);
    (t.*p)(size-y,size-x);
    (t.*p)(size-x,x+y);
    (t.*p)(x+y,size-y);
    break;
  case SYM_P3ML_O:
    (t.*p)(x,y);
    (t.*p)(size-x-y,x);
    (t.*p)(y,size-x-y);
    (t.*p)(y,x);
    (t.*p)(x,size-x-y);
    (t.*p)(size-x-y,y);
    break;
  case SYM_P6_O:
    (t.*p)(x,y);
    (t.*p)(size-x-y,x);
    (t.*p)(y,size-x-y);
    (t.*p)(size-x,size-y);
    (t.*p)(x+y,size-x);
    (t.*p)(size-y,x+y);
    break;
  case SYM_P6M_O:
    (t.*p)(x,y);
    (t.*p)(size-x-y,x);
    (t.*p)(y,size-x-y);
    (t.*p)(size-x,size-y);
    (t.*p)(x+y,size-x);
    (t.*p)(size-y,x+y);
    (t.*p)(y,x);
    (t.*p)(size-x-y,y);
    (t.*p)(x,size-x-y);
    (t.*p)(size-y,size-x);
    (t.*p)(x+y,size-y);
    (t.*p)(size-x,x+y);
    break;
  case SYM_CM_2:
    (t.*p)(x,y);
    (t.*p)(size1-y,size1-x);
    break;
  case SYM_P4M_2:
    (t.*p)(x,y);
    (t.*p)(size1-x,y);
    (t.*p)(x,size1-y);
    (t.*p)(size1-x,size1-y);
    (t.*p)(halfsize1-y,halfsize1-x);
    (t.*p)(y+halfsize,halfsize1-x);
    (t.*p)(halfsize1-y,x+halfsize);
    (t.*p)(y+halfsize,x+halfsize);
    break;
  }
}

template <typename T>
void (painter::*(painter::get_sym_func()))(T &, void (T::*)(int,int),int, int)
{
  switch(sg) {
  case SYM_P1:
    return &painter::symmetrize_p1;
  case SYM_P2:
    return &painter::symmetrize_p2;
 case SYM_PM:
    return &painter::symmetrize_pm;
  case SYM_CM:
    return &painter::symmetrize_cm;
  case SYM_PG:
    return &painter::symmetrize_pg;
  case SYM_P4:
    return &painter::symmetrize_p4;
  case SYM_PMM:
    return &painter::symmetrize_pmm;
  case SYM_CMM:
    return &painter::symmetrize_cmm;
  case SYM_PMG:
    return &painter::symmetrize_pmg;
  case SYM_PGG:
    return &painter::symmetrize_pgg;
  case SYM_P4G:
    return &painter::symmetrize_p4g;
  case SYM_P4M:
    return &painter::symmetrize_p4m;
  case SYM_P3:
    return &painter::symmetrize_p3;
  case SYM_P3LM:
    return &painter::symmetrize_p3lm;
  case SYM_P3ML:
    return &painter::symmetrize_p3ml;
  case SYM_P6:
    return &painter::symmetrize_p6;
  case SYM_P6M:
    return &painter::symmetrize_p6m;
  case SYM_P1_O:
    return &painter::symmetrize_p1_o;
  case SYM_P2_O:
    return &painter::symmetrize_p2_o;
  case SYM_PM_O:
    return &painter::symmetrize_pm_o;
  case SYM_CM_O:
    return &painter::symmetrize_cm_o;
  case SYM_PG_O:
    return &painter::symmetrize_pg_o;
  case SYM_P4_O:
    return &painter::symmetrize_p4_o;
  case SYM_PMM_O:
    return &painter::symmetrize_pmm_o;
  case SYM_CMM_O:
    return &painter::symmetrize_cmm_o;
  case SYM_PMG_O:
    return &painter::symmetrize_pmg_o;
  case SYM_PGG_O:
    return &painter::symmetrize_pgg_o;
  case SYM_P4G_O:
    return &painter::symmetrize_p4g_o;
  case SYM_P4M_O:
    return &painter::symmetrize_p4m_o;
  case SYM_P3_O:
    return &painter::symmetrize_p3_o;
  case SYM_P3LM_O:
    return &painter::symmetrize_p3lm_o;
  case SYM_P3ML_O:
    return &painter::symmetrize_p3ml_o;
  case SYM_P6_O:
    return &painter::symmetrize_p6_o;
  case SYM_P6M_O:
    return &painter::symmetrize_p6m_o;
  case SYM_CM_2:
    return &painter::symmetrize_cm_2;
  case SYM_P4M_2:
    return &painter::symmetrize_p4m_2;
  }
}

template <typename T>
void painter::symmetrize_p1(T &t, void (T::*p)(int,int), int x, int y)
{
  (t.*p)(x,y);
}

template <typename T>
void painter::symmetrize_p2(T &t, void (T::*p)(int,int), int x, int y)
{
  (t.*p)(x,y);
  (t.*p)(size1-x,size1-y);
}

template<typename T>
void painter::symmetrize_pm(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x,y);
}

template<typename T>
void painter::symmetrize_cm(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(y,x);
}

template<typename T>
void painter::symmetrize_pg(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(x+halfsize,size1-y);
}

template<typename T>
void painter::symmetrize_p4(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(y,size1-x);
  (t.*p)(size1-x,size1-y);
  (t.*p)(size1-y,x);
}

template<typename T>
void painter::symmetrize_pmm(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x,y);
  (t.*p)(x,size1-y);
  (t.*p)(size1-x,size1-y);
}

template<typename T>
void painter::symmetrize_cmm(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(y,x);
  (t.*p)(size1-y,size1-x);
  (t.*p)(size1-x,size1-y);
}

template<typename T>
void painter::symmetrize_pmg(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x,y);
  (t.*p)(x+halfsize,size1-y);
  (t.*p)(halfsize1-x,size1-y);
}

template<typename T>
void painter::symmetrize_pgg(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x,y+halfsize);
  (t.*p)(x+halfsize,size1-y);
  (t.*p)(halfsize1-x,halfsize1-y);
}

template<typename T>
void painter::symmetrize_p4g(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(y,size1-x);
  (t.*p)(size1-x,size1-y);
  (t.*p)(size1-y,x);
  (t.*p)(halfsize1-y,halfsize1-x);
  (t.*p)(x+halfsize,halfsize1-y);
  (t.*p)(y+halfsize,x+halfsize);
  (t.*p)(halfsize1-x,y+halfsize);
}

template<typename T>
void painter::symmetrize_p4m(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x,y);
  (t.*p)(x,size1-y);
  (t.*p)(size1-x,size1-y);
  (t.*p)(y,x);
  (t.*p)(size1-y,x);
  (t.*p)(y,size1-x);
  (t.*p)(size1-y,size1-x);
}

template<typename T>
void painter::symmetrize_p3(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x-y,x);
  (t.*p)(y,size1-x-y);
}

template<typename T>
void painter::symmetrize_p3lm(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x-y,x);
  (t.*p)(y,size1-x-y);
  (t.*p)(size1-y,size1-x);
  (t.*p)(size1-x,x+y);
  (t.*p)(x+y,size1-y);
}

template<typename T>
void painter::symmetrize_p3ml(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x-y,x);
  (t.*p)(y,size1-x-y);
  (t.*p)(y,x);
  (t.*p)(x,size1-x-y);
  (t.*p)(size1-x-y,y);
}

template<typename T>
void painter::symmetrize_p6(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x-y,x);
  (t.*p)(y,size1-x-y);
  (t.*p)(size1-x,size1-y);
  (t.*p)(x+y,size1-x);
  (t.*p)(size1-y,x+y);
}

template<typename T>
void painter::symmetrize_p6m(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x-y,x);
  (t.*p)(y,size1-x-y);
  (t.*p)(size1-x,size1-y);
  (t.*p)(x+y,size1-x);
  (t.*p)(size1-y,x+y);
  (t.*p)(y,x);
  (t.*p)(size1-x-y,y);
  (t.*p)(x,size1-x-y);
  (t.*p)(size1-y,size1-x);
  (t.*p)(x+y,size1-y);
  (t.*p)(size1-x,x+y);
}

template<typename T>
void painter::symmetrize_p1_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
}

template<typename T>
void painter::symmetrize_p2_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x,size-y);
}

template<typename T>
void painter::symmetrize_pm_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x,y);
}

template<typename T>
void painter::symmetrize_cm_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(y,x);
}

template<typename T>
void painter::symmetrize_pg_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(x+halfsize,size-y);
}

template<typename T>
void painter::symmetrize_p4_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(y,size-x);
  (t.*p)(size-x,size-y);
  (t.*p)(size-y,x);
}

template<typename T>
void painter::symmetrize_pmm_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x,y);
  (t.*p)(x,size-y);
  (t.*p)(size-x,size-y);
}

template<typename T>
void painter::symmetrize_cmm_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(y,x);
  (t.*p)(size-y,size-x);
  (t.*p)(size-x,size-y);
}

template<typename T>
void painter::symmetrize_pmg_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x,y);
  (t.*p)(x+halfsize,size-y);
  (t.*p)(halfsize-x,size-y);
}

template<typename T>
void painter::symmetrize_pgg_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x,y+halfsize);
  (t.*p)(x+halfsize,size-y);
  (t.*p)(halfsize-x,halfsize-y);
}

template<typename T>
void painter::symmetrize_p4g_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(y,size-x);
  (t.*p)(size-x,size-y);
  (t.*p)(size-y,x);
  (t.*p)(halfsize-y,halfsize-x);
  (t.*p)(x+halfsize,halfsize-y);
  (t.*p)(y+halfsize,x+halfsize);
  (t.*p)(halfsize-x,y+halfsize);
}

template<typename T>
void painter::symmetrize_p4m_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x,y);
  (t.*p)(x,size-y);
  (t.*p)(size-x,size-y);
  (t.*p)(y,x);
  (t.*p)(size-y,x);
  (t.*p)(y,size-x);
  (t.*p)(size-y,size-x);
}

template<typename T>
void painter::symmetrize_p3_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x-y,x);
  (t.*p)(y,size-x-y);
}

template<typename T>
void painter::symmetrize_p3lm_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x-y,x);
  (t.*p)(y,size-x-y);
  (t.*p)(size-y,size-x);
  (t.*p)(size-x,x+y);
  (t.*p)(x+y,size-y);
}

template<typename T>
void painter::symmetrize_p3ml_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x-y,x);
  (t.*p)(y,size-x-y);
  (t.*p)(y,x);
  (t.*p)(x,size-x-y);
  (t.*p)(size-x-y,y);
}

template<typename T>
void painter::symmetrize_p6_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x-y,x);
  (t.*p)(y,size-x-y);
  (t.*p)(size-x,size-y);
  (t.*p)(x+y,size-x);
  (t.*p)(size-y,x+y);
}

template<typename T>
void painter::symmetrize_p6m_o(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size-x-y,x);
  (t.*p)(y,size-x-y);
  (t.*p)(size-x,size-y);
  (t.*p)(x+y,size-x);
  (t.*p)(size-y,x+y);
  (t.*p)(y,x);
  (t.*p)(size-x-y,y);
  (t.*p)(x,size-x-y);
  (t.*p)(size-y,size-x);
  (t.*p)(x+y,size-y);
  (t.*p)(size-x,x+y);
}

template<typename T>
void painter::symmetrize_cm_2(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-y,size1-x);
}

template<typename T>
void painter::symmetrize_p4m_2(T &t, void (T::*p)(int,int), int x, int y) {
  (t.*p)(x,y);
  (t.*p)(size1-x,y);
  (t.*p)(x,size1-y);
  (t.*p)(size1-x,size1-y);
  (t.*p)(halfsize1-y,halfsize1-x);
  (t.*p)(y+halfsize,halfsize1-x);
  (t.*p)(halfsize1-y,x+halfsize);
  (t.*p)(y+halfsize,x+halfsize);
}

#endif