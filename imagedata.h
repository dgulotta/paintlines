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
 
#ifndef _IMAGEDATA_H
#define _IMAGEDATA_H

#include <vector>
#include <QImage>

struct color_t;
class layer;
template<typename T>
class symmetric_canvas;
template<typename T>
class wrap_canvas;
template<typename T>
class canvas;

struct ImageData
{
	ImageData(const QImage &i, bool t=false, const symmetric_canvas<color_t> *sc=nullptr,
		const std::vector<layer> *l=nullptr, const ImageData *p=nullptr);
	ImageData() : ImageData(QImage()) {}
	ImageData(const canvas<color_t> &c, bool t=false, const symmetric_canvas<color_t> *sc=nullptr,
		const std::vector<layer> *l=nullptr, const ImageData *p=nullptr);
	ImageData(const wrap_canvas<color_t> &wc, const ImageData *p=nullptr);
	ImageData(const symmetric_canvas<color_t> &sc, const std::vector<layer> *l=nullptr);
	QImage img;
	bool tileable;
	const symmetric_canvas<color_t> *sym_canvas;
	const std::vector<layer> *layers;
	const ImageData *parent;
};

#endif
