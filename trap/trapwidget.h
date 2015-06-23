
#ifndef _TRAPWIDGET_H
#define _TRAPWIDGET_H

#include "../color.h"
#include "../imagegeneratorwidget.h"
/***************************************************************************
 *   Copyright (C) 2014-2015 by Daniel Gulotta                             *
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

#include "../symmetric_canvas.h"

class TrapWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	TrapWidget();
private slots:
	void draw();
private:
	QSpinBox *spinSize;
	SymmetryCombo *comboSymmetry;
	symmetric_canvas<color_t> img;
};

#endif