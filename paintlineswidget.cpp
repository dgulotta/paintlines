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
    paint(sz,n,sg);
    QImage myimage(sz,sz,32);
    int i, sz2=sz*sz;
    const vector<unsigned char> red=get_red();
    const vector<unsigned char> green=get_green();
    const vector<unsigned char> blue=get_blue();
    for(i=0;i<sz2;i++)
	myimage.setPixel(i/sz,i%sz,qRgb(red[i],green[i],blue[i]));
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
  symgroup sg=get_symgroup();
  const vector<unsigned char> red=get_red();
  const vector<unsigned char> green=get_green();
  const vector<unsigned char> blue=get_blue();
  int size=get_size();
  QImage myimage(xtiles*size,ytiles*size,32);
  int halfsize=size/2, size1=size-1, halfsize1=halfsize-1, i,j,k,l;
  int qsize=halfsize/2, sizeq1=size1+halfsize-qsize, sizesq=size*size;
  switch(sg) {
  case SYM_CMM:
  case SYM_P2:
    for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++)
	  for(j=0;j<halfsize;j++) {
	    int index=((z&1)?i:(size1-i))+size*j;
	    myimage.setPixel(i+k*size,j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=((z&2)?i:(size1-i))+size*j;
	    myimage.setPixel(size1-i+k*size,j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=((z&4)?i:(size1-i))+size*j;
	    myimage.setPixel(i+k*size,size1-j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=((z&8)?i:(size1-i))+size*j;
	    myimage.setPixel(size1-i+k*size,size1-j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	  }
      }
    break;
  case SYM_PMG:
    for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++)
	  for(j=0;j<halfsize;j++) {
	    int index=mod(((z&1)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    myimage.setPixel(i+k*size,j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=mod(((z&1)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    myimage.setPixel(size1-i+k*size,j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=mod(((z&1)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    myimage.setPixel(i+k*size,size1-j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=mod(((z&1)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    myimage.setPixel(size1-i+k*size,size1-j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	  }
      }
    break;
  case SYM_PGG:
     for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++)
	  for(j=0;j<halfsize;j++) {
	    int index=mod(((z&1)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    myimage.setPixel(i+k*size,j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=mod(((z&2)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    myimage.setPixel(size1-i+k*size,j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=mod(((z&4)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    myimage.setPixel(i+k*size,size1-j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=mod(((z&8)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    myimage.setPixel(size1-i+k*size,size1-j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	  }
      }
    break;
  case SYM_P4:
  case SYM_P4G:
    for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++)
	  for(j=0;j<=i;j++) {
	    int index=(z&0x1)?(i+size*j):(j+size*i);
	    myimage.setPixel(i+k*size,j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=(z&0x2)?(i+size*j):(j+size*i);
	    myimage.setPixel(j+k*size,i+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=(z&0x4)?(i+size*j):(j+size*i);
	    myimage.setPixel(size1-i+k*size,j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=(z&0x8)?(i+size*j):(j+size*i);
	    myimage.setPixel(j+k*size,size1-i+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=(z&0x10)?(i+size*j):(j+size*i);
	    myimage.setPixel(size1-i+k*size,size1-j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=(z&0x20)?(i+size*j):(j+size*i);
	    myimage.setPixel(size1-j+k*size,size1-i+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=(z&0x40)?(i+size*j):(j+size*i);
	    myimage.setPixel(i+k*size,size1-j+l*size,
			     qRgb(red[index],green[index],blue[index]));
	    index=(z&0x80)?(i+size*j):(j+size*i);
	    myimage.setPixel(size1-j+k*size,i+l*size,
			     qRgb(red[index],green[index],blue[index]));
	  }
      }
  }
  mypixmap.convertFromImage(myimage);
  resize(xtiles*size,ytiles*size);
  paintEvent(NULL);
}

void paintlineswidget::restore()
{
    int size=get_size();
    QImage myimage(size,size,32);
    int i, size2=size*size;
    const vector<unsigned char> red=get_red();
    const vector<unsigned char> green=get_green();
    const vector<unsigned char> blue=get_blue();
    for(i=0;i<size2;i++)
	myimage.setPixel(i/size,i%size,qRgb(red[i],green[i],blue[i]));
    mypixmap.convertFromImage(myimage);
    resize(size,size);
    paintEvent(NULL);
}

void paintlineswidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,mypixmap);
}
