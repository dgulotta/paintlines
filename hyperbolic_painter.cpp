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
#include <cstdlib>

#define ZMAX 20
#define EPSILON 1.e-2

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

hyperbolic_transformation hyperbolic_transformation::rotation(int n, const hyperbolic_coord &c)
{
  double cosine=cos(2.*M_PI/n), sine=sin(2.*M_PI/n);
  hyperbolic_transformation t;
  t.xx=cosine+(1-cosine)*c.x*c.x;
  t.xy=(1-cosine)*c.x*c.y+sine*c.z;
  t.xz=(cosine-1)*c.x*c.z+sine*c.y;
  t.yx=(1-cosine)*c.y*c.x-sine*c.z;
  t.yy=cosine+(1-cosine)*c.y*c.y;
  t.yz=(cosine-1)*c.y*c.z-sine*c.x;
  t.zx=(cosine-1)*c.z*c.x-sine*c.y;
  t.zy=(cosine-1)*c.z*c.y+sine*c.x;
  t.zz=cosine+(1-cosine)*c.z*c.z;
  return t;
}

hyperbolic_transformation hyperbolic_transformation::glide_reflection(const hyperbolic_coord &c, double r)
{
  double z=sqrt(r*r+1.);
  hyperbolic_transformation t;
  t.xx=z+(-1-z)*c.x*c.x;
  t.xy=-r*c.z+(-1-z)*c.x*c.y;
  t.xz=r*c.y-(-1-z)*c.x*c.z;
  t.yx=r*c.z+(-1-z)*c.y*c.x;
  t.yy=z+(-1-z)*c.y*c.y;
  t.yz=-r*c.x-(-1-z)*c.y*c.z;
  t.zx=r*c.y+(-1-z)*c.z*c.x;
  t.zy=-r*c.x+(-1-z)*c.z*c.y;
  t.zz=z-(-1-z)*c.z*c.z;
  return t;					       
}

double det(const hyperbolic_transformation &t)
{
  return t.xx*t.yy*t.zz+t.xy*t.yz*t.zx+t.xz*t.zy*t.yx
    -t.xx*t.yz*t.zy-t.xz*t.zx*t.yy-t.xy*t.yx*t.zz;
};

planar_coord poincare_projection(const hyperbolic_coord &hc)
{
  return planar_coord(hc.x/(1+hc.z),hc.y/(1+hc.z));
}

planar_coord klein_projection(const hyperbolic_coord &hc)
{
  return planar_coord(hc.x/hc.z,hc.y/hc.z);
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
protected:
	double a1, a2, a3, sin1, cos1, sin2, cos2, sin3, cos3;
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	hyperbolic_coord e1;
};

class hyperbolic_triangle_isoceles : public hyperbolic_triangle {
public:
	hyperbolic_triangle_isoceles(double _a1, double _a2) : hyperbolic_triangle(_a1,_a2,_a2) {}
	hyperbolic_transformation rotation() { return hyperbolic_transformation::rotation_origin(a1); }
	hyperbolic_transformation glide_reflection() {
		hyperbolic_coord m2 = mid2(), m3 = mid3();
		hyperbolic_coord l = cross(m2,m3);
		double r = sqrt(-l*l);
		l/=r;
		return hyperbolic_transformation::glide_reflection(l,r);
	}
};

