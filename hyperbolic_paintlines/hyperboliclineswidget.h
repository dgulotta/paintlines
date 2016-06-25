/***************************************************************************
 *   Copyright (C) 2008, 2013-2014 by Daniel Gulotta                       *
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

#ifndef _HYPERBOLICLINESWIDGET_H
#define _HYPERBOLICLINESWIDGET_H

#include "../imagegeneratorwidget.h"
#include "../canvas.h"
#include "../layer.h"

#include <QSpinBox>

class QMenuBar;
class QMenu;
class QAction;
class QBoxLayout;
class QGroupBox;
class QComboBox;
class QLabel;
class QPushButton;
class QDoubleSpinBox;
class hyperbolic_paintlines;

struct SymmetryGroupToken
{
	SymmetryGroupToken(char t) : type(t), value(0), min_value(0) {}
	SymmetryGroupToken(int v, int m) : type('#'), value(v), min_value(m) {}
	char type;
	int value;
	int min_value;
};

class SymmetrySpin : public QSpinBox
{
	Q_OBJECT
public:
	SymmetrySpin(SymmetryGroupToken &t) : token(&t) {
		setValue(token->value);
		setMinimum(token->min_value);
		connect(this,(void (QSpinBox::*)(int))&QSpinBox::valueChanged,this,&SymmetrySpin::update);
	}
	virtual QSize minimumSizeHint() const { return sizeHint(); }
public slots:
	void update(int n) { token->value=n; }
private:
	SymmetryGroupToken *token;
};

class HyperbolicLinesWidget : public ImageGeneratorWidget
{
	Q_OBJECT;
public:
	HyperbolicLinesWidget();
protected slots:
	void draw();
	void symmetryChanged(int n);
protected:
	QComboBox *comboModel;
	QComboBox *comboSymmetry;
	QComboBox *comboSubset;
	QSpinBox *spinSize;
	QSpinBox *spinColors;
	QDoubleSpinBox *spinThickness;
	QDoubleSpinBox *spinSharpness;
	QBoxLayout *parameterLayout;
	std::vector<std::vector<SymmetryGroupToken>> tokens;
	std::vector<canvas<uint8_t>> grids;
	std::vector<layer> layers;
};

#endif
