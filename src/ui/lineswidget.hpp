/***************************************************************************
 *   Copyright (C) 2013-2014 by Daniel Gulotta                             *
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

#ifndef _LINESWIDGET_H
#define _LINESWIDGET_H

#include <functional>
#include <QMetaType>
#include "imagegeneratorwidget.hpp"
#include "inputwidgets.hpp"
#include "lib/layer.hpp"
#include "lib/symmetric_canvas.hpp"

using paintfunc = std::function<void(symmetric_canvas<uint8_t> &)>;

Q_DECLARE_METATYPE(paintfunc);

class QCheckBox;
class QComboBox;
class QSpinBox;
class RandomColorWidget;

struct paintrule
{
	paintfunc func;
	bool pastel;
	int weight;
};

class PaintRuleWidget : public QWidget
{
	Q_OBJECT
public:
	PaintRuleWidget(int weight=0);
	paintrule rule();
public slots:
	void addRule(const QString &s, const paintfunc &f);
private:
	QComboBox *comboType;
	QSpinBox *spinWeight;
	QCheckBox *checkPastel;
};

class LinesWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	LinesWidget();
signals:
	void addRule(const QString &s, const paintfunc &f);
protected slots:
	void draw();
	void loadRule();
protected:
	bool checkLuaErrors();
	QSpinBox *spinSize;
	QSpinBox *spinColors;
	SymmetryCombo *comboSymmetry;
	std::vector<PaintRuleWidget *> rules;
	RandomColorWidget *colorWidget;
};

#endif
