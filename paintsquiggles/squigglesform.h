/***************************************************************************
 *   Copyright (C) 2008, 2013 by Daniel Gulotta                            *
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

#ifndef _SQUIGGLESFORM_H
#define _SQUIGGLESFORM_H

#include "../basicform.h"
#include "../symmetric_canvas.h"

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class RandomColorWidget;
class RandomizeWidget;
class paintsquiggles;

class SquigglesForm : public BasicForm
{
	Q_OBJECT
signals:
	void newLayeredImage(const std::vector<layer> *l);
	void newCanvas(const symmetric_canvas<color_t> *c);
protected slots:
	virtual void draw();
	virtual void init();
	void clearLayers();
	void updateImage();
protected:
	QSpinBox *spinSize;
	QSpinBox *spinColors;
	QComboBox *comboSymmetry;
	QDoubleSpinBox *spinAlpha;
	QDoubleSpinBox *spinExponent;
	QDoubleSpinBox *spinThickness;
	QDoubleSpinBox *spinSharpness;
	RandomColorWidget *colorWidget;
	RandomizeWidget *randomizeWidget;
	RestoreButton *buttonRestore;
	paintsquiggles *squiggles;
};

#endif
