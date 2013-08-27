/***************************************************************************
 *   Copyright (C) 2013 by Daniel Gulotta                                  *
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

#ifndef _HYPERBOLIC_POLYGONS_H
#define _HYPERBOLIC_POLYGONS_H

#include "hyperbolic_painter.h"

class hyperbolic_triangle {
public:
	hyperbolic_triangle(double _a1, double _a2, double _a3)
		:a1(_a1), a2(_a2), a3(_a3), sin1(sin(_a1)), cos1(cos(_a1)), sin2(sin(_a2)),
		cos2(cos(_a2)), sin3(sin(_a3)), cos3(cos(_a3)), x1(0), y1(0), z1(1)
	{
		z2=(cos3+cos1*cos2)/(sin1*sin2);
		x2=sqrt(z2*z2-1);
		y2=0;
		z3=(cos2+cos1*cos3)/(sin1*sin3);
		double r3=sqrt(z3*z3-1);
		x3=r3*cos1;
		y3=r3*sin1;
		e1=normalize(cross(hyperbolic_coord(x2,y2,z2),hyperbolic_coord(x3,y3,z3)));
	}

	hyperbolic_coord edge1() const { return e1; }
	
	hyperbolic_coord edge2() const { return hyperbolic_coord(-sin1,cos1,0); }
	
	hyperbolic_coord edge3() const { return hyperbolic_coord(0,-1,0); }
	
	hyperbolic_coord mid1() const { return normalize(hyperbolic_coord(x2+x3,y2+y3,z2+z3)); }
	
	hyperbolic_coord mid2() const { return normalize(hyperbolic_coord(x1+x3,y1+y3,z1+z3)); }
	
	hyperbolic_coord mid3() const { return normalize(hyperbolic_coord(x1+x2,y1+y2,z1+z2)); }

	hyperbolic_coord vertex1() const { return hyperbolic_coord(x1,y1,z1); }

	hyperbolic_coord vertex2() const { return hyperbolic_coord(x2,y2,z2); }

	hyperbolic_coord vertex3() const { return hyperbolic_coord(x3,y3,z3); }

	hyperbolic_coord interior_point() const { return normalize(hyperbolic_coord(x1+x2+x3,y1+y2+y3,z1+z2+z3)); }

	hyperbolic_transformation reflection1() { 
		return hyperbolic_transformation::reflection(edge1());
	}
	
	hyperbolic_transformation reflection2() {
		return hyperbolic_transformation::reflection(edge2());
	}
	
	hyperbolic_transformation reflection3() {
		return hyperbolic_transformation::reflection(edge3());
	}
	
	hyperbolic_transformation rot180_1() {
		return hyperbolic_transformation::rotation_180(mid1());
	}
	
	hyperbolic_transformation rot180_2() {
		return hyperbolic_transformation::rotation_180(mid2());
	}
	
	hyperbolic_transformation rot180_3() {
		return hyperbolic_transformation::rotation_180(mid3());
	}

	generator reflection1_gen() {
		return generator(reflection1(),edge1());
	}

	generator reflection2_gen() {
		return generator(reflection2(),edge2());
	}

	generator reflection3_gen() {
		return generator(reflection3(),edge3());
	}

	generator rot180_1_gen() {
		return generator(rot180_1(),edge1());
	}

	generator rot180_2_gen() {
		return generator(rot180_2(),edge2());
	}

	generator rot180_3_gen() {
		return generator(rot180_3(),edge3());
	}
protected:
	double a1, a2, a3, sin1, cos1, sin2, cos2, sin3, cos3;
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	hyperbolic_coord e1;
};

class hyperbolic_triangle_isoceles : public hyperbolic_triangle {
public:
	hyperbolic_triangle_isoceles(double _a1, double _a2) : hyperbolic_triangle(_a1,_a2,_a2) {}
	hyperbolic_transformation rotation() { return hyperbolic_transformation::rotation_origin(a1); }

	generator rotation2_gen() {
		return generator(rotation(),edge2());
	}

	generator rotation3_gen() {
		return generator(rotation().inverse(),edge3());
	}

	hyperbolic_transformation glide_reflection() {
		return hyperbolic_transformation::glide_reflection(cross(mid3(),mid2()));
	}

	generator glide2_gen() {
		return generator(glide_reflection(),edge2());
	}

	generator glide3_gen() {
		return generator(glide_reflection().inverse(),edge3());
	}
};

/*
 * Once we choose the angles of the quadrilateral, there is one degree
 * of freedom left over.  We need to make an arbitrary choice, so
 * we choose the quadrilateral that has an inscribed circle.
 */
