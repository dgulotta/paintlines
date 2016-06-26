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
		{ '2', '*', {2,2}, {2,2}, {2,2} },
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
	connect(comboSymmetry,(void (QComboBox::*)(int))&QComboBox::currentIndexChanged,this,&HyperbolicLinesWidget::symmetryChanged);
	connect(buttonDraw,&QPushButton::clicked,this,&HyperbolicLinesWidget::draw);
}

void HyperbolicLinesWidget::draw()
{
	hyperbolic_symmetry_group::group_spec spec;
	flip_type ft = (flip_type)comboSubset->currentIndex();
	const auto &t = tokens[comboSymmetry->currentIndex()];
	try {
		switch(comboSymmetry->currentIndex()) {
			case 0:
				spec=hyperbolic_symmetry_group::group_sax(t[1].value);
				break;
			case 1:
				spec=hyperbolic_symmetry_group::group_2sab(t[2].value,t[3].value);
				break;
			case 2:
				spec=hyperbolic_symmetry_group::group_a222(t[0].value);
				break;
			case 3:
				spec=hyperbolic_symmetry_group::group_ab2(t[0].value,t[1].value);
				break;
			case 4:
				spec=hyperbolic_symmetry_group::group_asb(t[0].value,t[2].value);
				break;
			case 5:
				spec=hyperbolic_symmetry_group::group_sabc(t[1].value,t[2].value,t[3].value);
				break;
			case 6:
				spec=hyperbolic_symmetry_group::group_a2x(t[0].value);
				break;
			case 7:
				spec=hyperbolic_symmetry_group::group_22sa(t[3].value);
				break;
			case 8:
				spec=hyperbolic_symmetry_group::group_asbc(t[0].value,t[2].value,t[3].value);
				break;
			case 9:
				spec=hyperbolic_symmetry_group::group_sabcd(t[1].value,t[2].value,t[3].value,t[4].value);
				break;
			case 10:
				spec=hyperbolic_symmetry_group::group_a2sb(t[0].value,t[3].value);
				break;
			case 11:
				spec=hyperbolic_symmetry_group::group_sasb(t[1].value,t[3].value);
				break;
			case 12:
				spec=hyperbolic_symmetry_group::group_ao(t[0].value);
				break;
			case 13:
				spec=hyperbolic_symmetry_group::group_abc(t[0].value,t[1].value,t[2].value);
				break;
			case 14:
				spec=hyperbolic_symmetry_group::group_axx(t[0].value);
				break;
			case 15:
				spec=hyperbolic_symmetry_group::group_2sabc(t[2].value,t[3].value,t[4].value);
		}
	} catch(const std::domain_error &) {
		QMessageBox::information(this,"Hyperbolic Paintlines",tr("The chosen group is not hyperbolic.  Try increasing the parameters."));
		return;
	}
	hyperbolic_symmetry_group sg(spec,ft);
	double th = spinThickness->value();
	hyperbolic_lines_param params{
		spinSize->value(),
			5*th,
			5*th*th,
			spinSharpness->value(),
			static_cast<projtype>(comboModel->currentIndex())
	};
	grids.resize(spinColors->value());
	generate(grids.begin(),grids.end(),[&sg,&params] () { return paint_hyperbolic_lines(sg,params); });
	layers.resize(grids.size());
	for(int i=0;i<grids.size();i++) {
		layers[i].pixels=&(grids[i]);
		layers[i].color=default_color_generator();
		layers[i].pastel=false;
	}
	canvas<color_t> img(spinSize->value(),spinSize->value());
	merge(img,layers);
	emit newImage(ImageData(img,false,nullptr,&layers));
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
