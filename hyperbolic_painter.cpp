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

#include "hyperbolic_painter.h"
#include "randgen.h"

using namespace std;
using namespace std::placeholders;

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

const hyperbolic_transformation hyperbolic_transformation::identity = hyperbolic_transformation(1.,0.,0.,0.,1.,0.,0.,0.,1.);

hyperbolic_transformation hyperbolic_transformation::reflection(const hyperbolic_coord &c)
{
  hyperbolic_transformation t;
  t.xx=1-2*c.x*c.x;
  t.yx=t.xy=-2*c.x*c.y;
  t.zx=-(t.xz=2*c.x*c.z);
  t.yy=1-2*c.y*c.y;
  t.zy=-(t.yz=2*c.y*c.z);
  t.zz=1+2*c.z*c.z;
  return t;
}

hyperbolic_transformation hyperbolic_transformation::rotation_180(const hyperbolic_coord &c)
{
  hyperbolic_transformation t;
  t.xx=-2*c.x*c.x-1;
  t.yx=t.xy=-2*c.x*c.y;
  t.zx=-(t.xz=2*c.x*c.z);
  t.yy=-2*c.y*c.y-1;
  t.zy=-(t.yz=2*c.y*c.z);
  t.zz=2*c.z*c.z-1;
  return t;
}

hyperbolic_transformation hyperbolic_transformation::rotation_origin(double a)
{
  hyperbolic_transformation t;
  double cosine=cos(a), sine=sin(a);
  t.xx=cosine;
  t.xy=sine;
  t.xz=0;
  t.yx=-sine;
  t.yy=cosine;
  t.yz=0;
  t.zx=0;
  t.zy=0;
  t.zz=1;
  return t;
}

hyperbolic_transformation hyperbolic_transformation::rotation(double a, const hyperbolic_coord &c)
{
  double cosine=cos(a), sine=sin(a);
  hyperbolic_transformation t;
  t.xx=cosine+(cosine-1)*c.x*c.x;
  t.xy=(cosine-1)*c.x*c.y-sine*c.z;
  t.xz=(1-cosine)*c.x*c.z+sine*c.y;
  t.yx=(cosine-1)*c.y*c.x+sine*c.z;
  t.yy=cosine+(cosine-1)*c.y*c.y;
  t.yz=(1-cosine)*c.y*c.z-sine*c.x;
  t.zx=(cosine-1)*c.z*c.x+sine*c.y;
  t.zy=(cosine-1)*c.z*c.y-sine*c.x;
  t.zz=cosine+(1-cosine)*c.z*c.z;
  return t;
}

hyperbolic_transformation hyperbolic_transformation::glide_reflection(const hyperbolic_coord &c)
{
  double z=sqrt(1-c*c);
  double d=1/(1-z);
  hyperbolic_transformation t;
  t.xx=z+d*c.x*c.x;
  t.xy=-c.z+d*c.x*c.y;
  t.xz=c.y-d*c.x*c.z;
  t.yx=c.z+d*c.y*c.x;
  t.yy=z+d*c.y*c.y;
  t.yz=-c.x-d*c.y*c.z;
  t.zx=c.y+d*c.z*c.x;
  t.zy=-c.x+d*c.z*c.y;
  t.zz=z-d*c.z*c.z;
  return t;					       
}

hyperbolic_transformation translation_helper(const hyperbolic_coord &e1, const hyperbolic_coord &e2,double o)
{
	hyperbolic_coord e = cross(e2,e1);
	double c = e1*e2;
	double d = 1/(c+o);
	hyperbolic_transformation t;
	t.xx = c - d * e.x * e.x;
	t.xy = -e.z - d * e.x * e.y;
	t.xz = e.y + d * e.x * e.z;
	t.yx = e.z - d * e.y * e.x;
	t.yy = c - d * e.y * e.y;
	t.yz = -e.x + d * e.y * e.z;
	t.zx = e.y - d * e.z * e.x;
	t.zy = -e.x - d * e.z * e.y;
	t.zz = c + d * e.z * e.z;
	return t;
}

hyperbolic_transformation hyperbolic_transformation::translation(const hyperbolic_coord &e1, const hyperbolic_coord &e2)
{
	return translation_helper(e1,e2,1);
}

