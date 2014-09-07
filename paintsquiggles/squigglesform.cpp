/***************************************************************************
 *   Copyright (C) 2008, 2013 by Daniel Gulotta                            *
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

#include "squigglesform.h"
#include "../paintsquiggles.h"
#include "../randomcolorwidget.h"
#include "../randomizewidget.h"

void SquigglesForm::init()
{
	QFormLayout *layout = new QFormLayout;
	spinSize = newSizeSpin();
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = newSymmetryCombo();
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
	buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	newColorButton = new QPushButton(tr("Change Colors"));
	layout->addRow(newColorButton);
	newColorButton->setEnabled(false);
	randomizeWidget = new RandomizeWidget;
	layout->addRow(randomizeWidget);
	buttonRestore = new RestoreButton;
	layout->addRow(buttonRestore);
	checkTiled = newTileCheck();
	layout->addRow(checkTiled);
	squiggles=new paintsquiggles;
	squiggles->set_color_generator(std::bind(&RandomColorWidget::generate,colorWidget));
#ifdef MULTIPAGE
	saver = new LayeredImageSaver(this);
#else
	saver = new ImageSaver(this);
#endif
	sideLayout = layout;
	connect(buttonRestore,&QPushButton::clicked,this,&SquigglesForm::updateImage);
	connect(newColorButton,&QPushButton::clicked,this,&SquigglesForm::resetColors);
	connect(this,&BasicForm::newImage,buttonRestore,&RestoreButton::newImage);
	connect(this,&BasicForm::newImage,randomizeWidget,&RandomizeWidget::imageUpdated);
	connect(randomizeWidget,&RandomizeWidget::newImage,this,&BasicForm::newImage);
}

void SquigglesForm::draw()
{
	if(spinSize->value()%2!=0) {
		QMessageBox::information(this,"paintsquiggles",tr("The size must be even."));
		return;
	}
	squiggles->set_alpha(spinAlpha->value());
	squiggles->set_exponent(spinExponent->value());
	squiggles->set_ncolors(spinColors->value());
	squiggles->set_thickness(spinThickness->value());
	squiggles->set_sharpness(spinSharpness->value());
	if(!colorWidget->load())
		QMessageBox::information(this,"paintsquggles",tr("Failed to load color palette image"));
	squiggles->paint(spinSize->value(),(symgroup)comboSymmetry->currentIndex());
	updateImage();
	newColorButton->setEnabled(true);
}

void SquigglesForm::updateImage()
{
	ImageData data(squiggles->get_image(),squiggles->get_symmetric_image(),&(squiggles->get_layers()));
	emit newImage(data);
}

void SquigglesForm::resetColors()
{
	if(!colorWidget->load())
		QMessageBox::information(this,"paintsquggles",tr("Failed to load color palette image"));
	squiggles->choose_new_colors();
	updateImage();

}
