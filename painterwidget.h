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

#ifndef _PAINTERWIDGET_H
#define _PAINTERWIDGET_H

#include "painter.h"
#include <qwidget.h>
#include <qpixmap.h>

class painterwidget : public QWidget, virtual public painter
{
    Q_OBJECT
public:
    painterwidget(QWidget *parent=0,const char *name=0);
    void paint(int sz, symgroup sg);
    bool save(const QString &filename, const char *format);
    void randomize(int xtiles, int ytiles);
    void restore();
protected:
    void paintEvent(QPaintEvent *);
private:
    QPixmap mypixmap;
};

#endif
