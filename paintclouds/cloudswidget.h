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

#ifndef _CLOUDSWIDGET_H
#define _CLOUDSWIDGET_H

#include "../color.h"
#include "../imagegeneratorwidget.h"
#include "../symmetric_canvas.h"

class QMouseEvent;
class RandomizeWidget;

class ColorButton : public QWidget
{
Q_OBJECT
public:
	ColorButton(QWidget *parent=0) : QWidget(parent) {
		setAutoFillBackground(true);
	}
	ColorButton(QColor col, QWidget *parent=0) : ColorButton(parent) {
		setPalette(QPalette(col));
	}
	color_t color() {
		QColor c = palette().color(QPalette::Window);
		return color_t(c.red(),c.green(),c.blue());
	}
protected:
	virtual void mousePressEvent(QMouseEvent *);
};

class CloudsWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	CloudsWidget();
	static QComboBox * newComboRandom();
protected slots:
	void draw();
protected:
	QSpinBox *spinSize;
	SymmetryCombo *comboSymmetry;
	ColorButton *color1;
	ColorButton *color2;
	ColorButton *color3;
	QComboBox *comboRandom;
	symmetric_canvas<color_t> canvas;
};

#endif
