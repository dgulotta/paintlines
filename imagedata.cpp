/***************************************************************************
 *   Copyright (C) 2014, 2016 by Daniel Gulotta                            *
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

#include "imagedata.h"
#include "color.h"

QImage makeImage(const CanvasView<color_t> &v)
{
	const auto &src = *(v.canvas_view);
	QImage image(src.width(),src.height(),QImage::Format_RGB32);
	for(int j=0;j<src.height();j++)
		for(int i=0;i<src.width();i++) {
			color_t col = src(i,j);
			image.setPixel(i,j,qRgb(col.red,col.green,col.blue));
		}
	if(v.sym_view)
		image.setText("SymmetryGroup",QString::number(
			static_cast<int>(v.sym_view->group())));
	return image;
}
