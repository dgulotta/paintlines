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

#include "squiggleswidget.h"
#include "paintsquiggles.h"
#include "../imagedata.h"
#include "../randomcolorwidget.h"

SquigglesWidget::SquigglesWidget()
{
	QFormLayout *layout = new QFormLayout;
	spinSize = newSizeSpin();
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = new SymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	spinColors = newColorSpin();
	layout->addRow(tr("Colors"),spinColors);
	spinAlpha = new QDoubleSpinBox;
	spinAlpha->setMinimum(.01);
	spinAlpha->setMaximum(2.);
	spinAlpha->setValue(2.);
	layout->addRow(tr("Alpha"),spinAlpha);
	spinExponent = new QDoubleSpinBox;
	spinExponent->setValue(2.);
	layout->addRow(tr("Exponent"),spinExponent);
	spinThickness = new QDoubleSpinBox;
	spinThickness->setValue(1.);
	layout->addRow(tr("Thickness"),spinThickness);
	spinSharpness = new QDoubleSpinBox;
	spinSharpness->setValue(2.);
	layout->addRow(tr("Sharpness"),spinSharpness);
	colorWidget = new RandomColorWidget;
	layout->addRow(colorWidget);
	QPushButton *buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	newColorButton = new QPushButton(tr("Change Colors"));
	layout->addRow(newColorButton);
	newColorButton->setEnabled(false);
	setLayout(layout);
	connect(buttonDraw,&QPushButton::clicked,this,&SquigglesWidget::draw);
	connect(newColorButton,&QPushButton::clicked,this,&SquigglesWidget::resetColors);
}

void SquigglesWidget::draw()
{
	if(spinSize->value()%2!=0) {
		QMessageBox::information(this,"paintsquiggles",tr("The size must be even."));
		return;
	}
	if(!colorWidget->load())
		QMessageBox::information(this,"paintsquggles",tr("Failed to load color palette image"));
	grids=paint_squiggles(spinColors->value(),
		spinSize->value(),
		comboSymmetry->group(),
		spinAlpha->value(),
		spinExponent->value(),
		spinThickness->value(),
		spinSharpness->value());
	layers.resize(grids.size());
	for(size_t i=0;i<grids.size();i++) {
		layers[i].pixels=&(grids[i].as_canvas());
		layers[i].pastel=false;
	}
	updateImage();
	newColorButton->setEnabled(true);
}

void SquigglesWidget::updateImage()
{
	symmetric_canvas<color_t> image(spinSize->value(),comboSymmetry->group());
	for(size_t i=0;i<grids.size();i++)
		layers[i].color=colorWidget->generate();
	merge(image.unsafe_get_canvas(),layers);
	emit newImage(ImageData(std::move(image),&layers));
}

void SquigglesWidget::resetColors()
{
	if(!colorWidget->load())
		QMessageBox::information(this,"paintsquggles",tr("Failed to load color palette image"));
	updateImage();
}
