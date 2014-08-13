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

#include "../basicform.h"
#include "../randomcolorwidget.h"
#include "paintlines.h"

class QCheckBox;
class QComboBox;
class QSpinBox;
class RandomizeWidget;

struct ruletype {
	QString name;
	std::function<void(symmetric_canvas<uint8_t> &)> func;
};

class PaintRuleWidget : public QWidget
{
	Q_OBJECT
public:
	PaintRuleWidget(const std::vector<ruletype> &rt, int weight=0);
	paintrule rule();
public slots:
	void updateRules();
private:
	const std::vector<ruletype> *rule_types;
	QComboBox *comboType;
	QSpinBox *spinWeight;
	QCheckBox *checkPastel;
};

class LinesForm : public BasicForm
{
	Q_OBJECT
signals:
	void ruleTypesChanged();
protected slots:
	virtual void draw();
	virtual void init();
	void updateImage();
	void loadRule();
protected:
	bool checkLuaErrors();
	QSpinBox *spinSize;
	QSpinBox *spinColors;
	QComboBox *comboSymmetry;
	QCheckBox *checkTiled;
	std::vector<PaintRuleWidget *> rules;
	RandomColorWidget *colorWidget;
	RandomizeWidget *randomizeWidget;
	RestoreButton *buttonRestore;
	paintlines *lines;
	std::vector<ruletype> rule_types;
};
