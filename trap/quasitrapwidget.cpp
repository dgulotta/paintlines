/***************************************************************************
 *   Copyright (C) 2015 by Daniel Gulotta                                  *
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
#include "trap.h"
#include "quasitrapwidget.h"

QuasiTrapWidget::QuasiTrapWidget()
{
	QFormLayout *layout = new QFormLayout;
	spinHeight = new QSpinBox();
	spinHeight->setMinimum(1);
	spinHeight->setMaximum(65536);
	spinHeight->setValue(900);
	layout->addRow(tr("Height"),spinHeight);
	spinWidth = new QSpinBox();
	spinWidth->setMinimum(1);
	spinWidth->setMaximum(65536);
	spinWidth->setValue(1600);
	layout->addRow(tr("Width"),spinWidth);
	spinQuasiperiod = new QDoubleSpinBox;
	spinQuasiperiod->setMinimum(1);
	spinQuasiperiod->setMaximum(65536);
	spinQuasiperiod->setValue(100);
	layout->addRow(tr("Quasiperiod"),spinQuasiperiod);
	comboSymmetry = new QComboBox;
	comboSymmetry->addItem("5",5);
	comboSymmetry->addItem("8",8);
	comboSymmetry->addItem("10",10);
	comboSymmetry->addItem("12",12);
	layout->addRow(tr("Symmetry"),comboSymmetry);
	comboType = new QComboBox;
	comboType->addItem("Trig");
	comboType->addItem("Polynomial");
	layout->addRow(tr("Type"),comboType);
	QPushButton *buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	connect(buttonDraw,&QPushButton::clicked,this,&QuasiTrapWidget::draw);
	setLayout(layout);
}

void QuasiTrapWidget::draw()
{
	img=canvas<color_t>(spinWidth->value(),spinHeight->value());
	auto fn = comboType->currentIndex() ? drawquasitrap_poly : drawquasitrap;
	fn(img,comboSymmetry->currentData().toInt(),spinQuasiperiod->value());
	emit newImage(ImageData(std::move(img)));
}
