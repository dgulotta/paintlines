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

#include "hyperbolic_paintlineswidget.h"
#include <qpainter.h>
#include <qcolor.h>
#include <qimage.h>

hyperbolic_paintlineswidget::hyperbolic_paintlineswidget
(QWidget *parent,const char *name)
    :QWidget(parent,name)	
{
}

void hyperbolic_paintlineswidget::draw(int sz, int n,
				       hyperbolic_symmetry_group sym)
{
  set_ncolors(n);
  paint(sz,sym);
  QImage myimage(sz,sz,32);
  int i, sz2=sz*sz;
  for(i=0;i<sz2;i++)
    myimage.setPixel(i/sz,i%sz,qRgb(hyperbolic_painter::red[i],
				    hyperbolic_painter::green[i],
				    hyperbolic_painter::blue[i]));
  mypixmap.convertFromImage(myimage);
  resize(sz,sz);
  paintEvent(NULL);
}

bool hyperbolic_paintlineswidget::save(const QString &filename,
				       const char *format)
{
    return mypixmap.save(filename,format);
}

void hyperbolic_paintlineswidget::restore()
{
    QImage myimage(hyperbolic_painter::size,hyperbolic_painter::size,32);
    int i, size2=hyperbolic_painter::size*hyperbolic_painter::size;
    for(i=0;i<size2;i++)
	myimage.setPixel(i/hyperbolic_painter::size,i%hyperbolic_painter::size,
			 qRgb(hyperbolic_painter::red[i],
			      hyperbolic_painter::green[i],
			      hyperbolic_painter::blue[i]));
    mypixmap.convertFromImage(myimage);
    resize(hyperbolic_painter::size,hyperbolic_painter::size);
    paintEvent(NULL);
}

void hyperbolic_paintlineswidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,mypixmap);
}
