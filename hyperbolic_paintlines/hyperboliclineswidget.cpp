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

#include "../hyperbolic_symmetry_chooser.h"
#include "../imagedata.h"
#include "hyperboliclineswidget.h"
#include "hyperbolic_paintlines.h"

HyperbolicLinesWidget::HyperbolicLinesWidget()
{
	QFormLayout *layout = new QFormLayout;
	comboModel = new QComboBox;
	comboModel->addItem(tr("Poincare"));
	comboModel->addItem(tr("Klein"));
	layout->addRow(tr("Model"),comboModel);
	chooser = new HyperbolicSymmetryChooser;
	chooser->addDefaultItems();
	layout->addRow(chooser);
	comboSubset=new QComboBox;
	comboSubset->addItem(tr("All"));
	comboSubset->addItem(tr("Det > 0"));
	comboSubset->addItem(tr("Random"));
	layout->addRow(tr("Subset"),comboSubset);
	spinSize=new SizeSpin;
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
	connect(buttonDraw,&QPushButton::clicked,this,&HyperbolicLinesWidget::draw);
}

void HyperbolicLinesWidget::draw()
{
	flip_type ft = static_cast<flip_type>(comboSubset->currentIndex());
	fundamental_domain fd;
	try {
		fd=chooser->domain();
	} catch(const std::domain_error &) {
		QMessageBox::information(this,"Hyperbolic Paintlines",tr("The chosen group is not hyperbolic.  Try increasing the parameters."));
		return;
	}
	hyperbolic_symmetry_group sg(fd.generators,ft);
	double th = spinThickness->value();
	hyperbolic_lines_param params{
		spinSize->value(),
			(float)(5*th),
			(float)(5*th*th),
			(float)(spinSharpness->value()),
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
	emit newImage(ImageData(std::move(img),&layers));
}
