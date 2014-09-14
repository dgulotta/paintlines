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

#include <QtWidgets>
#include <sstream>

#include "../imagedata.h"
#include "hyperboliclineswidget.h"
#include "hyperbolic_paintlines.h"

HyperbolicLinesWidget::HyperbolicLinesWidget()
{
	tokens = {
		{ '*',{2,2},'x' },
		{ '2','*',{3,2},{2,2} },
		{ {3,3},'2','2','2' },
		{ {5,3}, {4,3}, '2' },
		{ {5,3}, '*', {2,2} },
		{ '*', {5,2}, {4,2}, {2,2} },
		{ {3,3}, '2', 'x' },
		{ '2', '2', '*', {2,2} },
		{ {3,2}, '*', {2,2}, {2,2} },
		{ '*', {3,2}, {2,2}, {2,2}, {2,2} },
		{ {3,2}, '2', '*', {1,1} },
		{ '*', {2,1}, '*', {1,1} },
		{ {2,2}, 'o' },
		{ {5,2}, {4,2}, {2,2} },
		{ {2,2}, 'x', 'x' },
	};
	QFormLayout *layout = new QFormLayout;
	comboModel = new QComboBox;
	comboModel->addItem(tr("Poincare"));
	comboModel->addItem(tr("Klein"));
	layout->addRow(tr("Model"),comboModel);
	comboSymmetry = new QComboBox;
	for(auto &v : tokens) {
		std::ostringstream s;
		char next = 'a';
		for(auto &t : v) {
			if(t.type=='#')
				s << (next++);
			else
				s << t.type;
		}
		comboSymmetry->addItem(s.str().c_str());
	}
	layout->addRow(tr("Symmetry"),comboSymmetry);
	parameterLayout = new QHBoxLayout;
	layout->addRow(parameterLayout);
	symmetryChanged(0);
	comboSubset=new QComboBox;
	comboSubset->addItem(tr("All"));
	comboSubset->addItem(tr("Det > 0"));
	comboSubset->addItem(tr("Random"));
	layout->addRow(tr("Subset"),comboSubset);
	spinSize=new QSpinBox;
	spinSize->setMinimum(1);
	spinSize->setMaximum(65536);
	spinSize->setValue(256);
	layout->addRow(tr("Size"),spinSize);
	spinColors=newColorSpin();
	layout->addRow(tr("Colors"),spinColors);
	spinThickness = new QDoubleSpinBox;
	spinThickness->setValue(1);
	layout->addRow(tr("Thickness"),spinThickness);
	spinSharpness = new QDoubleSpinBox;
	spinSharpness->setValue(1.5);
	layout->addRow(tr("Sharpness"),spinSharpness);
	QPushButton *buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	setLayout(layout);
	lines = new hyperbolic_paintlines;
	connect(comboSymmetry,(void (QComboBox::*)(int))&QComboBox::currentIndexChanged,this,&HyperbolicLinesWidget::symmetryChanged);
	connect(buttonDraw,&QPushButton::clicked,this,&HyperbolicLinesWidget::draw);
}

void HyperbolicLinesWidget::draw()
{
  hyperbolic_symmetry_group *sg;
  flip_type ft = (flip_type)comboSubset->currentIndex();
  const auto &t = tokens[comboSymmetry->currentIndex()];
  switch(comboSymmetry->currentIndex()) {
  case 0:
    sg=hyperbolic_symmetry_group::group_sax(t[1].value,ft);
    break;
  case 1:
    sg=hyperbolic_symmetry_group::group_2sab(t[2].value,t[3].value,ft);
    break;
  case 2:
    sg=hyperbolic_symmetry_group::group_a222(t[0].value,ft);
    break;
  case 3:
    sg=hyperbolic_symmetry_group::group_ab2(t[0].value,t[1].value,ft);
    break;
  case 4:
    sg=hyperbolic_symmetry_group::group_asb(t[0].value,t[2].value,ft);
    break;
  case 5:
    sg=hyperbolic_symmetry_group::group_sabc(t[1].value,t[2].value,t[3].value,ft);
    break;
  case 6:
    sg=hyperbolic_symmetry_group::group_a2x(t[0].value,ft);
    break;
  case 7:
    sg=hyperbolic_symmetry_group::group_22sa(t[3].value,ft);
    break;
  case 8:
	sg=hyperbolic_symmetry_group::group_asbc(t[0].value,t[2].value,t[3].value,ft);
	break;
  case 9:
	sg=hyperbolic_symmetry_group::group_sabcd(t[1].value,t[2].value,t[3].value,t[4].value,ft);
	break;
  case 10:
	sg=hyperbolic_symmetry_group::group_a2sb(t[0].value,t[3].value,ft);
	break;
  case 11:
  	sg=hyperbolic_symmetry_group::group_sasb(t[1].value,t[3].value,ft);
	break;
  case 12:
  	sg=hyperbolic_symmetry_group::group_ao(t[0].value,ft);
	break;
  case 13:
    sg=hyperbolic_symmetry_group::group_abc(t[0].value,t[1].value,t[2].value,ft);
	break;
  case 14:
  	sg=hyperbolic_symmetry_group::group_axx(t[0].value,ft);
  }
  if(sg) {
  	double t = spinThickness->value();
  	lines->set_thickness(5*t,5*t*t,spinSharpness->value());
	lines->set_projtype((projtype)comboModel->currentIndex());
	lines->set_ncolors(spinColors->value());
    lines->paint(spinSize->value(),*sg);
	emit newImage(ImageData(lines->get_image(),false,nullptr,&(lines->get_layers())));
	delete sg;
  }
  else QMessageBox::information(this,"Hyperbolic Paintlines",tr("The chosen group is not hyperbolic.  Try increasing the parameters."));
}

void HyperbolicLinesWidget::symmetryChanged(int n)
{
	QLayoutItem *item;
	while((item = parameterLayout->takeAt(0))!=nullptr) {
		delete item->widget();
		delete item;
	}
	for(auto &t : tokens[n]) {
		if(t.type=='#')
			parameterLayout->addWidget(new SymmetrySpin(t));
		else
			parameterLayout->addWidget(new QLabel(QString(t.type)));
	}
}
