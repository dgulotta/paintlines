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
#include "../randomizewidget.h"

#ifdef LUARULES
#include "luafuncs.h"
#endif

using std::vector;

static const vector<ruletype> builtin_rules = {
	{"Arc",paintlines_layer_generator::generate_smootharc},
	{"Beads",paintlines_layer_generator::generate_smoothline2_beads},
	{"Cluster",paintlines_layer_generator::generate_cluster},
	{"Flower",paintlines_layer_generator::generate_flower},
	{"Fractal",paintlines_layer_generator::generate_cluster2},
	{"Granules",paintlines_layer_generator::generate_granules},
	{"Line",paintlines_layer_generator::generate_smoothline2},
	{"Loop",paintlines_layer_generator::generate_smoothline5},
	{"String",paintlines_layer_generator::generate_open_string},
	{"Swirl",paintlines_layer_generator::generate_swirl},
	{"Orbit",paintlines_layer_generator::generate_orbit},
	{"Tree",paintlines_layer_generator::generate_tree},
};

PaintRuleWidget::PaintRuleWidget(const vector<ruletype> &rt, int weight)
:rule_types(&rt)
{
	QFormLayout *layout = new QFormLayout();
	comboType = new QComboBox();
	updateRules();
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
	paintrule p = { (*rule_types)[comboType->currentIndex()].func, checkPastel->isChecked(), spinWeight->value() };
	return p;
}

void PaintRuleWidget::updateRules()
{
	for(int i=comboType->count();i<rule_types->size();i++)
		comboType->addItem((*rule_types)[i].name);
}

void LinesForm::init() {
	rule_types = builtin_rules;
	QFormLayout *layout = new QFormLayout;
	spinSize = newSizeSpin();
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = newSymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	spinColors = newColorSpin();
	layout->addRow(tr("Colors"),spinColors);
	for(int i=0;i<3;i++) {
		PaintRuleWidget *w = new PaintRuleWidget(rule_types,i==0?1:0);
		layout->addRow(w);
		rules.push_back(w);
		connect(this,SIGNAL(ruleTypesChanged()),w,SLOT(updateRules()));
	}
	colorWidget = new RandomColorWidget;
	layout->addRow(colorWidget);
	buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	randomizeWidget = new RandomizeWidget;
	layout->addRow(randomizeWidget);
	buttonRestore = new RestoreButton;
	layout->addRow(buttonRestore);
	checkTiled = newTileCheck();
	layout->addRow(checkTiled);
	lines = new paintlines;
	lines->set_color_generator(std::bind(&RandomColorWidget::generate,colorWidget));
#ifdef MULTIPAGE
	saver = new LayeredImageSaver(this);
#else
	saver = new ImageSaver(this);
#endif
	sideLayout = layout;
	connect(buttonRestore,SIGNAL(clicked()),this,SLOT(updateImage()));
	connect(this,SIGNAL(newImage(const ImageData &)),buttonRestore,SLOT(newImage(const ImageData &)));
	connect(this,SIGNAL(newImage(const ImageData &)),randomizeWidget,SLOT(imageUpdated(const ImageData &)));
	connect(randomizeWidget,SIGNAL(newImage(const ImageData &)),this,SIGNAL(newImage(const ImageData &)));
#ifdef LUARULES
	menuFile->addAction(tr("&Load rule"),this,SLOT(loadRule()));
#endif
}

bool LinesForm::checkLuaErrors()
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

void LinesForm::draw() {
	if(spinSize->value()%2!=0) {
		QMessageBox::information(this,"paintlines",tr("The size must be even."));
		return;
	}
	vector<paintrule> rule_list;
	for(int i=0;i<rules.size();i++)
		rule_list.push_back(rules[i]->rule());
	lines->set_rules(rule_list);
	lines->set_ncolors(spinColors->value());
	if(!colorWidget->load())
		QMessageBox::information(this,"paintlines",tr("Failed to load color palette image"));
	lines->paint(spinSize->value(),(symgroup)comboSymmetry->currentIndex());
	updateImage();
	checkLuaErrors();
}

void LinesForm::updateImage()
{
	ImageData data(lines->get_image(),lines->get_symmetric_image(),&(lines->get_layers()));
	emit newImage(data);
}

void LinesForm::loadRule()
{
#ifdef LUARULES
	QString fileName = QFileDialog::getOpenFileName(this,"paintlines",QString(),"Lua source code (*.lua);;All files (*)");
	if(fileName.isEmpty()) return;
	QByteArray fi = fileName.toAscii();
	auto funcs = get_lua_functions(fi.data());
	if(checkLuaErrors()) return;
	QStringList funclist;
	for(auto &s : funcs) funclist << s.c_str();
	QString funcName = QInputDialog::getItem(this,"paintlines","Function",funclist);
	if(funcName.isEmpty()) return;
	QByteArray fn = funcName.toAscii();
	std::function<void(symmetric_canvas<uint8_t> &)> f =
		[=] (symmetric_canvas<uint8_t> &c) { run_lua_rule(fi.data(),fn.data(),c); };
	rule_types.push_back({QString("(Lua)%1").arg(funcName),f});
	emit ruleTypesChanged();
#endif
}
