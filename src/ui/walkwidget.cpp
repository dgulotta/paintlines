/***************************************************************************
 *   Copyright (C) 2017 by Daniel Gulotta                                  *
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

#include "inputwidgets.hpp"
#include "lib/generators.hpp"
#include "genwidgets.hpp"
#include <QCheckBox>

WalkWidget::WalkWidget()
{
	auto wd = new SizeSpin(1);
	layout()->addRow("Width",wd);
	auto ht = new SizeSpin(1);
	layout()->addRow("Height",ht);
	auto ba = new QCheckBox("Balanced");
	ba->setChecked(true);
	layout()->addRow(ba);
	auto ti = new QCheckBox("Tiled");
	ti->setChecked(true);
	layout()->addRow(ti);
	auto wf = new QComboBox;
	wf->addItem("Area");
	wf->addItem("Length");
	layout()->addRow("Mode",wf);
	addGenerator("Draw",[=] () {
		if(ti->isChecked())
			return ImageData(draw_walk(wd->value(), ht->value(),
				ba->isChecked(), (walk_fill) wf->currentIndex()));
		else
			return ImageData(draw_walk2(wd->value(), ht->value(),
				ba->isChecked(), (walk_fill) wf->currentIndex()));
	});
}
