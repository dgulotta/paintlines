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

#include <Magick++.h>

#include "magick.h"

using namespace Magick;

bool save_multilayer(int width, int height, const vector<layer> &layers, const std::string &name) {
	try {
		// Graphicsmagick's alpha channel is the inverse of the usual
		// alpha channel (0 = opaque, 255 = transparent)
		// Also, it seems that if you create an image with an opaque
		// default color, Graphicsmagick assumes that the image
		// doesn't have an alpha channel
		vector<Image> images(layers.size()+1,Image(Geometry(width,height),Color(0,0,0,255)));
		int i, x, y, r, g, b;
		for(y=0;y<height;y++)
			for(x=0;x<width;x++)
				images[0].pixelColor(x,y,Color(0,0,0,0));
		for(i=0;i<layers.size();i++) {
			tie(r,g,b)=layers[i].color;
			for(y=0;y<height;y++)
				for(x=0;x<width;x++)
					images[i+1].pixelColor(x,y,Color(r,g,b,255-layers[i].pixels[x+y*width]));
		}
		writeImages(images.begin(),images.end(),name);
	} catch(...) {
		return false;
	}
	return true;
}
