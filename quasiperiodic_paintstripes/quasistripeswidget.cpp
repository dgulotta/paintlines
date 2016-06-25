/***************************************************************************
 *   Copyright (C) 2008, 2014, 2016 by Daniel Gulotta                      *
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
#include "quasiperiodic_paintstripes.h"
#include "quasistripeswidget.h"

QuasiStripesWidget::QuasiStripesWidget()
{
	QFormLayout *layout = new QFormLayout;
	spinSize = new QSpinBox;
	spinSize->setMinimum(1);
	spinSize->setMaximum(65536);
	spinSize->setValue(256);
	layout->addRow(tr("Size"),spinSize);
	spinQuasiSize = new QSpinBox;
	spinQuasiSize->setMinimum(1);
	spinQuasiSize->setMaximum(256);
	spinQuasiSize->setValue(16);
	layout->addRow(tr("Quasiperiod"),spinQuasiSize);
	spinAlpha = new QDoubleSpinBox;
	spinAlpha->setMinimum(.01);
	spinAlpha->setMaximum(2.);
	spinAlpha->setValue(1.);
	layout->addRow(tr("Alpha"),spinAlpha);
	QPushButton *buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	setLayout(layout);
	connect(buttonDraw,&QPushButton::clicked,this,&QuasiStripesWidget::draw);
}

void QuasiStripesWidget::draw()
{
	canvas<color_t> img=paint_quasiperiodic_stripes(spinSize->value(),
		spinQuasiSize->value(),spinAlpha->value());
	emit newImage(img);
}
