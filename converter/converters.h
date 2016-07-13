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

#ifndef _HYPERBOLIC_CONVERTER_H
#define _HYPERBOLIC_CONVERTER_H

#include "../color.h"
#include "../hyperbolic_group.h"
#include "../interpolate.h"
#include "../symmetric_canvas.h"

class hyperbolic_polygon;

canvas<color_t> make_hyperbolic(const symmetric_canvas<color_t> &img, projtype pt, int newsize);

wrap_canvas<color_t> hexagonal_stretch(const symmetric_canvas<color_t> &src);

typedef std::function<point<double>(const hyperbolic_coord &)> coord_transformer;

coord_transformer triangle_transformer(const hyperbolic_polygon &t, double x1, double y1, double x2, double y2, double x3, double y3);

coord_transformer quadrilateral_transformer(const hyperbolic_polygon &q, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);

template<typename Grid, typename T, typename I>
canvas<color_t> copy_hyperbolic(const Grid &img, projtype pt, int newsize, const fundamental_domain &fd, const T &trans, const I& interpolate)
{
	canvas<color_t> newimg(newsize,newsize);
	coord_converter conv(newsize);
	for(int j=0;j<newsize;j++)
		for(int i=0;i<newsize;i++) {
			planar_coord pc = conv.fromscreen(screen_coord(i,j));
			if(pc.x*pc.x+pc.y*pc.y>=.999) continue;
			hyperbolic_coord hc = fundamental_domain_point(fd.generators,inverse_projection(pc,pt));
			double xd,yd;
			std::tie(xd,yd)=trans(hc);
			newimg(i,j)=combine_colors(img,interpolate(xd,yd));
		}
	return newimg;
}

#endif
