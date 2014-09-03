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

#include "../paintclouds.h"
#include "../randomizewidget.h"
#include "cloudsform.h"

void ColorButton::mousePressEvent(QMouseEvent *event)
{
  QColor col=QColorDialog::getColor(palette().color(QPalette::Window));
  if(col.isValid()) {
    setPalette(col);
  }
}

void CloudsForm::init() {
	QFormLayout *layout = new QFormLayout;
	spinSize = newSizeSpin();
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = newSymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	comboRandom = new QComboBox;
	comboRandom->addItem(tr("Cauchy"));
	comboRandom->addItem(tr("Normal"));
	comboRandom->addItem(tr("Pseudo-Exponential"));
	comboRandom->addItem(tr("Sech^2"));
	layout->addRow(tr("Distribution"),comboRandom);
	color1 = new ColorButton(qRgb(255,255,0));
	layout->addRow(tr("Color 1"),color1);
	color2 = new ColorButton(qRgb(255,0,255));
	layout->addRow(tr("Color 2"),color2);
	color3 = new ColorButton(qRgb(0,255,255));
	layout->addRow(tr("Color 3"),color3);
	buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	randomizeWidget = new RandomizeWidget;
	layout->addRow(randomizeWidget);
	buttonRestore = new RestoreButton;
	layout->addRow(buttonRestore);
	checkTiled = newTileCheck();
	layout->addRow(checkTiled);
	clouds = new paintclouds;
	saver = new ImageSaver(this);
	sideLayout = layout;
	connect(buttonRestore,SIGNAL(clicked()),this,SLOT(updateImage()));
	connect(this,SIGNAL(newImage(const ImageData &)),buttonRestore,SLOT(newImage(const ImageData &)));
	connect(this,SIGNAL(newImage(const ImageData &)),randomizeWidget,SLOT(imageUpdated(const ImageData &)));
	connect(randomizeWidget,SIGNAL(newImage(const ImageData &)),this,SIGNAL(newImage(const ImageData &)));
}

static double (*(randfuncs[4]))(double) = {
	&paintclouds::rand_cauchy,
	&paintclouds::rand_normal,
	&paintclouds::rand_exp_cos,
	&paintclouds::rand_sechsquare
};

void CloudsForm::draw()
{
	if(spinSize->value()%2!=0) {
		QMessageBox::information(this,"paintclouds",tr("The size must be even."));
		return;
	}
	symgroup sg=symgroup(comboSymmetry->currentIndex());
	QColor c=color1->palette().color(QPalette::Window);
    clouds->set_color1(c.red(),c.green(),c.blue());
    c=color2->palette().color(QPalette::Window);
    clouds->set_color2(c.red(),c.green(),c.blue());
    c=color3->palette().color(QPalette::Window);
    clouds->set_color3(c.red(),c.green(),c.blue());
	clouds->set_randfunc(randfuncs[comboRandom->currentIndex()]);
    clouds->paint(spinSize->value(),sg);
	updateImage();
}

void CloudsForm::updateImage() {
	ImageData data(clouds->get_image(),clouds->get_symmetric_image());
	emit newImage(data);
}
