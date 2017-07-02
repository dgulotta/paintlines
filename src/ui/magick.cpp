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

#ifdef MULTIPAGE
#include "lib/layer.hpp"
#include "magick.hpp"

#include <Magick++.h>

using namespace Magick;

bool save_multilayer(int width, int height, const std::vector<layer> &layers, const std::string &name) {
	try {
		// GraphicsMagick's alpha channel is the inverse of the usual
		// alpha channel (0 = opaque, 255 = transparent)
		// Also, it seems that if you create an image with an opaque
		// default color, Graphicsmagick assumes that the image
		// doesn't have an alpha channel
		std::vector<Image> images(layers.size()+1,Image(Geometry(width,height),Color(0,0,0,255)));
		int i, x, y;
		for(y=0;y<height;y++)
			for(x=0;x<width;x++)
				images[0].pixelColor(x,y,Color(0,0,0,0));
		for(i=0;i<layers.size();i++) {
			color_t col=layers[i].color;
			const canvas<uint8_t> &img = *layers[i].pixels;
			for(y=0;y<height;y++)
				for(x=0;x<width;x++)
					images[i+1].pixelColor(x,y,Color(col.red,col.green,col.blue,255-img(x,y)));
		}
		writeImages(images.begin(),images.end(),name);
	} catch(Exception &) {
		return false;
	}
	return true;
}
#endif
