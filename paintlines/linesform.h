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
#include "paintlines.h"

class QCheckBox;
class QComboBox;
class QSpinBox;
class paintlineswidget;

class PaintRuleWidget : public QWidget
{
public:
	PaintRuleWidget();
	paintrule rule();
private:
	QComboBox *comboType;
	QSpinBox *spinWeight;
	QCheckBox *checkPastel;
};

class LinesForm : public BasicForm
{
	Q_OBJECT
public:
	virtual bool saveAs();
protected:
	virtual void addWidgets(QBoxLayout *sideLayout);
	virtual painterwidget * createPainterWidget();
	virtual void draw(int sz, int sym_index);
	QSpinBox *spinColors;
	vector<PaintRuleWidget *> rules;
	paintlineswidget *lines;
};
