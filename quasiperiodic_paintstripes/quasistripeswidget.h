/***************************************************************************
 *   Copyright (C) 2008, 2014 by Daniel Gulotta                            *
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

#ifndef _QUASISTRIPESWIDGET_H
#define _QUASISTRIPESWIDGET_H

#include "../imagegeneratorwidget.h"

class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class quasiperiodic_paintstripes;

class QuasiStripesWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	QuasiStripesWidget();
protected slots:
	void draw();
protected:
	QSpinBox *spinSize;
	QSpinBox *spinQuasiSize;
	QDoubleSpinBox *spinAlpha;
	quasiperiodic_paintstripes *stripes;
};

#endif