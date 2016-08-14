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

#include <QDoubleSpinBox>
#include "../inputwidgets.h"
#include "quasiperiodic_paintstripes.h"
#include "quasistripeswidget.h"

QuasiStripesWidget::QuasiStripesWidget()
{
	auto sz = new SizeSpin;
	layout()->addRow("Size",sz);
	auto qsz = new QSpinBox;
	qsz->setRange(1,256);
	qsz->setValue(16);
	layout()->addRow("Quasiperiod",qsz);
	auto alpha = new QDoubleSpinBox;
	alpha->setRange(.01,2.);
	alpha->setValue(1.);
	addGenerator("Draw",[=] () {
		return paint_quasiperiodic_stripes(sz->value(),qsz->value(),
			alpha->value());
	});
}
