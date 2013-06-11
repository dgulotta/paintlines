/***************************************************************************
 *   Copyright (C) 2005-2008, 2013 by Daniel Gulotta                       *
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

#ifndef _HYPERBOLIC_PAINTER_H
#define _HYPERBOLIC_PAINTER_H

#include <vector>
#include <cmath>
#include <cstdlib>
#include <functional>
#include "basic_painter.h"

using namespace std;

struct hyperbolic_coord
{
  hyperbolic_coord() {}
  hyperbolic_coord(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
  double x;
  double y;
  double z;
};

inline double operator * (const hyperbolic_coord &a, const hyperbolic_coord &b)
{
  return a.z*b.z-a.x*b.x-a.y*b.y;
}

inline hyperbolic_coord operator * (double a, const hyperbolic_coord &b)
{
  hyperbolic_coord ans;
  ans.x=a*b.x;
  ans.y=a*b.y;
  ans.z=a*b.z;
  return ans;
}

inline hyperbolic_coord operator * (const hyperbolic_coord &a, double b)
{
  return b*a;
}

inline hyperbolic_coord operator / (const hyperbolic_coord &a, double b)
{
  hyperbolic_coord ans;
  ans.x=a.x/b;
  ans.y=a.y/b;
  ans.z=a.z/b;
  return ans;
}

inline hyperbolic_coord operator + (const hyperbolic_coord &a,
				    const hyperbolic_coord &b)
{
  hyperbolic_coord ans;
  ans.x=a.x+b.x;
  ans.y=a.y+b.y;
  ans.z=a.z+b.z;
  return ans;
}

inline hyperbolic_coord operator - (const hyperbolic_coord &a)
{
  hyperbolic_coord ans;
  ans.x=-a.x;
  ans.y=-a.y;
  ans.z=-a.z;
  return ans;
}

inline hyperbolic_coord operator - (const hyperbolic_coord &a,
				    const hyperbolic_coord &b)
{
  hyperbolic_coord ans;
  ans.x=a.x-b.x;
  ans.y=a.y-b.y;
  ans.z=a.z-b.z;
  return ans;
}

inline hyperbolic_coord & operator *= (hyperbolic_coord &a,double b)
{
  a.x*=b;
  a.y*=b;
  a.z*=b;
  return a;
}

inline hyperbolic_coord & operator /= (hyperbolic_coord &a,double b)
{
  a.x/=b;
  a.y/=b;
  a.z/=b;
  return a;
}

inline hyperbolic_coord & operator += (hyperbolic_coord &a,
				       const hyperbolic_coord &b)
{
  a.x+=b.x;
  a.y+=b.y;
  a.z+=b.z;
  return a;
}

inline hyperbolic_coord & operator -= (hyperbolic_coord &a,
				       const hyperbolic_coord &b)
{
  a.x-=b.x;
  a.y-=b.y;
  a.z-=b.z;
  return a;
}

hyperbolic_coord normalize(const hyperbolic_coord &c);

hyperbolic_coord & inplace_normalize(hyperbolic_coord &c);

hyperbolic_coord cross(const hyperbolic_coord &a, const hyperbolic_coord &b);

struct planar_coord
{
  planar_coord() {}
  planar_coord(double _x, double _y) : x(_x), y(_y) {}
  double x;
  double y;
};

struct screen_coord
{
  screen_coord() {}
  screen_coord(int _x, int _y) : x(_x), y(_y) {}
  int x;
  int y;
};

planar_coord poincare_projection(const hyperbolic_coord &hc);

planar_coord klein_projection(const hyperbolic_coord &hc);

enum projtype {POINCARE, KLEIN};

class hyperbolic_transformation
{
 public:
  static const hyperbolic_transformation identity;
  static hyperbolic_transformation reflection(const hyperbolic_coord &c);
  static hyperbolic_transformation rotation_180(const hyperbolic_coord &c);
  static hyperbolic_transformation rotation_origin(int n);
  static hyperbolic_transformation rotation(int n, const hyperbolic_coord &c);
  static hyperbolic_transformation glide_reflection(const hyperbolic_coord &c, double r);
  hyperbolic_transformation() {}
  hyperbolic_transformation(double _xx, double _xy, double _xz, double _yx,
			    double _yy, double _yz, double _zx, double _zy,
			    double _zz)
    : xx(_xx), xy(_xy), xz(_xz), yx(_yx), yy(_yy), yz(_yz), zx(_zx), zy(_zy),
    zz(_zz) {}
  hyperbolic_transformation(const hyperbolic_transformation &t)
    : xx(t.xx), xy(t.xy), xz(t.xz), yx(t.yx), yy(t.yy), yz(t.yz), zx(t.zx),
    zy(t.zy), zz(t.zz) {}
  hyperbolic_transformation & operator = (const hyperbolic_transformation &t) {
    xx=t.xx;xy=t.xy;xz=t.xz;yx=t.yx;yy=t.yy;yz=t.yz;zx=t.zx;zy=t.zy;zz=t.zz;
    return *this;
  }
  ~hyperbolic_transformation() {}
  inline hyperbolic_coord operator () (const hyperbolic_coord &c) const {
    hyperbolic_coord ans;
    ans.x=xx*c.x+xy*c.y+xz*c.z;
    ans.y=yx*c.x+yy*c.y+yz*c.z;
    ans.z=zx*c.x+zy*c.y+zz*c.z;
    return ans;
  }
  inline hyperbolic_coord transpose(const hyperbolic_coord &c) const {
    hyperbolic_coord ans;
    ans.x=xx*c.x+yx*c.y+zx*c.z;
    ans.y=xy*c.x+yy*c.y+zy*c.z;
    ans.z=xz*c.x+yz*c.y+zz*c.z;
    return ans;
  }
  inline hyperbolic_coord inverse(const hyperbolic_coord &c) const {
    hyperbolic_coord ans;
    ans.x=xx*c.x+yx*c.y-zx*c.z;
    ans.y=xy*c.x+yy*c.y-zy*c.z;
    ans.z=-xz*c.x-yz*c.y+zz*c.z;
    return ans;
  }
  inline hyperbolic_transformation inverse() const {
  	return hyperbolic_transformation(xx,yx,-zx,xy,yy,-zy,-xz,-yz,zz);
  }
  double xx, xy, xz, yx, yy, yz, zx, zy, zz;
  // it would probably be faster to use opengl for this
};

inline hyperbolic_transformation operator *
(const hyperbolic_transformation &t1, const hyperbolic_transformation &t2)
{
  hyperbolic_transformation ans;
  ans.xx=t1.xx*t2.xx+t1.xy*t2.yx+t1.xz*t2.zx;
  ans.xy=t1.xx*t2.xy+t1.xy*t2.yy+t1.xz*t2.zy;
  ans.xz=t1.xx*t2.xz+t1.xy*t2.yz+t1.xz*t2.zz;
  ans.yx=t1.yx*t2.xx+t1.yy*t2.yx+t1.yz*t2.zx;
  ans.yy=t1.yx*t2.xy+t1.yy*t2.yy+t1.yz*t2.zy;
  ans.yz=t1.yx*t2.xz+t1.yy*t2.yz+t1.yz*t2.zz;
  ans.zx=t1.zx*t2.xx+t1.zy*t2.yx+t1.zz*t2.zx;
  ans.zy=t1.zx*t2.xy+t1.zy*t2.yy+t1.zz*t2.zy;
  ans.zz=t1.zx*t2.xz+t1.zy*t2.yz+t1.zz*t2.zz;
  return ans;
}

enum flip_type { FLIP_ALL, FLIP_ALTERNATING, FLIP_RANDOM };

struct hyperbolic_tile
{
  hyperbolic_transformation t;
  hyperbolic_coord edge1;
  hyperbolic_coord edge2;
  hyperbolic_coord edge3;
};

class hyperbolic_symmetry_group
{
public:
  static hyperbolic_symmetry_group * group_sabc(int n1, int n2, int n3, flip_type f=FLIP_ALL);
  static hyperbolic_symmetry_group * group_a222(int a, flip_type f=FLIP_ALL);
  static hyperbolic_symmetry_group * group_2sab(int a, int b, flip_type f=FLIP_ALL);
  static hyperbolic_symmetry_group * group_22sa(int a, flip_type f=FLIP_ALL);
  static hyperbolic_symmetry_group * group_ab2(int n1, int n2, flip_type f=FLIP_ALL);
  static hyperbolic_symmetry_group * group_asb(int n1, int n2, flip_type f=FLIP_ALL);
  static hyperbolic_symmetry_group * group_a2x(int a, flip_type f=FLIP_ALL);
  static hyperbolic_symmetry_group * group_sax(int a, flip_type f=FLIP_ALL);
  template<typename F>
  function<void(const hyperbolic_coord &)> symmetrize(const F &f);
  hyperbolic_coord random_symmetry(const hyperbolic_coord &c) {
    double i=double(rand())/RAND_MAX;
    return tiles[pow(tiles.size(),i)].t(c);
  }
private:
  hyperbolic_symmetry_group() : alternating(false) {}
  void make_tiles(const hyperbolic_transformation &t1,
		  const hyperbolic_transformation &t2,
		  const hyperbolic_transformation &t3, flip_type f=FLIP_ALL);
  vector<hyperbolic_tile> tiles;
  vector<bool> flipped;
  bool alternating;
};

template <typename F>
function<void(const hyperbolic_coord &)> hyperbolic_symmetry_group::symmetrize(const F &f)
{
	return [&](const hyperbolic_coord &hc) {
		vector<hyperbolic_tile>::iterator it=tiles.begin();
		while(it!=tiles.end()&&(it->edge1*hc<0||it->edge2*hc<0||it->edge3*hc<0)) {
			it++;
		}
		if(it!=tiles.end()) {
			hyperbolic_coord c=(it->t).inverse(hc);
			if(alternating) {
				bool b=flipped[it-tiles.begin()];
				vector<bool>::iterator it2=flipped.begin();
				for(it=tiles.begin();it!=tiles.end();it++,it2++) {
					if(b==*it2) f((it->t)(c));
				}
			}
			else {
				// the following two lines are useful for debugging
				//(t.*p)(c);
				//(t.*p)(tiles[1].t(c));
				for(it=tiles.begin();it!=tiles.end();it++) {
					f((it->t)(c));
				}
			}
		}
 	};
}

class hyperbolic_painter : public virtual basic_painter
{
 public:
  hyperbolic_painter() : pt(POINCARE) {};
  void paint(int sz, hyperbolic_symmetry_group &sym) {
    basic_painter::paint(sz);
    //size=sz;
    pixel_width=2./size;
    sg=&sym;
    //red.resize(size*size);
    //green.resize(size*size);
    //blue.resize(size*size);
  }
  void set_projtype(projtype _pt) {pt=_pt;}
  screen_coord toscreen(const planar_coord &p) {
    return screen_coord(size*(p.x+1.)/2.,size*(p.y+1.)/2.);
  }
 protected:
  //int size;
  double pixel_width;
  //vector<unsigned char> red;
  //vector<unsigned char> green;
  //vector<unsigned char> blue;
  hyperbolic_symmetry_group *sg;
  projtype pt;
};

#endif