class hyperbolic_quadrilateral {
public:
	hyperbolic_quadrilateral(double _a1, double _a2, double _a3, double _a4)
		: a1(_a1), a2(_a2), a3(_a3), a4(_a4)
	{
		double c1 = cos(a1/2), c2 = cos(a2/2), c3 = cos(a3/2), c4=cos(a4/2);
		double z = 2*sqrt((c1*c2+c3*c4)*(c1*c3+c2*c4)*(c1*c4+c2*c3)/
			((c1+c2+c3-c4)*(c1+c2-c3+c4)*(c1-c2+c3+c4)*(-c1+c2+c3+c4)));
		c1/=z; c2/=z; c3/=z; c4/=z;
		double r = sqrt(z*z-1);
		double c1d=1-2*c1*c1, c2d=1-2*c2*c2, c3d=1-2*c3*c3, c4d=1-2*c4*c4;
		double s1d=2*c1*sqrt(1-c1*c1), s2d=2*c2*sqrt(1-c2*c2), s3d=2*c3*sqrt(1-c3*c3), s4d=2*c4*sqrt(1-c4*c4);
		e1 = hyperbolic_coord(z,0,r);
		e2 = hyperbolic_coord(z*c1d,z*s1d,r);
		e3 = hyperbolic_coord(e2.x*c2d-e2.y*s2d,e2.x*s2d+e2.y*c2d,r);
		e4 = hyperbolic_coord(z*c4d,-z*s4d,r);
	}

	hyperbolic_coord interior_point() { return hyperbolic_coord(0,0,1); }

	hyperbolic_coord edge1() const { return e1; }
	hyperbolic_coord edge2() const { return e2; }
	hyperbolic_coord edge3() const { return e3; }
	hyperbolic_coord edge4() const { return e4; }

	hyperbolic_coord vertex1() const { return normalize(cross(e1,e2)); }
	hyperbolic_coord vertex2() const { return normalize(cross(e2,e3)); }
	hyperbolic_coord vertex3() const { return normalize(cross(e3,e4)); }
	hyperbolic_coord vertex4() const { return normalize(cross(e4,e1)); }

	hyperbolic_coord mid1() const { return normalize(vertex1()+vertex4()); }
	hyperbolic_coord mid2() const { return normalize(vertex2()+vertex1()); }
	hyperbolic_coord mid3() const { return normalize(vertex3()+vertex2()); }
	hyperbolic_coord mid4() const { return normalize(vertex4()+vertex3()); }

	hyperbolic_transformation reflection1() { return hyperbolic_transformation::reflection(e1); } 
	hyperbolic_transformation reflection2() { return hyperbolic_transformation::reflection(e2); }

	hyperbolic_transformation rot180_4() {
		hyperbolic_coord p = normalize(cross(e4,e1)), q = normalize(cross(e3,e4));
		return hyperbolic_transformation::rotation_180(normalize(p+q));
	}

	generator reflection1_gen() { return generator(reflection1(),e1); }
	generator reflection2_gen() { return generator(reflection2(),e2); }
	generator reflection3_gen() { return generator(hyperbolic_transformation::reflection(e3),e3); }
	generator reflection4_gen() { return generator(hyperbolic_transformation::reflection(e4),e4); }
	generator rot180_4_gen() { return generator(rot180_4(),e4); }
protected:
	double a1, a2, a3, a4;
	hyperbolic_coord e1, e2, e3, e4;
};

class hyperbolic_quadrilateral_kite : public hyperbolic_quadrilateral {
public:
	hyperbolic_quadrilateral_kite(double _a1, double _a2, double _a3) : hyperbolic_quadrilateral(_a1,_a2,_a3,_a2) {}

	hyperbolic_transformation rotation1() {
		hyperbolic_coord p = normalize(cross(e2,e1));
		return hyperbolic_transformation::rotation(a1,p);
	}

	hyperbolic_transformation rotation3() {
		hyperbolic_coord p = normalize(cross(e4,e3));
		return hyperbolic_transformation::rotation(a3,p);
	}

	generator rotation1_gen() { return generator(rotation1(),e1); }
	generator rotation2_gen() { return generator(rotation1().inverse(),e2); }
	generator rotation3_gen() { return generator(rotation3(),e3); }
	generator rotation4_gen() { return generator(rotation3().inverse(),e4); }
	generator glide1_gen() { return generator(hyperbolic_transformation::glide_reflection(cross(mid2(),mid1())),e1); }
	generator glide2_gen() { return generator(hyperbolic_transformation::glide_reflection(cross(mid1(),mid2())),e2); }
	generator glide3_gen() { return generator(hyperbolic_transformation::glide_reflection(cross(mid4(),mid3())),e3); }
	generator glide4_gen() { return generator(hyperbolic_transformation::glide_reflection(cross(mid3(),mid4())),e4); }
};

class hyperbolic_quadrilateral_trapezoid : public hyperbolic_quadrilateral {
public:
	hyperbolic_quadrilateral_trapezoid(double _a1, double _a2) : hyperbolic_quadrilateral(_a1,_a1,_a2,_a2) {}

	generator translation1_gen() { return generator(hyperbolic_transformation::translation(e1,e3),e1); }
	generator translation3_gen() { return generator(hyperbolic_transformation::translation(e3,e1),e3); }
};

class hyperbolic_quadrilateral_square : public hyperbolic_quadrilateral_trapezoid {
public:
	hyperbolic_quadrilateral_square(double _a1) : hyperbolic_quadrilateral_trapezoid(_a1,_a1) {}
	
	generator translation2_gen() { return generator(hyperbolic_transformation::translation(e2,e4),e2); }
	generator translation4_gen() { return generator(hyperbolic_transformation::translation(e4,e2),e4); }
};

#endif