void hyperbolic_symmetry_group::make_tiles
(const hyperbolic_transformation &t1, const hyperbolic_transformation &t2,
 const hyperbolic_transformation &t3, flip_type f)
{
  alternating=(f!=FLIP_ALL);
  if(alternating) flipped.push_back(true);
  bool flip1=(det(t1)*t1.zz<0);
  bool flip2=(det(t2)*t2.zz<0);
  bool flip3=(det(t3)*t3.zz<0);
  int i;
  vector<hyperbolic_tile>::iterator it2;
  hyperbolic_tile t;
  hyperbolic_transformation tt1, tt2, tt3;
  for(i=0;i<tiles.size();i++) {
    tt1=tiles[i].t*t1*tiles[i].t.inverse();
    t.edge1=tt1(tiles[i].edge1);
    t.edge2=tt1(tiles[i].edge2);
    t.edge3=tt1(tiles[i].edge3);
    if(fabs(t.edge1.z)<ZMAX&&fabs(t.edge2.z)<ZMAX&&fabs(t.edge3.z)<ZMAX) {
      t.t=tt1*(tiles[i].t);
      for(it2=tiles.begin();it2!=tiles.end();it2++) {
	if(fabs(t.t.xx-it2->t.xx)<EPSILON&&fabs(t.t.xy-it2->t.xy)<EPSILON&&
	   fabs(t.t.xz-it2->t.xz)<EPSILON&&fabs(t.t.yx-it2->t.yx)<EPSILON&&
	   fabs(t.t.yy-it2->t.yy)<EPSILON&&fabs(t.t.yz-it2->t.yz)<EPSILON&&
	   fabs(t.t.zx-it2->t.zx)<EPSILON&&fabs(t.t.zy-it2->t.zy)<EPSILON&&
	   fabs(t.t.zz-it2->t.zz)<EPSILON) break;
      }
      if(it2==tiles.end()) {
	tiles.push_back(t);
	if(f==FLIP_ALTERNATING) flipped.push_back(flip1?!flipped[i]:flipped[i]);
	else if(f==FLIP_RANDOM) flipped.push_back(rand()&1==1);
      }
    }
    tt2=tiles[i].t*t2*tiles[i].t.inverse();
    t.edge1=tt2(tiles[i].edge1);
    t.edge2=tt2(tiles[i].edge2);
    t.edge3=tt2(tiles[i].edge3);
    if(fabs(t.edge1.z)<ZMAX&&fabs(t.edge2.z)<ZMAX&&fabs(t.edge3.z)<ZMAX) {
      t.t=tt2*(tiles[i].t);
      for(it2=tiles.begin();it2!=tiles.end();it2++) {
	if(fabs(t.t.xx-it2->t.xx)<EPSILON&&fabs(t.t.xy-it2->t.xy)<EPSILON&&
	   fabs(t.t.xz-it2->t.xz)<EPSILON&&fabs(t.t.yx-it2->t.yx)<EPSILON&&
	   fabs(t.t.yy-it2->t.yy)<EPSILON&&fabs(t.t.yz-it2->t.yz)<EPSILON&&
	   fabs(t.t.zx-it2->t.zx)<EPSILON&&fabs(t.t.zy-it2->t.zy)<EPSILON&&
	   fabs(t.t.zz-it2->t.zz)<EPSILON) break;
      }
      if(it2==tiles.end()) {
	tiles.push_back(t);
	if(f==FLIP_ALTERNATING) flipped.push_back(flip2?!flipped[i]:flipped[i]);
	else if(f==FLIP_RANDOM) flipped.push_back(rand()&1==1);
      }
    }
    tt3=tiles[i].t*t3*tiles[i].t.inverse();
    t.edge1=tt3(tiles[i].edge1);
    t.edge2=tt3(tiles[i].edge2);
    t.edge3=tt3(tiles[i].edge3);
    if(fabs(t.edge1.z)<ZMAX&&fabs(t.edge2.z)<ZMAX&&fabs(t.edge3.z)<ZMAX) {
      t.t=tt3*(tiles[i].t);
      for(it2=tiles.begin();it2!=tiles.end();it2++) {
	if(fabs(t.t.xx-it2->t.xx)<EPSILON&&fabs(t.t.xy-it2->t.xy)<EPSILON&&
	   fabs(t.t.xz-it2->t.xz)<EPSILON&&fabs(t.t.yx-it2->t.yx)<EPSILON&&
	   fabs(t.t.yy-it2->t.yy)<EPSILON&&fabs(t.t.yz-it2->t.yz)<EPSILON&&
	   fabs(t.t.zx-it2->t.zx)<EPSILON&&fabs(t.t.zy-it2->t.zy)<EPSILON&&
	   fabs(t.t.zz-it2->t.zz)<EPSILON) break;
      }
      if(it2==tiles.end()) {
	tiles.push_back(t);
	if(f==FLIP_ALTERNATING) flipped.push_back(flip3?!flipped[i]:flipped[i]);
	else if(f==FLIP_RANDOM) flipped.push_back(rand()&1==1);
      }
    }
  }
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_sabc(int n1, int n2, int n3, flip_type f)
{
  if(n1*n2+n1*n3+n2*n3>=n1*n2*n3)
	return nullptr;
  hyperbolic_triangle t(M_PI/n1,M_PI/n2,M_PI/n3);
  hyperbolic_symmetry_group *s=new hyperbolic_symmetry_group;
  s->tiles.resize(1);
  s->tiles[0].t=hyperbolic_transformation::identity;
  s->tiles[0].edge1=t.edge3();
  s->tiles[0].edge2=t.edge2();
  s->tiles[0].edge3=t.edge1();
  s->make_tiles(t.reflection3(),t.reflection2(),t.reflection1(),f);
  return s;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_a222(int a, flip_type f)
{
  if(a<=2)
	return nullptr;
  double q = 2*M_PI/(3*a);
  hyperbolic_triangle t(q,q,q);
  hyperbolic_symmetry_group *s=new hyperbolic_symmetry_group;
  s->tiles.resize(1);
  s->tiles[0].t=hyperbolic_transformation::identity;
  s->tiles[0].edge1=t.edge3();
  s->tiles[0].edge2=t.edge2();
  s->tiles[0].edge3=t.edge1();
  s->make_tiles(t.rot180_3(),t.rot180_2(),t.rot180_1(),f);
  return s;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_2sab(int a, int b, flip_type f)
{
  if(a+b>=a*b)
    return nullptr;
  hyperbolic_triangle_isoceles t(M_PI/a,M_PI_2/b);
  hyperbolic_symmetry_group *s=new hyperbolic_symmetry_group;
  s->tiles.resize(1);
  s->tiles[0].t=hyperbolic_transformation::identity;
  s->tiles[0].edge1=t.edge3();
  s->tiles[0].edge2=t.edge2();
  s->tiles[0].edge3=t.edge1();
  s->make_tiles(t.reflection3(),t.reflection2(),t.rot180_1(),f);
  return s;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_22sa(int a, flip_type f)
{
  double q=M_PI/(3*a);
  hyperbolic_triangle t(q,q,q);
  hyperbolic_symmetry_group *s=new hyperbolic_symmetry_group;
  s->tiles.resize(1);
  s->tiles[0].t=hyperbolic_transformation::identity;
  s->tiles[0].edge1=t.edge3();
  s->tiles[0].edge2=t.edge2();
  s->tiles[0].edge3=t.edge1();
  s->make_tiles(t.rot180_3(),t.rot180_2(),t.reflection1(),f);
  return s;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_ab2(int a, int b, flip_type f)
{
  if(2*(a+b)>=a*b)
	return nullptr;
  hyperbolic_triangle_isoceles t(2*M_PI/a,M_PI/b);
  hyperbolic_symmetry_group *s=new hyperbolic_symmetry_group;
  s->tiles.resize(1);
  s->tiles[0].t=hyperbolic_transformation::identity;
  s->tiles[0].edge1=t.edge3();
  s->tiles[0].edge2=t.edge2();
  s->tiles[0].edge3=t.edge1();
  s->make_tiles(t.rotation(),t.rotation().inverse(),t.rot180_1(),f);
  return s;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_asb(int a, int b, flip_type f)
{
  if(a+2*b>=a*b)
	return nullptr;
  hyperbolic_triangle_isoceles t(2*M_PI/a,M_PI_2/b);
  hyperbolic_symmetry_group *s=new hyperbolic_symmetry_group;
  s->tiles.resize(1);
  s->tiles[0].t=hyperbolic_transformation::identity;
  s->tiles[0].edge1=t.edge3();
  s->tiles[0].edge2=t.edge2();
  s->tiles[0].edge3=t.edge1();
  s->make_tiles(t.rotation(),t.rotation().inverse(),t.reflection1(),f);
  return s;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_a2x(int a, flip_type f)
{
  if(a<=2)
	return nullptr;
  hyperbolic_symmetry_group *s=new hyperbolic_symmetry_group;
  s->tiles.resize(1);
  double q=2*M_PI/(3*a);
  hyperbolic_triangle_isoceles t(q,q);
  s->tiles[0].t=hyperbolic_transformation::identity;
  s->tiles[0].edge1=t.edge3();
  s->tiles[0].edge2=t.edge2();
  s->tiles[0].edge3=t.edge1();
  s->make_tiles(t.glide_reflection(),t.glide_reflection().inverse(),t.rot180_1(),f);
  return s;
}

hyperbolic_symmetry_group * hyperbolic_symmetry_group::group_sax(int a, flip_type f)
{
  double q=M_PI/(3*a);
  hyperbolic_triangle_isoceles t(q,q);
  hyperbolic_symmetry_group *s=new hyperbolic_symmetry_group;
  s->tiles.resize(1);
  s->tiles[0].t=hyperbolic_transformation::identity;
  s->tiles[0].edge1=t.edge3();
  s->tiles[0].edge2=t.edge2();
  s->tiles[0].edge3=t.edge1();
  s->make_tiles(t.glide_reflection(),t.glide_reflection().inverse(),t.reflection1(),f);
  return s;
}
