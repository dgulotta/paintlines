/***************************************************************************
 *   Copyright (C) 2016 by Daniel Gulotta                                  *
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

#ifndef _HYPERBOLICCLOUDSWIDGET_H
#define _HYPERBOLICCLOUDSWIDGET_H

#include "../imagegeneratorwidget.h"

class ColorButton;
class HyperbolicSymmetryChooser;
class QComboBox;
class QSpinBox;

class HyperbolicCloudsWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	HyperbolicCloudsWidget();
protected slots:
	void draw();
protected:
	QComboBox *comboModel;
	HyperbolicSymmetryChooser *chooser;
	QSpinBox *spinSize;
	ColorButton *color1;
	ColorButton *color2;
	ColorButton *color3;
	QComboBox *comboRandom;
};

#endif