planar_coord poincare_projection(const hyperbolic_coord &hc)
{
  return planar_coord(hc.x/(1+hc.z),hc.y/(1+hc.z));
}

planar_coord klein_projection(const hyperbolic_coord &hc)
{
  return planar_coord(hc.x/hc.z,hc.y/hc.z);
}

hyperbolic_coord inverse_poincare_projection(const planar_coord &pc)
{
	double r = (1-pc.x*pc.x-pc.y*pc.y)/2;
	return hyperbolic_coord(pc.x/r,pc.y/r,(1-r)/r);
}

hyperbolic_coord inverse_klein_projection(const planar_coord &pc)
{
	double r = sqrt(1-pc.x*pc.x-pc.y*pc.y);
	return hyperbolic_coord(pc.x/r,pc.y/r,1/r);
}

class hyperbolic_triangle {
public:
	hyperbolic_triangle(double _a1, double _a2, double _a3)
		:a1(_a1), a2(_a2), a3(_a3), sin1(sin(_a1)), cos1(cos(_a1)), sin2(sin(_a2)),
		cos2(cos(_a2)), sin3(sin(_a3)), cos3(cos(_a3)), x1(0), y1(0), z1(1)
	{
		z2=(cos2+cos1*cos3)/(sin1*sin3);
		x2=sqrt(z2*z2-1);
		y2=0;
		z3=(cos3+cos1*cos2)/(sin1*sin2);
		double r3=sqrt(z3*z3-1);
		x3=r3*cos1;
		y3=r3*sin1;
		e1=normalize(cross(hyperbolic_coord(x2,y2,z2),hyperbolic_coord(x3,y3,z3)));
	}

	hyperbolic_coord edge1() { return e1; }
	
	hyperbolic_coord edge2() { return hyperbolic_coord(-sin1,cos1,0); }
	
	hyperbolic_coord edge3() { return hyperbolic_coord(0,-1,0); }
	
	hyperbolic_coord mid1() { return normalize(hyperbolic_coord(x2+x3,y2+y3,z2+z3)); }
	
	hyperbolic_coord mid2() { return normalize(hyperbolic_coord(x1+x3,y1+y3,z1+z3)); }
	
	hyperbolic_coord mid3() { return normalize(hyperbolic_coord(x1+x2,y1+y2,z1+z2)); }

	hyperbolic_coord interior_point() { return normalize(hyperbolic_coord(x1+x2+x3,y1+y2+y3,z1+z2+z3)); }

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

	hyperbolic_coord vertex1() { return normalize(cross(e1,e2)); }
	hyperbolic_coord vertex2() { return normalize(cross(e2,e3)); }
	hyperbolic_coord vertex3() { return normalize(cross(e3,e4)); }
	hyperbolic_coord vertex4() { return normalize(cross(e4,e1)); }

