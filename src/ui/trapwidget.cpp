/***************************************************************************
 *   Copyright (C) 2014-2015 by Daniel Gulotta                             *
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
#include "imagedata.hpp"
#include "inputwidgets.hpp"
#include "lib/generators.hpp"
#include "genwidgets.hpp"

TrapWidget::TrapWidget()
{
	auto spinSize = new SizeSpin(2);
	layout()->addRow(tr("Size"),spinSize);
	auto comboSymmetry = new SymmetryCombo;
	layout()->addRow(tr("Symmetry"),comboSymmetry);
	addGenerator("Draw",[=] () {
		symmetric_canvas<color_t> img(spinSize->value(),comboSymmetry->group());
		drawtrap(img);
		return img;
	});
}

QuasiTrapWidget::QuasiTrapWidget()
{
	auto spinHeight = new QSpinBox;
	spinHeight->setRange(1,0x10000);
	spinHeight->setValue(900);
	layout()->addRow(tr("Height"),spinHeight);
	auto spinWidth = new QSpinBox;
	spinWidth->setRange(1,0x10000);
	spinWidth->setValue(1600);
	layout()->addRow(tr("Width"),spinWidth);
	auto spinQuasiperiod = new QDoubleSpinBox;
	spinQuasiperiod->setRange(1,0x10000);
	spinQuasiperiod->setValue(100);
	layout()->addRow(tr("Quasiperiod"),spinQuasiperiod);
	DataComboAdapter<int> comboSymmetry({{"5",5},{"8",8},{"10",10},{"12",12}});
	layout()->addRow(tr("Symmetry"),comboSymmetry.box());
	auto comboType = new QComboBox;
	comboType->addItems({"Trig","Polynomial"});
	layout()->addRow(tr("Type"),comboType);
	addGenerator("Draw",[=] () {
		auto img=canvas<color_t>(spinWidth->value(),spinHeight->value());
		auto fn = comboType->currentIndex() ? drawquasitrap_poly : drawquasitrap;
		fn(img,comboSymmetry.value(),spinQuasiperiod->value());
		return img;
	});
}
