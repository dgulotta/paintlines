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

#ifndef _PAINTLINES_HYPERBOLIC_H
#define _PAINTLINES_HYPERBOLIC_H

#include <vector>
#include <cmath>
using namespace std;

struct hyperbolic_coord
{
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

inline hyperbolic_coord & operator += (hyperbolic_coord &a
				       const hyperbolic_coord &b)
{
  a.x+=b.x;
  a.y+=b.y;
  a.z+=b.z;
  return a;
}

inline hyperbolic_coord & operator -= (hyperbolic_coord &a
				       const hyperbolic_coord &b)
{
  a.x-=b.x;
  a.y-=b.y;
  a.z-=b.z;
  return a;
}

hyperbolic_coord normalize(const hyperbolic_coord &c)
{
  double n=c*c;
  if(n>0.) return c/sqrt(n);
  else if(n<0.) return c/sqrt(-n);
  else return c;
}

hyperbolic_coord & inplace_normalize(hyperbolic_coord &c)
{
  double n=c*c;
  if(n>0.) c/=sqrt(n);
  else if(n<0.) c/=sqrt(-n);
  return c;
}

hyperbolic_coord cross(const hyperbolic_coord &a, const hyperbolic_coord &b)
{
  hyperbolic_coord ans;
  ans.x=a.z*b.y-a.y*b.z;
  ans.y=a.x*b.z-a.z*b.x;
  ans.z=a.x*b.y-a.y*b.x;
  return ans;
}

class hyperbolic_transformation
{
 public:
  virtual ~hyperbolic_transformation() {}
  virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const;
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
  hyperbolic_rotation_180(const hyperbolic_coord &c) : normal(c) {}
  virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
    return -(c+2.*(normal*c)*normal);
  }
 private:
  hyperbolic_coord normal;
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
  hyperbolic_rotatation_180 r;
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
  friend hyperbolic_symmetry_group hyperbolic_rotation_180(int n1, int n2);
  friend hyperbolic_symmetry_group hyperbolic_rotation_mirror(int n1, int n2);
 public:
  hyperbolic_symmetry_group(hyperbolic_symmetry_group &s) {
    trans1=s.trans1;
    trans2=s.trans2;
    trans3.s.trans3;
    s.trans1=NULL;
    s.trans2=NULL;
    s.trans3=NULL;
  }
  hyperbolic_symmetry_group & operator = (hyperbolic_symmetry_group &s) {
    if(this!=&s) {
      trans1=s.trans1;
      trans2=s.trans2;
      trans3.s.trans3;
      s.trans1=NULL;
      s.trans2=NULL;
      s.trans3=NULL;
    }
    return s;
  }
  ~hyperbolic_symmetry_group() {
    if(trans1) delete trans1;
    if(trans2) delete trans2;
    if(trans3) delete trans3;
  }
 private:
  hyperbolic_symmetry_group() {}
  hyperbolic_transformation *trans1;
  hyperbolic_transformation *trans2;
  hyperbolic_transformation *trans3;
};

hyperbolic_symmetry_group hyperbolic_3mirror(int n1, int n2, int n3);
hyperbolic_symmetry_group hyperbolic_3_180(double a1, double a2, double a3);
hyperbolic_symmetry_group hyperbolic_2mirror_180(int n1, double a2, double a3);
hyperbolic_symmetry_group hyperbolic_mirror_2_180(double a1, double a2,
						  double a3);
hyperbolic_symmetry_group hyperbolic_rotation_180(int n1, int n2);
hyperbolic_symmetry_group hyperbolic_rotation_mirror(int n1, int n2);
hyperbolic_symmetry_group hyperbolic_3rotation(int n);
hyperbolic_symmetry_group hyperbolic_glide_180(double a1, double a2);
hyperbolic_symmetry_group hyperbolic_glide_mirror(double a1, double a2,
						  double a3);

#endif
