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

#include "../color.h"
#include "../hyperbolic_painter.h"
#include "../hyperbolic_polygons.h"
#include "../interpolate.h"
#include "../symmetric_canvas.h"

#include <functional>
#include <tuple>

using std::function;
using std::make_tuple;
using std::tie;
using std::tuple;

typedef function<tuple<double,double>(const hyperbolic_coord &)> coord_transformer;

coord_transformer triangle_transformer(const hyperbolic_triangle &t, double x1, double y1, double x2, double y2, double x3, double y3)
{
	hyperbolic_coord e1n = t.edge1()/(t.edge1()*t.vertex1());
	hyperbolic_coord e2n = t.edge2()/(t.edge2()*t.vertex2());
	hyperbolic_coord e3n = t.edge3()/(t.edge3()*t.vertex3());
	return [=] (const hyperbolic_coord &c) {
		double d1 = e1n * c, d2 = e2n * c, d3 = e3n * c;
		double d = d1+d2+d3;
		d1/=d; d2/=d; d3/=d;
		return make_tuple(d1*x1+d2*x2+d3*x3,d1*y1+d2*y2+d3*y3);
	};
}

coord_transformer quadrilateral_transformer(const hyperbolic_quadrilateral &q, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	hyperbolic_coord ea1 = cross(q.vertex4(),q.vertex1());
	hyperbolic_coord eb1 = cross(q.vertex3(),q.vertex1())+cross(q.vertex4(),q.vertex2());
	hyperbolic_coord ec1 = cross(q.vertex3(),q.vertex2());
	hyperbolic_coord ea2 = cross(q.vertex1(),q.vertex2());
	hyperbolic_coord eb2 = cross(q.vertex1(),q.vertex3())+cross(q.vertex4(),q.vertex2());
	hyperbolic_coord ec2 = cross(q.vertex4(),q.vertex3());
	return [=] (const hyperbolic_coord &c) {
		double a1 = ea1*c, b1 = eb1*c, c1=ec1*c;
		double a2 = ea2*c, b2 = eb2*c, c2=ec2*c;
		double d1 = sqrt(b1*b1-4*a1*c1), d2 = sqrt(b2*b2-4*a2*c2);
		double p1 = 2*c1/(-b1-d1+2*c1), q1 = 2*a1/(-b1+d1+2*a1);
		double p2 = 2*c2/(-b2-d2+2*c2), q2 = 2*a2/(-b2+d2+2*a2);
		double f1 = p1*p2, f2 = q1*p2, f3 = q1*q2, f4 = p1*q2;
		return make_tuple(f1*x1+f2*x2+f3*x3+f4*x4,f1*y1+f2*y2+f3*y3+f4*y4);
	};
}

