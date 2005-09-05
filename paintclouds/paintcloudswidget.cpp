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

#include "paintcloudswidget.h"

paintcloudswidget::paintcloudswidget(QWidget *parent, const char *name)
  : painterwidget(parent,name)
{
}

void paintcloudswidget::draw
(int sz, symgroup sg, unsigned char red1,unsigned char green1,
 unsigned char blue1, unsigned char red2, unsigned char green2,
 unsigned char blue2, unsigned char red3, unsigned char green3,
 unsigned char blue3)
{
  paintclouds::set_colors(red1,green1,blue1,red2,green2,blue2,red3,green3,
			  blue3);
  paintclouds::paint(sz,sg);
  painterwidget::paint(sz,sg);
}
