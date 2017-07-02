/***************************************************************************
 *   Copyright (C) 2005-2008, 2013, 2016 by Daniel Gulotta                 *
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

#ifndef _HYPERBOLIC_H
#define _HYPERBOLIC_H

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
	return hyperbolic_coord(a*b.x,a*b.y,a*b.z);
}

inline hyperbolic_coord operator * (const hyperbolic_coord &a, double b)
{
	return b*a;
}

inline hyperbolic_coord operator / (const hyperbolic_coord &a, double b)
{
	return hyperbolic_coord(a.x/b,a.y/b,a.z/b);
}

inline hyperbolic_coord operator + (const hyperbolic_coord &a,
		const hyperbolic_coord &b)
{
	return hyperbolic_coord(a.x+b.x,a.y+b.y,a.z+b.z);
}

inline hyperbolic_coord operator - (const hyperbolic_coord &a)
{
	return hyperbolic_coord(-a.x,-a.y,-a.z);
}

inline hyperbolic_coord operator - (const hyperbolic_coord &a,
		const hyperbolic_coord &b)
{
	return hyperbolic_coord(a.x-b.x,a.y-b.y,a.z-b.z);
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

hyperbolic_coord midpoint(const hyperbolic_coord &a, const hyperbolic_coord &b);

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

enum class projtype {POINCARE, KLEIN};

planar_coord projection(const hyperbolic_coord &hc, projtype pt);
hyperbolic_coord inverse_projection(const planar_coord &hc, projtype pt);

class hyperbolic_transformation
{
	public:
		static const hyperbolic_transformation identity;
		static hyperbolic_transformation reflection(const hyperbolic_coord &c);
		static hyperbolic_transformation rotation_180(const hyperbolic_coord &c);
		static hyperbolic_transformation rotation_origin(double a);
		static hyperbolic_transformation rotation(double a, const hyperbolic_coord &c);
		static hyperbolic_transformation glide_reflection(const hyperbolic_coord &c);
		static hyperbolic_transformation glide_reflection(const hyperbolic_coord &from, const hyperbolic_coord &to);
		static hyperbolic_transformation translation(const hyperbolic_coord &e1, const hyperbolic_coord &e2);
		hyperbolic_transformation(double _xx, double _xy, double _xz, double _yx,
				double _yy, double _yz, double _zx, double _zy,
				double _zz)
			: xx(_xx), xy(_xy), xz(_xz), yx(_yx), yy(_yy), yz(_yz), zx(_zx), zy(_zy),
			zz(_zz) {}
		hyperbolic_coord operator () (const hyperbolic_coord &c) const {
			return hyperbolic_coord(xx*c.x+xy*c.y+xz*c.z,
				yx*c.x+yy*c.y+yz*c.z,zx*c.x+zy*c.y+zz*c.z);
		}
		hyperbolic_coord inverse(const hyperbolic_coord &c) const {
			return hyperbolic_coord(xx*c.x+yx*c.y-zx*c.z,
				xy*c.x+yy*c.y-zy*c.z,-xz*c.x-yz*c.y+zz*c.z);
		}
		hyperbolic_transformation inverse() const {
			return hyperbolic_transformation(xx,yx,-zx,xy,yy,-zy,-xz,-yz,zz);
		}
		bool is_flip() const {
			return xx*yy*zz+xy*yz*zx+xz*zy*yx
				-xx*yz*zy-xz*zx*yy-xy*yx*zz<0;
		}
		double xx, xy, xz, yx, yy, yz, zx, zy, zz;
		// it would probably be faster to use opengl for this
};

inline hyperbolic_transformation operator *
(const hyperbolic_transformation &t1, const hyperbolic_transformation &t2)
{
	return hyperbolic_transformation(
		t1.xx*t2.xx+t1.xy*t2.yx+t1.xz*t2.zx,
		t1.xx*t2.xy+t1.xy*t2.yy+t1.xz*t2.zy,
		t1.xx*t2.xz+t1.xy*t2.yz+t1.xz*t2.zz,
		t1.yx*t2.xx+t1.yy*t2.yx+t1.yz*t2.zx,
		t1.yx*t2.xy+t1.yy*t2.yy+t1.yz*t2.zy,
		t1.yx*t2.xz+t1.yy*t2.yz+t1.yz*t2.zz,
		t1.zx*t2.xx+t1.zy*t2.yx+t1.zz*t2.zx,
		t1.zx*t2.xy+t1.zy*t2.yy+t1.zz*t2.zy,
		t1.zx*t2.xz+t1.zy*t2.yz+t1.zz*t2.zz);
}

class coord_converter
{
	public:
		coord_converter(int sz) : size(sz) {}
		screen_coord toscreen(const planar_coord &p) {
			return screen_coord(size*(p.x+1.)/2.,size*(p.y+1.)/2.);
		}
		planar_coord fromscreen(const screen_coord &s) {
			return planar_coord((2*s.x+1.)/size-1,(2*s.y+1.)/size-1);
		}
	private:
		int size;
};

#endif
