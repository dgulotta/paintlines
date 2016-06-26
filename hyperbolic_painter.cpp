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

#include "hyperbolic_painter.h"
#include "hyperbolic_polygons.h"
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
	return hyperbolic_coord(a.z*b.y-a.y*b.z,a.x*b.z-a.z*b.x,a.x*b.y-a.y*b.x);
}

hyperbolic_coord midpoint(const hyperbolic_coord &a, const hyperbolic_coord &b)
{
	return normalize(a+b);
}

const hyperbolic_transformation hyperbolic_transformation::identity(1.,0.,0.,0.,1.,0.,0.,0.,1.);

hyperbolic_transformation hyperbolic_transformation::reflection(const hyperbolic_coord &c)
{
	double xy=-2*c.x*c.y;
	double xz=2*c.x*c.z;
	double yz=2*c.y*c.z;
	return hyperbolic_transformation(
		1-2*c.x*c.x,
		xy,
		xz,
		xy,
		1-2*c.y*c.y,
		yz,
		-xz,
		-yz,
		1+2*c.z*c.z);
}

hyperbolic_transformation hyperbolic_transformation::rotation_180(const hyperbolic_coord &c)
{
	auto xy=-2*c.x*c.y;
	auto xz=2*c.x*c.z;
	auto yz=2*c.y*c.z;
	return hyperbolic_transformation(
		-2*c.x*c.x-1,
		xy,
		xz,
		xy,
		-2*c.y*c.y-1,
		yz,
		-xz,
		-yz,
		2*c.z*c.z-1);
}

hyperbolic_transformation hyperbolic_transformation::rotation_origin(double a)
{
	double c=cos(a), s=sin(a);
	return hyperbolic_transformation(c,-s,0,s,c,0,0,0,1);
}

hyperbolic_transformation hyperbolic_transformation::rotation(double a, const hyperbolic_coord &h)
{
	double c=cos(a), s=sin(a);
	return hyperbolic_transformation(
		c+(c-1)*h.x*h.x,
		(c-1)*h.x*h.y-s*h.z,
		(1-c)*h.x*h.z+s*h.y,
		(c-1)*h.y*h.x+s*h.z,
		c+(c-1)*h.y*h.y,
		(1-c)*h.y*h.z-s*h.x,
		(c-1)*h.z*h.x+s*h.y,
		(c-1)*h.z*h.y-s*h.x,
		c+(1-c)*h.z*h.z);
}

hyperbolic_transformation hyperbolic_transformation::rotation(const hyperbolic_coord &from, const hyperbolic_coord &to)
{
	hyperbolic_coord cr=cross(to,from);
	double c=from*to;
	double d=1/(1+c);
	return hyperbolic_transformation(
		c-d*cr.x*cr.x,
		-d*cr.x*cr.y-cr.z,
		d*cr.x*cr.z+cr.y,
		-d*cr.y*cr.x+cr.z,
		c-d*cr.y*cr.y,
		d*cr.y*cr.z-cr.x,
		-d*cr.z*cr.x+cr.y,
		-d*cr.z*cr.y-cr.x,
		c+d*cr.z*cr.z);
}

hyperbolic_transformation hyperbolic_transformation::glide_reflection(const hyperbolic_coord &c)
{
	double z=sqrt(1-c*c);
	double d=1/(1-z);
	return hyperbolic_transformation(
		z+d*c.x*c.x,
		-c.z+d*c.x*c.y,
		c.y-d*c.x*c.z,
		c.z+d*c.y*c.x,
		z+d*c.y*c.y,
		-c.x-d*c.y*c.z,
		c.y+d*c.z*c.x,
		-c.x+d*c.z*c.y,
		z-d*c.z*c.z);
}

// this should be the same as the above, except for rounding
// this version seems to trigger numerical instability issues
hyperbolic_transformation hyperbolic_transformation::glide_reflection(const hyperbolic_coord &from, const hyperbolic_coord &to)
{
	hyperbolic_coord c=cross(from,to);
	double z=from*to;
	double d=1/(1-z);
	return hyperbolic_transformation(
		z+d*c.x*c.x,
		-c.z+d*c.x*c.y,
		c.y-d*c.x*c.z,
		c.z+d*c.y*c.x,
		z+d*c.y*c.y,
		-c.x-d*c.y*c.z,
		c.y+d*c.z*c.x,
		-c.x+d*c.z*c.y,
		z-d*c.z*c.z);
}

