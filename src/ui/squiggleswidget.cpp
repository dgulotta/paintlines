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

#ifdef FFTWPROGS
#include <memory>
#include <QtWidgets>

#include "squiggleswidget.hpp"
#include "lib/generators.hpp"
#include "imagedata.hpp"
#include "inputwidgets.hpp"
#include "randomcolorwidget.hpp"

using std::make_shared;
using std::shared_ptr;
using std::vector;

SquigglesWidget::SquigglesWidget()
{
	QFormLayout *layout = this->layout();
	spinSize = new SizeSpin(2);
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = new SymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	spinColors = new QSpinBox;
	spinColors->setValue(DEFAULT_NCOLORS);
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
	connect(buttonDraw,&QPushButton::clicked,this,&SquigglesWidget::draw);
	connect(newColorButton,&QPushButton::clicked,this,&SquigglesWidget::resetColors);
}

void SquigglesWidget::draw()
{
	if(!colorWidget->load())
		QMessageBox::information(this,"paintsquggles",tr("Failed to load color palette image"));
	auto grids=paint_squiggles(spinColors->value(),
		spinSize->value(),
		comboSymmetry->group(),
		spinAlpha->value(),
		spinExponent->value(),
		spinThickness->value(),
		spinSharpness->value());
	layers = make_shared<vector<layer>>(grids.size());
	for(size_t i=0;i<grids.size();i++) {
		auto p_grid = make_shared<symmetric_canvas<uint8_t>>(std::move(grids[i]));
		(*layers)[i].pixels=shared_ptr<const canvas<uint8_t>>(p_grid,&(p_grid->as_canvas()));
		(*layers)[i].pastel=false;
	}
	updateImage();
	newColorButton->setEnabled(true);
}

void SquigglesWidget::updateImage()
{
	symmetric_canvas<color_t> image(spinSize->value(),comboSymmetry->group());
	for(size_t i=0;i<layers->size();i++)
		(*layers)[i].color=colorWidget->generate();
	merge(image.unsafe_get_canvas(),*layers);
	emit newImage(ImageData(std::move(image),layers));
}

void SquigglesWidget::resetColors()
{
	if(!colorWidget->load())
		QMessageBox::information(this,"paintsquggles",tr("Failed to load color palette image"));
	updateImage();
}

#endif
