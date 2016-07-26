/***************************************************************************
 *   Copyright (C) 2008, 2013-2014, 2016 by Daniel Gulotta                 *
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

#include "paintclouds.h"
#include "cloudswidget.h"
#include "../imagedata.h"

void ColorButton::mousePressEvent(QMouseEvent *)
{
  QColor col=QColorDialog::getColor(palette().color(QPalette::Window));
  if(col.isValid()) {
    setPalette(col);
  }
}

QComboBox * CloudsWidget::newComboRandom()
{
	QComboBox *comboRandom = new QComboBox;
	comboRandom->addItem(tr("Cauchy"));
	comboRandom->addItem(tr("Normal"));
	comboRandom->addItem(tr("Pseudo-Exponential"));
	comboRandom->addItem(tr("Sech^2"));
	return comboRandom;
}

CloudsWidget::CloudsWidget()
{
	QFormLayout *layout = new QFormLayout;
	spinSize = newSizeSpin();
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = new SymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	comboRandom = newComboRandom();
	layout->addRow(tr("Distribution"),comboRandom);
	color1 = new ColorButton(qRgb(255,255,0));
	layout->addRow(tr("Color 1"),color1);
	color2 = new ColorButton(qRgb(255,0,255));
	layout->addRow(tr("Color 2"),color2);
	color3 = new ColorButton(qRgb(0,255,255));
	layout->addRow(tr("Color 3"),color3);
	QPushButton *buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	setLayout(layout);
	connect(buttonDraw,&QPushButton::clicked,this,&CloudsWidget::draw);
}

static clouds_randfunc randfuncs[] = {
	&clouds_rand_cauchy,
	&clouds_rand_normal,
	&clouds_rand_exp_cos,
	&clouds_rand_sechsquare
};

void CloudsWidget::draw()
{
	if(spinSize->value()%2!=0) {
		QMessageBox::information(this,"paintclouds",tr("The size must be even."));
		return;
	}
	symgroup sg=symgroup(comboSymmetry->group());
	auto canvas=paint_clouds(spinSize->value(),sg,color1->color(),color2->color(),
		color3->color(),randfuncs[comboRandom->currentIndex()]);
	emit newImage(ImageData(std::move(canvas)));
}