hyperbolic_transformation hyperbolic_transformation::translation(const hyperbolic_coord &e1, const hyperbolic_coord &e2)
{
	hyperbolic_coord e = cross(e2,e1);
	double c = e1*e2;
	double d = 1/(c+1);
	return hyperbolic_transformation(
		c - d * e.x * e.x,
		-e.z - d * e.x * e.y,
		e.y + d * e.x * e.z,
		e.z - d * e.y * e.x,
		c - d * e.y * e.y,
		-e.x + d * e.y * e.z,
		e.y - d * e.z * e.x,
		-e.x - d * e.z * e.y,
		c + d * e.z * e.z);	
}

planar_coord projection(const hyperbolic_coord &hc,projtype pt)
{
	double denom=(pt==projtype::POINCARE)?(1+hc.z):hc.z;
	return planar_coord(hc.x/denom,hc.y/denom);
}

hyperbolic_coord inverse_projection(const planar_coord &pc,projtype pt)
{
	if(pt==projtype::POINCARE) {
		double r = (1-pc.x*pc.x-pc.y*pc.y)/2;
		return hyperbolic_coord(pc.x/r,pc.y/r,(1-r)/r);
	}
	else {
		double r = sqrt(1-pc.x*pc.x-pc.y*pc.y);
		return hyperbolic_coord(pc.x/r,pc.y/r,1/r);
	}
}

hyperbolic_symmetry_group::hyperbolic_symmetry_group(const group_spec &s,flip_type f)
: generators(s.gens)
{
	vector<hyperbolic_transformation> queue;
	queue.push_back(hyperbolic_transformation::identity);
	transformations.push_back(hyperbolic_transformation::identity);
	for(int i=0;i<queue.size();i++) {
		for(auto it = generators.begin();it!=generators.end();++it) {
			hyperbolic_transformation t = it->right_multiply(queue[i]);
			hyperbolic_coord p=t.inverse(s.pt);
			//if(p.z>ZMAX) continue;
			//if(any_of(generators.begin(),generators.end(),[&](generator &g) { return abs(t(g.edge).z)>ZMAX;})) continue;
			if(abs(t.zz)>ZMAX) continue;
			if(it->inside(p)) continue;
			if(!all_of(generators.begin(),it,bind(&generator::inside,_1,p))) continue;
			queue.push_back(t);
			if(f==flip_type::ALL||
					(f==flip_type::ALTERNATING&&!t.is_flip())||
					(f==flip_type::RANDOM&&random_bool()))
				transformations.push_back(t);
			else
				flips.push_back(t);
		}
	}
	switch(f) {
		case flip_type::ALL:
			fdfunc=&hyperbolic_symmetry_group::fundamental_domain;
			break;
		case flip_type::ALTERNATING:
			fdfunc=&hyperbolic_symmetry_group::fundamental_domain_alternating;
			break;
		case flip_type::RANDOM:
			fdfunc=&hyperbolic_symmetry_group::fundamental_domain_random;
	}
}

