/***************************************************************************
 *   Copyright (C) 2013-2014, 2016 by Daniel Gulotta                       *
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

#include <algorithm>
#include <QtWidgets>
#include "imagedata.hpp"
#include "randomcolorwidget.hpp"
#include "lineswidget.hpp"
#include "lib/paintlines.hpp"
#include "lib/randgen.hpp"
#include "lib/luafuncs.hpp"

using std::vector;
using std::shared_ptr;

PaintRuleWidget::PaintRuleWidget(int weight)
{
	QFormLayout *layout = new QFormLayout();
	comboType = new QComboBox();
	layout->addRow("Type",comboType);
	spinWeight = new QSpinBox();
	spinWeight->setValue(weight);
	layout->addRow("Weight",spinWeight);
	checkPastel = new QCheckBox("Pastel");
	layout->addRow(checkPastel);
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
}

paintrule PaintRuleWidget::rule()
{
	paintrule p = { comboType->itemData(comboType->currentIndex()).value<paintfunc>(), checkPastel->isChecked(), spinWeight->value() };
	return p;
}

void PaintRuleWidget::addRule(const QString &s, const paintfunc &f)
{
	comboType->addItem(s,QVariant::fromValue(f));
}

LinesWidget::LinesWidget()
{
	QFormLayout *layout = this->layout();
	spinSize = new SizeSpin(2);
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = new SymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	spinColors = new QSpinBox;
	spinColors->setValue(DEFAULT_NCOLORS);
	layout->addRow(tr("Colors"),spinColors);
#ifdef LUARULES
	QPushButton *buttonAddRule = new QPushButton(tr("Load rule"));
	layout->addRow(buttonAddRule);
	connect(buttonAddRule,&QPushButton::clicked,this,&LinesWidget::loadRule);
#endif
	for(int i=0;i<3;i++) {
		PaintRuleWidget *w = new PaintRuleWidget(i==0?1:0);
		layout->addRow(w);
		rules.push_back(w);
		connect(this,&LinesWidget::addRule,w,&PaintRuleWidget::addRule);
	}
	emit addRule("Arc",generate_smootharc);
	emit addRule("Beads",generate_smoothline2_beads);
	emit addRule("Cluster",generate_cluster);
	emit addRule("Curl",generate_curl);
	emit addRule("Flower",generate_flower);
	emit addRule("Fractal",generate_cluster2);
	emit addRule("Granules",generate_granules);
	emit addRule("Line",generate_smoothline2);
	emit addRule("Loop",generate_smoothline5);
	emit addRule("Orbit",generate_orbit);
	emit addRule("Star",generate_star);
	emit addRule("String",generate_open_string);
	emit addRule("Swirl",generate_swirl);
	emit addRule("Tree",generate_tree);
	colorWidget = new RandomColorWidget;
	layout->addRow(colorWidget);
	QPushButton *buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	connect(buttonDraw,&QPushButton::clicked,this,&LinesWidget::draw);
}

bool LinesWidget::checkLuaErrors()
{
#ifdef LUARULES
	if(!get_lua_errors().empty()) {
		QString msg;
		QTextStream s(&msg);
		s << "The following errors occurred:\n";
		for(auto e : get_lua_errors()) { s << e.c_str(); }
		QMessageBox::information(this,"paintlines",msg);
		clear_lua_errors();
		return true;
	}
#endif
	return false;
}

void LinesWidget::draw() {
	if(!colorWidget->load())
		QMessageBox::information(this,"paintlines",tr("Failed to load color palette image"));
	std::vector<int> weights(rules.size());
	transform(rules.begin(),rules.end(),weights.begin(),[] (auto &r) { return r->rule().weight; });
	auto layers = std::make_shared<vector<layer>>(spinColors->value());
	symgroup sg = comboSymmetry->group();
	for(size_t i=0;i<layers->size();i++) {
		auto rule=rules[random_weighted(weights)]->rule();
		auto grid_ptr = std::make_shared<symmetric_canvas<uint8_t>>(spinSize->value(),sg);
		rule.func(*grid_ptr);
		(*layers)[i].pixels=shared_ptr<const canvas<uint8_t>>(grid_ptr,&(grid_ptr->as_canvas()));
		(*layers)[i].color=colorWidget->generate();
		(*layers)[i].pastel=rule.pastel;
	}
	auto img = std::make_shared<symmetric_canvas<color_t>>(spinSize->value(),sg);
	merge(img->unsafe_get_canvas(),*layers);
	emit newImage(ImageData(std::move(img),std::move(layers)));
	checkLuaErrors();
}

void LinesWidget::loadRule()
{
#ifdef LUARULES
	QString fileName = QFileDialog::getOpenFileName(this,"paintlines",QString(),"Lua source code (*.lua);;All files (*)");
	if(fileName.isEmpty()) return;
	QByteArray fi = fileName.toLocal8Bit();
	auto funcs = get_lua_functions(fi.data());
	if(checkLuaErrors()) return;
	QStringList funclist;
	for(auto &s : funcs) funclist << s.c_str();
	QString funcName = QInputDialog::getItem(this,"paintlines","Function",funclist);
	if(funcName.isEmpty()) return;
	QByteArray fn = funcName.toLocal8Bit();
	paintfunc f = [=] (symmetric_canvas<uint8_t> &c) { run_lua_rule(fi.data(),fn.data(),c); };
	emit addRule(QString("(Lua)%1").arg(funcName),f);
#endif
}
