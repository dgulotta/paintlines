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

#include "hyperbolic_painter.h"

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

planar_coord poincare_projection(const hyperbolic_coord &hc)
{
  return planar_coord(hc.x/(1+hc.z),hc.y/(1+hc.z));
}

planar_coord klein_projection(const hyperbolic_coord &hc)
{
  return planar_coord(hc.x/hc.z,hc.y/hc.z);
}

hyperbolic_symmetry_group hyperbolic_3mirror(int n1, int n2, int n3)
{
  double cos1=cos(M_PI/n1);
  double sin1=sin(M_PI/n1);
  double cos2=cos(M_PI/n2);
  double sin2=sin(M_PI/n2);
  double cos3=cos(M_PI/n3);
  double sin3=sin(M_PI/n3);
  double z2=(cos2+cos1*cos3)/(sin1*sin3);
  double r2=sqrt(z2*z2-1.);
  double z3=(cos3+cos1*cos2)/(sin1*sin2);
  double r3=sqrt(z3*z3-1.);
  hyperbolic_symmetry_group s;
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection (hyperbolic_coord(0.,1.,0.)));
  s.trans2=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection (hyperbolic_coord(-sin1,cos1,0.)));
  s.trans3=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection(normalize(cross(hyperbolic_coord(r2,0.,z2),
		     hyperbolic_coord(r3*cos1,r3*sin1,z3)))));
  return s;
}

hyperbolic_symmetry_group hyperbolic_3_180(double a1, double a2, double a3)
{
  double cos1=cos(a1);
  double sin1=sin(a1);
  double cos2=cos(a2);
  double sin2=sin(a2);
  double cos3=cos(a3);
  double sin3=sin(a3);
  double z2=(cos2+cos1*cos3)/(sin1*sin3);
  double r2=sqrt(z2*z2-1.);
  double z3=(cos3+cos1*cos2)/(sin1*sin2);
  double r3=sqrt(z3*z3-1.);
  hyperbolic_symmetry_group s;
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_180
     (hyperbolic_coord(sqrt((z2-1.)/2.),0.,sqrt((z2+1.)/2.))));
  s.trans3=auto_ptr<hyperbolic_transformation>
     (new hyperbolic_rotation_180
      (normalize(hyperbolic_coord(r3*cos1+r2,r3*sin1,z2+z3))));
  r3=sqrt((z3-1.)/2.);
  s.trans2=auto_ptr<hyperbolic_transformation>
     (new hyperbolic_rotation_180 
      (hyperbolic_coord(r3*cos1,r3*sin1,sqrt((z3+1.)/2.))));
  return s;
}

hyperbolic_symmetry_group hyperbolic_2mirror_180(int n1, double a2, double a3)
{
  double cos1=cos(M_PI/n1);
  double sin1=sin(M_PI/n1);
  double cos2=cos(a2);
  double sin2=sin(a2);
  double cos3=cos(a3);
  double sin3=sin(a3);
  double z2=(cos2+cos1*cos3)/(sin1*sin3);
  double r2=sqrt(z2*z2-1.);
  double z3=(cos3+cos1*cos2)/(sin1*sin2);
  double r3=sqrt(z3*z3-1.);
  hyperbolic_symmetry_group s;
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection (hyperbolic_coord(0.,1.,0.)));
  s.trans2=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection (hyperbolic_coord(-sin1,cos1,0.)));
  s.trans3=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_180
     (normalize(hyperbolic_coord(r3*cos1+r2,r3*sin1,z2+z3))));
  return s;
}

hyperbolic_symmetry_group hyperbolic_mirror_2_180(double a1, double a2,
						  double a3)
{
  double cos1=cos(a1);
  double sin1=sin(a1);
  double cos2=cos(a2);
  double sin2=sin(a2);
  double cos3=cos(a3);
  double sin3=sin(a3);
  double z2=(cos2+cos1*cos3)/(sin1*sin3);
  double r2=sqrt(z2*z2-1.);
  double z3=(cos3+cos1*cos2)/(sin1*sin2);
  double r3=sqrt(z3*z3-1.);
  hyperbolic_symmetry_group s;
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection
     (hyperbolic_coord(sqrt((z2-1)/2.),0.,sqrt(z2+1)/2.)));
  s.trans3=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection
     (normalize(cross(hyperbolic_coord(r2,0.,z2),
		      hyperbolic_coord(r3*cos1,r3*sin1,z3)))));
  r3=sqrt((z3-1.)/2.);
  s.trans2=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_180
     (hyperbolic_coord(r3*cos1,r3*sin1,sqrt((z3+1.)/2.))));
  return s;
}

hyperbolic_symmetry_group hyperbolic_180_rotation(int n1, int n2)
{
  hyperbolic_symmetry_group s;
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_origin(n1));
  s.trans2=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_origin(-n1));
  double z=cos(M_PI/n2)/sin(M_PI/n1);
  s.trans3=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_180(hyperbolic_coord(sqrt(z*z-1.),0,z)));
  return s;
}

hyperbolic_symmetry_group hyperbolic_mirror_rotation(int n1, int n2)
{
  hyperbolic_symmetry_group s;
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_origin(n1));
  s.trans2=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_origin(-n1));
  double z=cos(M_PI_2/n2)/sin(M_PI/n1);
  s.trans3=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection
     (hyperbolic_coord(-z*cos(M_PI/n1),0,sqrt(z*z-1.))));
  return s;
}

hyperbolic_symmetry_group hyperbolic_3rotation(int n)
{
  hyperbolic_symmetry_group s;
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation(n,hyperbolic_coord(0.,0.,1.)));
  double cosine=cos(2.*M_PI/n);
  double sine=sin(2.*M_PI/n);
  double z=cosine/(1.-cosine);
  double r=sqrt(z*z-1.);
  s.trans2=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation(n,hyperbolic_coord(r,0.,z)));
  s.trans3=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation(n,hyperbolic_coord(r*cosine,r*sine,z)));
  return s;
}

hyperbolic_symmetry_group hyperbolic_glide_180(double a1, double a2)
{
  hyperbolic_symmetry_group s;
  double cos1=cos(a1);
  double sin1=sin(a1);
  double cos2=cos(a2);
  double sin2=sin(a2);
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_glide_reflection(a1,cos1*(1.+cos2)/(sin1*sin2)));
  s.trans2=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_glide_reflection(-a1,cos1*(1.+cos2)/(sin1*sin2)));
  double z=cos2/sqrt((1.-cos1)/2.);
  s.trans3=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_rotation_180(hyperbolic_coord(sqrt(z*z-1.),0.,z)));
  return s;
}

hyperbolic_symmetry_group hyperbolic_glide_mirror(double a1, double a2)
{
  hyperbolic_symmetry_group s;
  double cos1=cos(a1);
  double sin1=sin(a1);
  double cos2=cos(a2);
  double sin2=sin(a2);
  s.trans1=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_glide_reflection(a1,cos1*(1.+cos2)/(sin1*sin2)));
  s.trans2=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_glide_reflection(-a1,cos1*(1.+cos2)/(sin1*sin2)));
  double z=cos2/sqrt((1.-cos1)/2.);
  s.trans3=auto_ptr<hyperbolic_transformation>
    (new hyperbolic_reflection
    (normalize(hyperbolic_coord(-z*sqrt((1.+cos1)/2),0,z))));
}
