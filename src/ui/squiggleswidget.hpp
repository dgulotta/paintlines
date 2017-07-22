/***************************************************************************
 *   Copyright (C) 2008, 2013-2014, 2016 by Daniel Gulotta                 *
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

#ifndef _SQUIGGLESWIDGET_H
#define _SQUIGGLESWIDGET_H
#ifdef FFTWPROGS

#include <vector>
#include "imagegeneratorwidget.hpp"
#include "inputwidgets.hpp"
#include "lib/layer.hpp"
#include "lib/symmetric_canvas.hpp"

class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class RandomColorWidget;

class SquigglesWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	SquigglesWidget();
protected slots:
	void draw();
	void updateImage();
	void resetColors();
protected:
	QSpinBox *spinSize;
	QSpinBox *spinColors;
	SymmetryCombo *comboSymmetry;
	QDoubleSpinBox *spinAlpha;
	QDoubleSpinBox *spinExponent;
	QDoubleSpinBox *spinThickness;
	QDoubleSpinBox *spinSharpness;
	RandomColorWidget *colorWidget;
	QPushButton *newColorButton;
	std::shared_ptr<std::vector<layer>> layers;
};

#endif
#endif
