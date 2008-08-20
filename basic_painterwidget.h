/***************************************************************************
 *   Copyright (C) 2008 by Daniel Gulotta                                  *
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

#ifndef _BASIC_PAINTERWIDGET_H
#define _BASIC_PAINTERWIDGET_H

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QWidget>

#include "basic_painter.h"

class basic_painterwidget : public QWidget, virtual public basic_painter
{
    Q_OBJECT
public:
    basic_painterwidget(QWidget *parent=0,const char *name=0)
      : QWidget(parent,name) {}
    void paint() {
      QImage myimage(basic_painter::size,basic_painter::size,32);
      int i, sz2=basic_painter::size*basic_painter::size;
      for(i=0;i<sz2;i++)
	myimage.setPixel(i/basic_painter::size,i%basic_painter::size,
			 qRgb(basic_painter::red[i],
			      basic_painter::green[i],
			      basic_painter::blue[i]));
      mypixmap.convertFromImage(myimage);
      resize(basic_painter::size,basic_painter::size);
      update();
    }
    bool save(const QString &filename, const char *format) {
      return mypixmap.save(filename,format);
    }
protected:
    void paintEvent(QPaintEvent *) {
      QPainter p(this);
      p.drawPixmap(0,0,mypixmap);
    }
    QPixmap mypixmap;
};

#endif
