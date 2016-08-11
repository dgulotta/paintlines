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

#include "../imagedata.h"
#include "paintstripes.h"
#include "stripeswidget.h"

StripesWidget::StripesWidget()
{
	QFormLayout *layout = new QFormLayout;
	spinSize = new SizeSpin(2);
	layout->addRow(tr("Size"),spinSize);
	comboSymmetry = new SymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	spinAlpha = new QDoubleSpinBox;
	spinAlpha->setMinimum(.01);
	spinAlpha->setMaximum(2.);
	spinAlpha->setValue(1.);
	layout->addRow(tr("Alpha"),spinAlpha);
	QPushButton *buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	setLayout(layout);
	connect(buttonDraw,&QPushButton::clicked,this,&StripesWidget::draw);
}

void StripesWidget::draw()
{
	auto img=paint_stripes(spinSize->value(),(symgroup)comboSymmetry->group(),spinAlpha->value());
	emit newImage(ImageData(std::move(img)));
}
