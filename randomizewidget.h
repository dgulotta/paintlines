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

#ifndef _RANDOMIZEWIDGET_H
#define _RANDOMIZEWIDGET_H

#include <QWidget>

#include "color.h"
#include "symmetric_canvas.h"

class QPushButton;
class QSpinBox;

class RandomizeWidget : public QWidget
{
	Q_OBJECT
public:
	RandomizeWidget(QWidget *parent=nullptr);
signals:
	void newImage(QPixmap p);
public slots:
	void imageUpdated(const symmetric_canvas<color_t> *c);
	void randomize();
private:
	const symmetric_canvas<color_t> *src;
	QPushButton *buttonRandomize;
	QSpinBox *spinXTiles;
	QSpinBox *spinYTiles;
};

#endif
