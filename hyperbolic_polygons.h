/***************************************************************************
 *   Copyright (C) 2013, 2016 by Daniel Gulotta                            *
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

#include <array>
#include "hyperbolic_painter.h"

class hyperbolic_triangle {
public:
	hyperbolic_triangle(double a1, double a2, double a3)
		: hyperbolic_triangle(a1,a2,a3,compute_coords(a1,a2,a3)) {}

	hyperbolic_coord edge1() const { return e1; }
	hyperbolic_coord edge2() const { return e2; }
	hyperbolic_coord edge3() const { return e3; }
	hyperbolic_coord mid1() const { return midpoint(v2,v3); }
	hyperbolic_coord mid2() const { return midpoint(v1,v3); }
	hyperbolic_coord mid3() const { return midpoint(v1,v2); }
	hyperbolic_coord vertex1() const { return v1; }
	hyperbolic_coord vertex2() const { return v2; }
	hyperbolic_coord vertex3() const { return v3; }
	hyperbolic_coord interior_point() const { return normalize(v1+v2+v3); }

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
	double a1,a2,a3;
	hyperbolic_coord e1,e2,e3,v1,v2,v3;
private:
	static std::array<hyperbolic_coord,6> compute_coords(double a1, double a2, double a3) {
		double s1=sin(a1), c1=cos(a1), s2=sin(a2), c2=cos(a2), s3=sin(a3), c3=cos(a3);
		double z2=(c3+c1*c2)/(s1*s2);
		hyperbolic_coord v2(sqrt(z2*z2-1),0,z2);
		double z3=(c2+c1*c3)/(s1*s3);
		double r3=sqrt(z3*z3-1);
		hyperbolic_coord v3(r3*c1,r3*s1,z3);
		return {
			normalize(cross(v2,v3)),
			hyperbolic_coord(-s1,c1,0),
			hyperbolic_coord(0,-1,0),
			hyperbolic_coord(0,0,1),
			v2,
			v3};
	}
	hyperbolic_triangle(double _a1, double _a2, double _a3,
		const std::array<hyperbolic_coord,6> &t)
		: a1(_a1), a2(_a2), a3(_a3), e1(t[0]), e2(t[1]), e3(t[2]),
			v1(t[3]), v2(t[4]), v3(t[5]) {}
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
		: hyperbolic_quadrilateral(_a1,_a2,_a3,_a4,compute_coords(_a1,_a2,_a3,_a4)) {}

	hyperbolic_coord interior_point() { return hyperbolic_coord(0,0,1); }

	hyperbolic_coord edge1() const { return e1; }
	hyperbolic_coord edge2() const { return e2; }
	hyperbolic_coord edge3() const { return e3; }
	hyperbolic_coord edge4() const { return e4; }

	hyperbolic_coord vertex1() const { return normalize(cross(e1,e2)); }
	hyperbolic_coord vertex2() const { return normalize(cross(e2,e3)); }
	hyperbolic_coord vertex3() const { return normalize(cross(e3,e4)); }
	hyperbolic_coord vertex4() const { return normalize(cross(e4,e1)); }

	hyperbolic_coord mid1() const { return midpoint(vertex1(),vertex4()); }
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
	double a1,a2,a3,a4;
	hyperbolic_coord e1, e2, e3, e4;
private:
	static std::array<hyperbolic_coord,4> compute_coords(double a1, double a2, double a3, double a4) {
		double c1 = cos(a1/2), c2 = cos(a2/2), c3 = cos(a3/2), c4=cos(a4/2);
		double z = 2*sqrt((c1*c2+c3*c4)*(c1*c3+c2*c4)*(c1*c4+c2*c3)/
			((c1+c2+c3-c4)*(c1+c2-c3+c4)*(c1-c2+c3+c4)*(-c1+c2+c3+c4)));
		c1/=z; c2/=z; c3/=z; c4/=z;
		double r = sqrt(z*z-1);
		double c1d=1-2*c1*c1, c2d=1-2*c2*c2, c3d=1-2*c3*c3, c4d=1-2*c4*c4;
		double s1d=2*c1*sqrt(1-c1*c1), s2d=2*c2*sqrt(1-c2*c2), s3d=2*c3*sqrt(1-c3*c3), s4d=2*c4*sqrt(1-c4*c4);
		hyperbolic_coord e2(z*c1d,z*s1d,r);
		return {
			hyperbolic_coord(z,0,r),
			e2,
			hyperbolic_coord(e2.x*c2d-e2.y*s2d,e2.x*s2d+e2.y*c2d,r),
			hyperbolic_coord(z*c4d,-z*s4d,r)};
	}
	hyperbolic_quadrilateral(double _a1, double _a2, double _a3, double _a4,
		const std::array<hyperbolic_coord,4> &t)
		: a1(_a1), a2(_a2), a3(_a3), a4(_a4), e1(t[0]), e2(t[1]), e3(t[2]), e4(t[3]) {}
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
