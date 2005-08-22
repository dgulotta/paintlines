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

#include "paintlineswidget.h"
#include <qpainter.h>
#include <qcolor.h>
#include <qimage.h>

paintlineswidget::paintlineswidget(QWidget *parent,const char *name)
    :QWidget(parent,name)	
{
}

void paintlineswidget::draw(int sz, int n, symgroup sg)
{
  set_ncolors(n);
  paint(sz,sg);
  QImage myimage(sz,sz,32);
  int i, sz2=sz*sz;
  for(i=0;i<sz2;i++)
    myimage.setPixel(i/sz,i%sz,qRgb(painter::red[i],painter::green[i],
				    painter::blue[i]));
  mypixmap.convertFromImage(myimage);
  resize(sz,sz);
  paintEvent(NULL);
}

bool paintlineswidget::save(const QString &filename,const char *format)
{
    return mypixmap.save(filename,format);
}

void paintlineswidget::randomize(int xtiles, int ytiles)
{
  vector<unsigned char> r,g,b;
  int width=xtiles*painter::size, height=ytiles*painter::size, i, j;
  painter::randomize(xtiles,ytiles,r,g,b);
  QImage myimage(width,height,32);
  for(i=0;i<width;i++)
    for(j=0;j<height;j++)
      myimage.setPixel(i,j,qRgb(r[i+width*j],g[i+width*j],b[i+width*j]));
  mypixmap.convertFromImage(myimage);
  resize(width,height);
  paintEvent(NULL);
}

void paintlineswidget::restore()
{
    QImage myimage(painter::size,painter::size,32);
    int i, size2=painter::size*painter::size;
    for(i=0;i<size2;i++)
	myimage.setPixel(i/painter::size,i%painter::size,
			 qRgb(painter::red[i],painter::green[i],
			      painter::blue[i]));
    mypixmap.convertFromImage(myimage);
    resize(painter::size,painter::size);
    paintEvent(NULL);
}

void paintlineswidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,mypixmap);
}
