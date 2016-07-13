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

#include "../color.h"
#include "../hyperbolic_group.h"
#include "../hyperbolic_polygons.h"
#include "../interpolate.h"
#include "../symmetric_canvas.h"
#include "converters.h"

#include <functional>
#include <tuple>

using std::function;
using std::make_tuple;
using std::tie;
using std::tuple;

typedef function<point<double>(const hyperbolic_coord &)> coord_transformer;

coord_transformer triangle_transformer(const hyperbolic_polygon &t, double x1, double y1, double x2, double y2, double x3, double y3)
{
	hyperbolic_coord e1n = t.edge(2)/(t.edge(2)*t.vertex(1));
	hyperbolic_coord e2n = t.edge(0)/(t.edge(0)*t.vertex(2));
	hyperbolic_coord e3n = t.edge(1)/(t.edge(1)*t.vertex(0));
	return [=] (const hyperbolic_coord &c) {
		double d1 = e1n * c, d2 = e2n * c, d3 = e3n * c;
		double d = d1+d2+d3;
		d1/=d; d2/=d; d3/=d;
		return make_tuple(d1*x1+d2*x2+d3*x3,d1*y1+d2*y2+d3*y3);
	};
}

coord_transformer quadrilateral_transformer(const hyperbolic_polygon &q, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	hyperbolic_coord ea1 = cross(q.vertex(0),q.vertex(1));
	hyperbolic_coord eb1 = cross(q.vertex(3),q.vertex(1))+cross(q.vertex(0),q.vertex(2));
	hyperbolic_coord ec1 = cross(q.vertex(3),q.vertex(2));
	hyperbolic_coord ea2 = cross(q.vertex(1),q.vertex(2));
	hyperbolic_coord eb2 = cross(q.vertex(1),q.vertex(3))+cross(q.vertex(0),q.vertex(2));
	hyperbolic_coord ec2 = cross(q.vertex(0),q.vertex(3));
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
	fundamental_domain_family fdf;
	fundamental_domain fd;
	coord_transformer trans;
	int size = img.size();
	switch(img.group()) {
	case symgroup::CM:
		fdf = fundamental_domain_family({{1,true},{0,true},{2,true}});
		fd = fdf.domain({2});
		trans = triangle_transformer(fd.polygon,0,size,size,size,0,0);
		break;
	case symgroup::CMM:
		fdf = fundamental_domain_family({{0,true},{1,true},{2,false}});
		fd = fdf.domain({3,2});
		trans = triangle_transformer(fd.polygon,.5*size,.5*size,size,0,0,0);
		break;
	case symgroup::P1:
		fdf = fundamental_domain_family({{2,false},{3,false},{0,false},{1,false}});
		fd = fdf.domain({2});
		trans = quadrilateral_transformer(fd.polygon,0,0,0,size,size,size,size,0);
		break;
	case symgroup::P2:
		fdf = fundamental_domain_family({{0,false},{1,false},{2,false}});
		fd = fdf.domain({3});
		trans = triangle_transformer(fd.polygon,0,0,0,size,size,0);
		break;
	case symgroup::P3:
		fdf = fundamental_domain_family({{1,false},{0,false},{3,false},{2,false}});
		fd = fdf.domain({3,3,4});
		trans = quadrilateral_transformer(fd.polygon,size/3.,size/3.,size,0,2*size/3.,-size/3.,0,0);
		break;
	case symgroup::P31M:
		fdf = fundamental_domain_family({{1,false},{0,false},{2,true}});
		fd = fdf.domain({3,4});
		trans = triangle_transformer(fd.polygon,size/3.,size/3.,size,0,0,0);
		break;
	case symgroup::P3M1:
		fdf = fundamental_domain_family({{0,true},{1,true},{2,true}});
		fd = fdf.domain({4,3,3});
		trans = triangle_transformer(fd.polygon,0,0,size/3.,size/3.,2*size/3.,-size/3.);
		break;
	case symgroup::P4:
		fdf = fundamental_domain_family({{1,false},{0,false},{2,false}});
		fd = fdf.domain({5,4});
		trans = triangle_transformer(fd.polygon,0,0,.5*size,.5*size,.5*size,-.5*size);
		break;
	case symgroup::P4G:
		fdf = fundamental_domain_family({{1,false},{0,false},{2,true}});
		fd = fdf.domain({5,2});
		trans = triangle_transformer(fd.polygon,0,0,0,.5*size,.5*size,0);
		break;
	case symgroup::P4M:
		fdf = fundamental_domain_family({{0,true},{1,true},{2,true}});
		fd = fdf.domain({2,4,5});
		trans = triangle_transformer(fd.polygon,0,0,.5*size,.5*size,.5*size,0);
		break;
	case symgroup::P6:
		fdf = fundamental_domain_family({{1,false},{0,false},{2,false}});
		fd = fdf.domain({3,7});
		trans = triangle_transformer(fd.polygon,0,0,size/3.,size/3.,2*size/3.,-size/3.);
		break;
	case symgroup::P6M:
		fdf = fundamental_domain_family({{0,true},{1,true},{2,true}});
		fd = fdf.domain({2,3,7});
		trans = triangle_transformer(fd.polygon,0,0,size/3.,size/3.,.5*size,0);
		break;
	case symgroup::PG:
		fdf = fundamental_domain_family({{1,true},{0,true},{3,true},{2,true}});
		fd = fdf.domain({2});
		trans = quadrilateral_transformer(fd.polygon,.5*size,.5*size,size,0,.5*size,-.5*size,0,0);
		break;
	case symgroup::PGG:
		fdf = fundamental_domain_family({{1,true},{0,true},{2,false}});
		fd = fdf.domain({3});
		trans = triangle_transformer(fd.polygon,0,0,.5*size,.5*size,.5*size,-.5*size);
		break;
	case symgroup::PM:
		fdf = fundamental_domain_family({{2,false},{1,true},{0,false},{3,true}});
		fd = fdf.domain({1,2});
		trans = quadrilateral_transformer(fd.polygon,0,0,0,size,.5*size,size,.5*size,0);
		break;
	case symgroup::PMG:
		fdf = fundamental_domain_family({{1,false},{0,false},{2,true},{3,false}});
		fd = fdf.domain({3,1});
		trans = quadrilateral_transformer(fd.polygon,0,.5*size,.25*size,.75*size,.25*size,-.25*size,-.25*size,.25*size);
		break;
	case symgroup::PMM:
		fdf = fundamental_domain_family({{0,true},{1,true},{2,true},{3,true}});
		fd = fdf.domain({3,2,2,2});
		trans = quadrilateral_transformer(fd.polygon,0,0,0,.5*size,.5*size,.5*size,.5*size,0);
		break;
	}
	return copy_hyperbolic(img,pt,newsize,fd,trans,interpolate_linear);
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