hyperbolic_coord hyperbolic_symmetry_group::random_symmetry(const hyperbolic_coord &c) const {
	double i=random_uniform();
	return transformations[pow(transformations.size(),i)](c);
}

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain(const hyperbolic_coord &hc) const {
	hyperbolic_coord c(hc);
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

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain_alternating(const hyperbolic_coord &hc) const {
	hyperbolic_coord c(hc);
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

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain_random(const hyperbolic_coord &hc) const {
	hyperbolic_coord c(hc);
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

const char * bad_param_str = "Bad parameters for hyperbolic group";

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_sabc(int a, int b, int c)
{
	if(a*b+a*c+b*c>=a*b*c)
		throw std::domain_error(bad_param_str);
	hyperbolic_triangle t(M_PI/a,M_PI/b,M_PI/c);
	return group_spec({
			t.reflection1_gen(),
			t.reflection2_gen(),
			t.reflection3_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_a222(int a)
{
	if(a<=2)
		throw std::domain_error(bad_param_str);
	double q = 2*M_PI/(3*a);
	hyperbolic_triangle t(q,q,q);
	return group_spec({
			t.rot180_1_gen(),
			t.rot180_2_gen(),
			t.rot180_3_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_2sab(int a, int b)
{
	if(a+b>=a*b)
		throw std::domain_error(bad_param_str);
	hyperbolic_triangle_isoceles t(M_PI/a,M_PI_2/b);
	return group_spec({
			t.rot180_1_gen(),
			t.reflection2_gen(),
			t.reflection3_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_22sa(int a)
{
	double q=M_PI/(3*a);
	hyperbolic_triangle t(q,q,q);
	return group_spec({
			t.reflection1_gen(),
			t.rot180_2_gen(),
			t.rot180_3_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_ab2(int a, int b)
{
	if(2*(a+b)>=a*b)
		throw std::domain_error(bad_param_str);
	hyperbolic_triangle_isoceles t(2*M_PI/a,M_PI/b);
	return group_spec({
			t.rot180_1_gen(),
			t.rotation2_gen(),
			t.rotation3_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_asb(int a, int b)
{
	if(a+2*b>=a*b)
		throw std::domain_error(bad_param_str);
	hyperbolic_triangle_isoceles t(2*M_PI/a,M_PI_2/b);
	return group_spec({
			t.reflection1_gen(),
			t.rotation2_gen(),
			t.rotation3_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_a2x(int a)
{
	if(a<=2)
		throw std::domain_error(bad_param_str);
	double q=2*M_PI/(3*a);
	hyperbolic_triangle_isoceles t(q,q);
	return group_spec({
			t.rot180_1_gen(),
			t.glide2_gen(),
			t.glide3_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_sax(int a)
{
	double q=M_PI/(3*a);
	hyperbolic_triangle_isoceles t(q,q);
	return group_spec({
			t.reflection1_gen(),
			t.glide2_gen(),
			t.glide3_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_asbc(int a, int b, int c)
{
	if(a*b+a*c+2*b*c>=2*a*b*c)
		throw std::domain_error(bad_param_str);
	hyperbolic_quadrilateral_kite t(2*M_PI/a,M_PI_2/b,M_PI/c);
	return group_spec({
			t.rotation1_gen(),
			t.rotation2_gen(),
			t.reflection3_gen(),
			t.reflection4_gen()
			},t.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_sabcd(int a, int b, int c, int d)
{
	if(a*b*c+a*b*d+a*c*d+b*c*d>=2*a*b*c*d)
		throw std::domain_error(bad_param_str);
	hyperbolic_quadrilateral q(M_PI/a,M_PI/b,M_PI/c,M_PI/d);
	return group_spec({
			q.reflection1_gen(),
			q.reflection2_gen(),
			q.reflection3_gen(),
			q.reflection4_gen()
			},q.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_a2sb(int a, int b)
{
	if(a+2*b>=2*a*b)
		throw std::domain_error(bad_param_str);
	hyperbolic_quadrilateral_kite q(2*M_PI/a,M_PI/(3*b),M_PI/(3*b));
	return group_spec({
			q.rotation1_gen(),
			q.rotation2_gen(),
			q.reflection3_gen(),
			q.rot180_4_gen()
			},q.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_abc(int a, int b, int c)
{
	if(a*b*c<=a*b+b*c+c*a)
		throw std::domain_error(bad_param_str);
	hyperbolic_quadrilateral_kite q(2*M_PI/a,M_PI/c,2*M_PI/b);
	return group_spec({
			q.rotation1_gen(),
			q.rotation2_gen(),
			q.rotation3_gen(),
			q.rotation4_gen()
			},q.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_sasb(int a, int b)
{
	hyperbolic_quadrilateral_trapezoid q(M_PI_2/a,M_PI_2/b);
	return group_spec({
			q.translation1_gen(),
			q.reflection2_gen(),
			q.translation3_gen(),
			q.reflection4_gen()
			},q.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_axx(int a)
{
	hyperbolic_quadrilateral_kite q(M_PI_2/a,M_PI_2/a,M_PI_2/a);
	return group_spec({
			q.glide1_gen(),
			q.glide2_gen(),
			q.glide3_gen(),
			q.glide4_gen(),
			},q.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_ao(int a)
{
	hyperbolic_quadrilateral_square q(M_PI_2/a);
	return group_spec({
			q.translation1_gen(),
			q.translation2_gen(),
			q.translation3_gen(),
			q.translation4_gen(),
			},q.interior_point());
}

hyperbolic_symmetry_group::group_spec hyperbolic_symmetry_group::group_2sabc(int a,int b,int c)
{
	hyperbolic_quadrilateral q(M_PI/a,M_PI/b,M_PI_2/c,M_PI_2/c);
	return group_spec({
			q.reflection1_gen(),
			q.reflection2_gen(),
			q.reflection3_gen(),
			q.rot180_4_gen(),
			},q.interior_point());
}
