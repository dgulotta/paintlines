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

#include "painterwidget.h"
#include <QPainter>
#include <QColor>
#include <QImage>
#include <QPaintEvent>

painterwidget::painterwidget(QWidget *parent)
    : basic_painterwidget(parent)	
{
}

void painterwidget::randomize(int xtiles, int ytiles)
{
  vector<unsigned char> r,g,b;
  int width=xtiles*painter::size, height=ytiles*painter::size, i, j;
  painter::randomize(xtiles,ytiles,r,g,b);
  QImage myimage(width,height,QImage::Format_RGB32);
  for(i=0;i<width;i++)
    for(j=0;j<height;j++)
      myimage.setPixel(i,j,qRgb(r[i+width*j],g[i+width*j],b[i+width*j]));
  mypixmap=QPixmap::fromImage(myimage);
  resize(width,height);
  update();
}