	hyperbolic_coord mid1() { return normalize(vertex1()+vertex4()); }
	hyperbolic_coord mid2() { return normalize(vertex2()+vertex1()); }
	hyperbolic_coord mid3() { return normalize(vertex3()+vertex2()); }
	hyperbolic_coord mid4() { return normalize(vertex4()+vertex3()); }

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

hyperbolic_symmetry_group::hyperbolic_symmetry_group(const vector<generator> &g, const hyperbolic_coord &pt,flip_type f)
	: generators(g)
{
	hyperbolic_transformation trans;
	hyperbolic_coord edge;
	vector<hyperbolic_transformation> queue;
	queue.push_back(hyperbolic_transformation::identity);
	transformations.push_back(hyperbolic_transformation::identity);
	for(int i=0;i<queue.size();i++) {
		for(auto it = generators.begin();it!=generators.end();++it) {
			hyperbolic_transformation t = it->right_multiply(queue[i]);
			hyperbolic_coord p=t.inverse(pt);
			//if(p.z>ZMAX) continue;
			//if(any_of(generators.begin(),generators.end(),[&](generator &g) { return abs(t(g.edge).z)>ZMAX;})) continue;
			if(abs(t.zz)>ZMAX) continue;
			if(it->inside(p)) continue;
			if(!all_of(generators.begin(),it,bind(&generator::inside,_1,p))) continue;
			queue.push_back(t);
			if(f==FLIP_ALL||
				(f==FLIP_ALTERNATING&&!t.is_flip())||
				(f==FLIP_RANDOM&&random_bool()))
				transformations.push_back(t);
			else
				flips.push_back(t);
		}
	}
	switch(f) {
	case FLIP_ALL:
		fdfunc=&hyperbolic_symmetry_group::fundamental_domain;
		break;
	case FLIP_ALTERNATING:
		fdfunc=&hyperbolic_symmetry_group::fundamental_domain_alternating;
		break;
	case FLIP_RANDOM:
		fdfunc=&hyperbolic_symmetry_group::fundamental_domain_random;
	}
}

hyperbolic_coord hyperbolic_symmetry_group::random_symmetry(const hyperbolic_coord &c) {
	double i=random_uniform();
	return transformations[pow(transformations.size(),i)](c);
}

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain(const hyperbolic_coord &hc) {
	hyperbolic_coord c = hc;
	hyperbolic_transformation trans;
	hyperbolic_coord edge;
	bool any=true;
	while(any) {
		any=false;
		for(auto &t: generators) {
			if(!t.inside(c)) {
				c=t(c);
				any=true;
				break;
			}
		}
	}
	return make_tuple(c,false);
}

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain_alternating(const hyperbolic_coord &hc) {
	hyperbolic_coord c = hc;
	hyperbolic_transformation trans;
	hyperbolic_coord edge;
	bool any=true;
	bool flip=false;
	while(any) {
		any=false;
		for(auto &t: generators) {
			if(!t.inside(c)) {
				c=t(c);
				flip^=t.is_flip;
				any=true;
				break;
			}
		}
	}
	return make_tuple(c,flip);
}

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain_random(const hyperbolic_coord &hc) {
	hyperbolic_coord c = hc;
	hyperbolic_transformation trans;
	hyperbolic_coord edge;
	bool any=true;
	long index=0;
	while(any) {
		any=false;
		int i=0;
		for(auto &t: generators) {
			++i;
			if(!t.inside(c)) {
				c=t(c);
				index=index*(generators.size()+1)+i;
				any=true;
				break;
			}
		}
	}
	const auto &it = random_flips.find(index);
	if(it!=random_flips.end())
		return make_tuple(c,it->second);
	else {
		bool b = random_bool();
		random_flips.emplace(index,b);
		return make_tuple(c,b);
	}
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_sabc(int a, int b, int c, flip_type f)
{
	if(a*b+a*c+b*c>=a*b*c)
		return nullptr;
	hyperbolic_triangle t(M_PI/a,M_PI/b,M_PI/c);
	return new hyperbolic_symmetry_group({
		t.reflection1_gen(),
		t.reflection2_gen(),
		t.reflection3_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_a222(int a, flip_type f)
{
	if(a<=2)
		return nullptr;
	double q = 2*M_PI/(3*a);
	hyperbolic_triangle t(q,q,q);
	return new hyperbolic_symmetry_group({
		t.rot180_1_gen(),
		t.rot180_2_gen(),
		t.rot180_3_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_2sab(int a, int b, flip_type f) 
{
	if(a+b>=a*b)
		return nullptr;
	hyperbolic_triangle_isoceles t(M_PI/a,M_PI_2/b);
	return new hyperbolic_symmetry_group({
		t.rot180_1_gen(),
		t.reflection2_gen(),
		t.reflection3_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_22sa(int a, flip_type f)
{
	double q=M_PI/(3*a);
	hyperbolic_triangle t(q,q,q);
	return new hyperbolic_symmetry_group({
		t.reflection1_gen(),
		t.rot180_2_gen(),
		t.rot180_3_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_ab2(int a, int b, flip_type f)
{
	if(2*(a+b)>=a*b)
		return nullptr;
	hyperbolic_triangle_isoceles t(2*M_PI/a,M_PI/b);
	return new hyperbolic_symmetry_group({
		t.rot180_1_gen(),
		t.rotation2_gen(),
		t.rotation3_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_asb(int a, int b, flip_type f)
{
	if(a+2*b>=a*b)
		return nullptr;
	hyperbolic_triangle_isoceles t(2*M_PI/a,M_PI_2/b);
	return new hyperbolic_symmetry_group({
		t.reflection1_gen(),
		t.rotation2_gen(),
		t.rotation3_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_a2x(int a, flip_type f)
{
	if(a<=2)
		return nullptr;
	double q=2*M_PI/(3*a);
	hyperbolic_triangle_isoceles t(q,q);
	return new hyperbolic_symmetry_group({
		t.rot180_1_gen(),
		t.glide2_gen(),
		t.glide3_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_sax(int a, flip_type f)
{
	double q=M_PI/(3*a);
	hyperbolic_triangle_isoceles t(q,q);
	return new hyperbolic_symmetry_group({
		t.reflection1_gen(),
		t.glide2_gen(),
		t.glide3_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_asbc(int a, int b, int c, flip_type f)
{
	if(a*b+a*c+2*b*c>=2*a*b*c)
		return nullptr;
	hyperbolic_quadrilateral_kite t(2*M_PI/a,M_PI_2/b,M_PI/c);
	hyperbolic_coord p = t.interior_point();
	generator g1 = t.rotation1_gen(), g2 = t.rotation2_gen(), g3 = t.reflection3_gen(), g4 = t.reflection4_gen();
	return new hyperbolic_symmetry_group({
		t.rotation1_gen(),
		t.rotation2_gen(),
		t.reflection3_gen(),
		t.reflection4_gen()
	},t.interior_point(),f);
}

hyperbolic_symmetry_group *hyperbolic_symmetry_group::group_sabcd(int a, int b, int c, int d, flip_type f)
{
	if(a*b*c+a*b*d+a*c*d+b*c*d>=2*a*b*c*d)
		return nullptr;
	hyperbolic_quadrilateral q(M_PI/a,M_PI/b,M_PI/c,M_PI/d);
	return new hyperbolic_symmetry_group({
		q.reflection1_gen(),
		q.reflection2_gen(),
		q.reflection3_gen(),
		q.reflection4_gen()
	},q.interior_point(),f);
}

hyperbolic_symmetry_group *hyperbolic_symmetry_group::group_a2sb(int a, int b, flip_type f)
{
	if(a+2*b>=2*a*b)
		return nullptr;
	hyperbolic_quadrilateral_kite q(2*M_PI/a,M_PI/(3*b),M_PI/(3*b));
	return new hyperbolic_symmetry_group({
		q.rotation1_gen(),
		q.rotation2_gen(),
		q.reflection3_gen(),
		q.rot180_4_gen()
	},q.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_abc(int a, int b, int c, flip_type f)
{
	if(a*b*c<=a*b+b*c+c*a)
		return nullptr;
	hyperbolic_quadrilateral_kite q(2*M_PI/a,M_PI/c,2*M_PI/b);
	return new hyperbolic_symmetry_group({
		q.rotation1_gen(),
		q.rotation2_gen(),
		q.rotation3_gen(),
		q.rotation4_gen()
	},q.interior_point(),f);
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_ssa(int a, flip_type f)
{
	hyperbolic_quadrilateral_trapezoid q(M_PI_2,M_PI_2/a);
	return new hyperbolic_symmetry_group({
		q.translation1_gen(),
		q.reflection2_gen(),
		q.translation3_gen(),
		q.reflection4_gen()
	},q.interior_point(),f);
	return nullptr;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_axx(int a, flip_type f)
{
	hyperbolic_quadrilateral_kite q(M_PI_2/a,M_PI_2/a,M_PI_2/a);
	return new hyperbolic_symmetry_group({
		q.glide1_gen(),
		q.glide2_gen(),
		q.glide3_gen(),
		q.glide4_gen(),
	},q.interior_point(),f);
	return nullptr;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_ao(int a, flip_type f)
{
	hyperbolic_quadrilateral_square q(M_PI_2/a);
	return new hyperbolic_symmetry_group({
		q.translation1_gen(),
		q.translation2_gen(),
		q.translation3_gen(),
		q.translation4_gen(),
	},q.interior_point(),f);
}
