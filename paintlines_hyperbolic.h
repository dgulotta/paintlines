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

class hyperbolic_rotation_180: public hyperbolic_transformation
{
 public:
    hyperbolic_rotation_180(const hyperbolic_coord &c) : normal(c) {}
    virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
	return -(c+2.*(normal*c)*normal);
    }
 private:
    hyperbolic_coord normal;
};

class hyperbolic_rotation_origin : public hyperbolic_transformation
{
 public:
    hyperbolic_rotation_origin(int n)
	: cosine(cos(2.*M_PI/n)), sine(sin(2.*M_PI/n)) {}
    virtual hyperbolic_coord operator () (const hyperbolic_coord &c) const {
	hyperbolic_coord ans;
	ans.x=cosine*c.x-sine*c.y;
	ans.y=cosine*c.y+sine*c.x;
	ans.z=c.z;
	return ans;
    }
 private:
    double cosine;
    double sine;
};

class hyperbolic_symmetry_group
{
    friend hyperbolic_symmetry_group
	hyperbolic_3mirror(int m1, int m2, int m3);
 public:
    ~hyperbolic_symmetry_group();
 private:
    hyperbolic_symmetry_group();
    hyperbolic_transformation *trans1;
    hyperbolic_transformation *trans2;
    hyperbolic_transformation *trans3;
};

#endif
