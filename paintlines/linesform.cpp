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

using std::vector;

PaintRuleWidget::PaintRuleWidget()
{
	QFormLayout *layout = new QFormLayout();
	comboType = new QComboBox();
	QStringList types;
	types << "Arc" << "Beads" << "Cluster" << "Flower" << "Fractal" << "Line" << "String" << "Swirl" << "Orbit" << "Tree";
	comboType->addItems(types);
	layout->addRow("Type",comboType);
	spinWeight = new QSpinBox();
	spinWeight->setValue(1);
	layout->addRow("Weight",spinWeight);
	checkPastel = new QCheckBox("Pastel");
	layout->addRow(checkPastel);
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
}

paintrule PaintRuleWidget::rule()
{
	paintrule p = { (ruletype)comboType->currentIndex(), checkPastel->isChecked(), spinWeight->value() };
	return p;
}

void LinesForm::init() {
	QFormLayout *layout = new QFormLayout;
	spinSize = newSizeSpin();
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = newSymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	spinColors = newColorSpin();
	layout->addRow(tr("Colors"),spinColors);
	for(int i=0;i<3;i++) {
		PaintRuleWidget *w = new PaintRuleWidget;
		layout->addRow(w);
		rules.push_back(w);
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
	connect(this,SIGNAL(newLayeredImage(const std::vector<layer> *)),saver,SLOT(newLayeredImage(const std::vector<layer> *)));
	connect(randomizeWidget,SIGNAL(newImage(QPixmap)),this,SLOT(clearLayers()));
#else
	saver = new ImageSaver(this);
#endif
	sideLayout = layout;
	connect(buttonRestore,SIGNAL(clicked()),this,SLOT(updateImage()));
	connect(this,SIGNAL(newImage(QPixmap,bool)),buttonRestore,SLOT(disable()));
	connect(this,SIGNAL(newCanvas(const symmetric_canvas<color_t> *)),randomizeWidget,SLOT(imageUpdated(const symmetric_canvas<color_t> *)));
	connect(randomizeWidget,SIGNAL(newImage(QPixmap)),buttonRestore,SLOT(enable()));
	connect(randomizeWidget,SIGNAL(newImage(QPixmap)),labelImage,SLOT(setPixmapTileable(const QPixmap &)));
	connect(randomizeWidget,SIGNAL(newImage(QPixmap)),saver,SLOT(newImage(const QPixmap &)));
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
}

void LinesForm::clearLayers()
{
	emit newLayeredImage(nullptr);
}

void LinesForm::updateImage()
{
	emit newImage(makePixmap(lines->get_image()),true);
	emit newCanvas(&(lines->get_symmetric_image()));
	emit newLayeredImage(&(lines->get_layers()));
}
