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

#include <QtGui>

#include "linesform.h"
#include "paintlineswidget.h"

PaintRuleWidget::PaintRuleWidget()
{
	QFormLayout *layout = new QFormLayout();
	comboType = new QComboBox();
	QStringList types;
	types << "Arc" << "Beads" << "Cluster" << "Flower" << "Fractal" << "Line" << "String";
	comboType->addItems(types);
	layout->addRow("Type",comboType);
	spinWeight = new QSpinBox();
	spinWeight->setValue(1);
	layout->addRow("Weight",spinWeight);
	checkPastel = new QCheckBox("Pastel");
	layout->addRow(checkPastel);
	setLayout(layout);
}

paintrule PaintRuleWidget::rule()
{
	paintrule p = { (ruletype)comboType->currentIndex(), checkPastel->isChecked(), spinWeight->value() };
	return p;
}

void LinesForm::addWidgets(QBoxLayout *sideLayout) {
	sideLayout->addWidget(new QLabel("Colors"));
	spinColors = new QSpinBox;
	spinColors->setValue(25);
	sideLayout->addWidget(spinColors);
	for(int i=0;i<3;i++) {
		PaintRuleWidget *w = new PaintRuleWidget;
		sideLayout->addWidget(w);
		rules.push_back(w);
	}
}

painterwidget * LinesForm::createPainterWidget()
{
	lines = new paintlineswidget;
	return lines;
}

void LinesForm::draw(int sz, int sym_index) {
	vector<paintrule> rule_list;
	for(int i=0;i<rules.size();i++)
		rule_list.push_back(rules[i]->rule());
	lines->set_rules(rule_list);
	lines->draw(sz,spinColors->value(),(symgroup)sym_index);
}

bool LinesForm::saveAs()
{
	QString s=QFileDialog::getSaveFileName();
	if(!s.isEmpty()) {
		if(s.toUpper().endsWith(".TIFF")||s.toUpper().endsWith(".TIF")) {
			return save_multilayer(lines->get_size(),lines->get_size(),lines->get_layers(),s.toStdString());
		}
		else {
			painter->save(s,"PNG");
			return true;
		}
	}
	else {
		return false;
	}
}
