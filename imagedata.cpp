/***************************************************************************
 *   Copyright (C) 2014 by Daniel Gulotta                                  *
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
#include "mainform.h"
#include "symmetric_canvas.h"

ImageData::ImageData(const canvas<color_t> &c, bool t, const symmetric_canvas<color_t> *sc,
		const std::vector<layer> *l, const ImageData *p)
:ImageData(MainForm::makePixmap(c),t,sc,l,p) {}

ImageData::ImageData(const wrap_canvas<color_t> &wc, const ImageData *p)
:ImageData(wc.as_canvas(),true,nullptr,nullptr,p) {}

ImageData::ImageData(const symmetric_canvas<color_t> &sc, const std::vector<layer> *l)
:ImageData(sc.as_canvas(),true,&sc,l,nullptr) {}