canvas<color_t> make_hyperbolic(const symmetric_canvas<color_t> &img, projtype pt, int newsize)
{
	hyperbolic_symmetry_group *sg;
	coord_transformer trans;
	int size = img.size();
	canvas<color_t> newimg(newsize,newsize);
	switch(img.group()) {
	case SYM_CM:
		sg = hyperbolic_symmetry_group::group_sax(2);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(M_PI/6,M_PI/6),0,size,size,size,0,0);
		break;
	case SYM_CMM:
		sg = hyperbolic_symmetry_group::group_2sab(3,2);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(M_PI/3,M_PI/4),.5*size,.5*size,size,0,0,0);
		break;
	case SYM_P1:
		sg = hyperbolic_symmetry_group::group_ao(2);
		trans = quadrilateral_transformer(hyperbolic_quadrilateral_square(M_PI/4),0,0,0,size,size,size,size,0);
		break;
	case SYM_P2:
		sg = hyperbolic_symmetry_group::group_a222(3);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(2*M_PI/9,2*M_PI/9),0,0,0,size,size,0);
		break;
	case SYM_P3:
		sg = hyperbolic_symmetry_group::group_abc(3,3,4);
		trans = quadrilateral_transformer(hyperbolic_quadrilateral_kite(2*M_PI/3,M_PI/4,2*M_PI/3),size/3.,size/3.,size,0,2*size/3.,-size/3.,0,0);
		break;
	case SYM_P31M:
		sg = hyperbolic_symmetry_group::group_asb(3,4);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(2*M_PI/3,M_PI/8),size/3.,size/3.,size,0,0,0);
		break;
	case SYM_P3M1:
		sg = hyperbolic_symmetry_group::group_sabc(4,3,3);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(M_PI/4,M_PI/3),0,0,size/3.,size/3.,2*size/3.,-size/3.);
		break;
	case SYM_P4:
		sg = hyperbolic_symmetry_group::group_ab2(5,4);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(2*M_PI/5,M_PI/4),0,0,.5*size,.5*size,.5*size,-.5*size);
		break;
	case SYM_P4G:
		sg = hyperbolic_symmetry_group::group_asb(5,2);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(2*M_PI/5,M_PI/4),0,0,0,.5*size,.5*size,0);
		break;
	case SYM_P4M:
		sg = hyperbolic_symmetry_group::group_sabc(5,4,2);
		trans = triangle_transformer(hyperbolic_triangle(M_PI/5,M_PI/4,M_PI/2),0,0,.5*size,.5*size,.5*size,0);
		break;
	case SYM_P6:
		sg = hyperbolic_symmetry_group::group_ab2(7,3);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(2*M_PI/7,M_PI/3),0,0,size/3.,size/3.,2*size/3.,-size/3.);
		break;
	case SYM_P6M:
		sg = hyperbolic_symmetry_group::group_sabc(7,3,2);
		trans = triangle_transformer(hyperbolic_triangle(M_PI/7,M_PI/3,M_PI/2),0,0,size/3.,size/3.,.5*size,0);
		break;
	case SYM_PG:
		sg = hyperbolic_symmetry_group::group_axx(2);
		trans = quadrilateral_transformer(hyperbolic_quadrilateral_square(M_PI/4),.5*size,.5*size,size,0,.5*size,-.5*size,0,0);
		break;
	case SYM_PGG:
		sg = hyperbolic_symmetry_group::group_a2x(3);
		trans = triangle_transformer(hyperbolic_triangle_isoceles(2*M_PI/9,2*M_PI/9),0,0,.5*size,.5*size,.5*size,-.5*size);
		break;
	case SYM_PM:
		sg = hyperbolic_symmetry_group::group_sasb(1,2);
		trans = quadrilateral_transformer(hyperbolic_quadrilateral_trapezoid(M_PI/2,M_PI/4),0,0,0,size,.5*size,size,.5*size,0);
		break;
	case SYM_PMG:
		sg = hyperbolic_symmetry_group::group_a2sb(3,1);
		trans = quadrilateral_transformer(hyperbolic_quadrilateral_kite(2*M_PI/3,M_PI/3,M_PI/3),0,.5*size,.25*size,.75*size,.25*size,-.25*size,-.25*size,.25*size);
		break;
	case SYM_PMM:
		sg = hyperbolic_symmetry_group::group_sabcd(3,2,2,2);
		trans = quadrilateral_transformer(hyperbolic_quadrilateral(M_PI/3,M_PI/2,M_PI/2,M_PI/2),0,0,0,.5*size,.5*size,.5*size,.5*size,0);
		break;
	}
	auto iproj = (pt==KLEIN)?&inverse_klein_projection:&inverse_poincare_projection;
	coord_converter conv(newsize);
	for(int j=0;j<newsize;j++)
		for(int i=0;i<newsize;i++) {
			planar_coord pc = conv.fromscreen(screen_coord(i,j));
			if(pc.x*pc.x+pc.y*pc.y>=.999) continue;
			hyperbolic_coord hc = sg->fundamental_domain_point((*iproj)(pc));
			double xd,yd;
			tie(xd,yd)=trans(hc);
			newimg(i,j)=combine_colors(img.as_wrap_canvas(),interpolate_linear(xd,yd));
		}
	delete sg;
	return newimg;
}

const double short_factor = pow(4./3.,.25);
const double long_factor = pow(12.,.25);

wrap_canvas<color_t> hexagonal_stretch(const symmetric_canvas<color_t> &src)
{
	wrap_canvas<color_t> dest(round(src.size()*long_factor),round(src.size()*short_factor));
	for(int y=0;y<dest.height();y++) {
		double yn = double(y)*src.size()/dest.height();
		for(int x=0;x<dest.width();x++) {
			double xn = double(x)*src.size()/dest.width();
			dest(x,y)=combine_colors(src.as_wrap_canvas(),interpolate_cubic(xn+yn,xn-yn));
		}
	}
	return dest;
}
