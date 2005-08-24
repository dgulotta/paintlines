/***************************************************************************
 *   Copyright (C) 2005 by Daniel Gulotta                                  *
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

#ifndef _HYPERBOLIC_PAINTER_H
#define _HYPERBOLIC_PAINTER_H

#include <vector>
#include <cmath>
#include <memory>
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
  virtual ~hyperbolic_transformation() {}
  virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
    return c;
  };
};

class hyperbolic_reflection : public hyperbolic_transformation
{
 public:
  hyperbolic_reflection(const hyperbolic_coord &c) : normal(c) {}
  virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
    return c+2.*(normal*c)*normal;
  }
 private:
  hyperbolic_coord normal;
};

class hyperbolic_rotation_180 : public hyperbolic_transformation
{
 public:
  hyperbolic_rotation_180(const hyperbolic_coord &c) : center(c) {}
  virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
    return 2.*(center*c)*center-c;
  }
 private:
  hyperbolic_coord center;
};

class hyperbolic_rotation_origin : public hyperbolic_transformation
{
 public:
  hyperbolic_rotation_origin(int n) : cosine(cos(2.*M_PI/n)),
    sine(sin(2.*M_PI/n)) {}
  virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
    hyperbolic_coord ans;
    ans.x=cosine*c.x-sine*c.y;
    ans.y=sine*c.x+cosine*c.y;
    ans.z=c.z;
    return ans;
  }
  private:
    double cosine;
    double sine;
};

class hyperbolic_rotation : public hyperbolic_transformation
{
 public:
  hyperbolic_rotation(int n, const hyperbolic_coord &c)
    : cosine(cos(2.*M_PI/n)), cosine1(1.-cosine), sine(sin(2.*M_PI/n)),
    center(c) {}
  virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
    return cosine*c+cosine1*(c*center)*center+sine*cross(center,c);
  }
 private:
  double cosine;
  double cosine1;
  double sine;
  hyperbolic_coord center;
};

class hyperbolic_glide_reflection : public hyperbolic_transformation
{
 public:
  hyperbolic_glide_reflection(double a, double z)
    : r(normalize(hyperbolic_coord(cos(a/2.)*sqrt(z*z-1.),
				   sin(a/2.)*sqrt(z*z-1.),z+1.))) {}
    virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
    hyperbolic_coord ans=r(c);
    ans.y=-ans.y;
    return ans;
  }
 private:
  hyperbolic_rotation_180 r;
};

struct hyperbolic_symmetry_group_ref
{
  explicit hyperbolic_symmetry_group_ref
  (auto_ptr<hyperbolic_transformation> p1,
   auto_ptr<hyperbolic_transformation> p2,
   auto_ptr<hyperbolic_transformation> p3, bool rev)
    : trans1(p1), trans2(p2), trans3(p3), reversed(rev) {}
  auto_ptr_ref<hyperbolic_transformation> trans1;
  auto_ptr_ref<hyperbolic_transformation> trans2;
  auto_ptr_ref<hyperbolic_transformation> trans3;
  bool reversed;
};

class hyperbolic_symmetry_group
{
  friend hyperbolic_symmetry_group hyperbolic_3mirror(int n1, int n2, int n3);
  friend hyperbolic_symmetry_group hyperbolic_3_180(double a1, double a2,
						    double a3);
  friend hyperbolic_symmetry_group hyperbolic_2mirror_180(int n1, double a2,
							  double a3);
  friend hyperbolic_symmetry_group hyperbolic_mirror_2_180
    (double a1, double a2,double a3);
  friend hyperbolic_symmetry_group hyperbolic_180_rotation(int n1, int n2);
  friend hyperbolic_symmetry_group hyperbolic_mirror_rotation(int n1, int n2);
  friend hyperbolic_symmetry_group hyperbolic_3rotation(int n);
  friend hyperbolic_symmetry_group hyperbolic_glide_180(double a1, double a2);
  friend hyperbolic_symmetry_group hyperbolic_glide_mirror
    (double a1, double a2);
 public:
  hyperbolic_symmetry_group() {}
  hyperbolic_symmetry_group(hyperbolic_symmetry_group &s)
    : trans1(s.trans1), trans2(s.trans2), trans3(s.trans3),
    reversed(s.reversed) {}
  hyperbolic_symmetry_group(hyperbolic_symmetry_group_ref s) 
    : trans1(s.trans1), trans2(s.trans2), trans3(s.trans3),
    reversed(s.reversed) {}
  operator hyperbolic_symmetry_group_ref () {
    return hyperbolic_symmetry_group_ref(trans1,trans2,trans3,reversed);
  }
  hyperbolic_symmetry_group & operator = (hyperbolic_symmetry_group &s) {
    if(this!=&s) {
      trans1=s.trans1;
      trans2=s.trans2;
      trans3=s.trans3;
      reversed=s.reversed;
    }
    return *this;
  }
  hyperbolic_symmetry_group & operator = (hyperbolic_symmetry_group_ref s) {
    trans1=s.trans1;
    trans2=s.trans2;
    trans3=s.trans3;
    reversed=s.reversed;
    return *this;
  }
  template<typename T>
  void symmetrize(T &t,void (T::*p)(const hyperbolic_coord &),
		  const hyperbolic_coord &hc, int depth) {
    (t.*p)(hc);
    if(depth) {
      if(reversed) {
	symmetrize1r(t,p,(*trans1)(hc),depth-1);
	symmetrize2r(t,p,(*trans2)(hc),depth-1);
	symmetrize3r(t,p,(*trans3)(hc),depth-1);
      }
      else {
	symmetrize1(t,p,(*trans1)(hc),depth-1);
	symmetrize2(t,p,(*trans2)(hc),depth-1);
	symmetrize3(t,p,(*trans3)(hc),depth-1);
      }
    }
  }
  template<typename T>
  void symmetrize1(T &t,void (T::*p)(const hyperbolic_coord &),
		  const hyperbolic_coord &hc, int depth) {
    (t.*p)(hc);
    if(depth) {
      symmetrize2(t,p,(*trans2)(hc),depth-1);
      symmetrize3(t,p,(*trans3)(hc),depth-1);
    }
  }
  template<typename T>
  void symmetrize2(T &t,void (T::*p)(const hyperbolic_coord &),
		  const hyperbolic_coord &hc, int depth) {
    (t.*p)(hc);
    if(depth) {
      symmetrize1(t,p,(*trans1)(hc),depth-1);
      symmetrize3(t,p,(*trans3)(hc),depth-1);
    }
  }
  template<typename T>
  void symmetrize3(T &t,void (T::*p)(const hyperbolic_coord &),
		  const hyperbolic_coord &hc, int depth) {
    (t.*p)(hc);
    if(depth) {
      symmetrize1(t,p,(*trans1)(hc),depth-1);
      symmetrize2(t,p,(*trans2)(hc),depth-1);
    }
  }
  template<typename T>
  void symmetrize1r(T &t,void (T::*p)(const hyperbolic_coord &),
		  const hyperbolic_coord &hc, int depth) {
    (t.*p)(hc);
    if(depth) {
      symmetrize1r(t,p,(*trans1)(hc),depth-1);
      symmetrize3r(t,p,(*trans3)(hc),depth-1);
    }
  }
  template<typename T>
  void symmetrize2r(T &t,void (T::*p)(const hyperbolic_coord &),
		  const hyperbolic_coord &hc, int depth) {
    (t.*p)(hc);
    if(depth) {
      symmetrize2r(t,p,(*trans2)(hc),depth-1);
      symmetrize3r(t,p,(*trans3)(hc),depth-1);
    }
  }
  template<typename T>
  void symmetrize3r(T &t,void (T::*p)(const hyperbolic_coord &),
		  const hyperbolic_coord &hc, int depth) {
    (t.*p)(hc);
    if(depth) {
      symmetrize1r(t,p,(*trans1)(hc),depth-1);
      symmetrize2r(t,p,(*trans2)(hc),depth-1);
    }
  }
  hyperbolic_coord random_symmetry(const hyperbolic_coord &c, int depth) {
    hyperbolic_coord ans(c);
    while(depth--) {
      switch(rand()%3) {
      case 0:
	ans=(*trans1)(ans);
	break;
      case 1:
	ans=(*trans2)(ans);
	break;
      case 2:
	ans=(*trans3)(ans);
      }
    }
    return ans;
  }
 private:
  auto_ptr<hyperbolic_transformation> trans1;
  auto_ptr<hyperbolic_transformation> trans2;
  auto_ptr<hyperbolic_transformation> trans3;
  bool reversed;
};

hyperbolic_symmetry_group hyperbolic_3mirror(int n1, int n2, int n3);
hyperbolic_symmetry_group hyperbolic_3_180(double a1, double a2, double a3);
hyperbolic_symmetry_group hyperbolic_2mirror_180(int n1, double a2, double a3);
hyperbolic_symmetry_group hyperbolic_mirror_2_180(double a1, double a2,
						  double a3);
hyperbolic_symmetry_group hyperbolic_180_rotation(int n1, int n2);
hyperbolic_symmetry_group hyperbolic_mirror_rotation(int n1, int n2);
hyperbolic_symmetry_group hyperbolic_3rotation(int n);
hyperbolic_symmetry_group hyperbolic_glide_180(double a1, double a2);
hyperbolic_symmetry_group hyperbolic_glide_mirror(double a1, double a2);

class hyperbolic_painter
{
 public:
  hyperbolic_painter() : size(0), pt(POINCARE) {};
  void paint(int sz, hyperbolic_symmetry_group &sym) {
    size=sz;
    pixel_width=2./size;
    sg=sym;
    red.resize(size*size);
    green.resize(size*size);
    blue.resize(size*size);
  }
  void set_projtype(projtype _pt) {pt=_pt;}
  screen_coord toscreen(const planar_coord &p) {
    return screen_coord(size*(p.x+1.)/2.,size*(p.y+1.)/2.);
  }
 protected:
  int size;
  double pixel_width;
  vector<unsigned char> red;
  vector<unsigned char> green;
  vector<unsigned char> blue;
  hyperbolic_symmetry_group sg;
  projtype pt;
};

#endif
