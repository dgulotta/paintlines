/***************************************************************************
 *   Copyright (C) 2016 by Daniel Gulotta                                  *
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
#include <QDoubleSpinBox>
#include "inputwidgets.hpp"
#include "lib/generators.hpp"
#include "genwidgets.hpp"

StripesWidget::StripesWidget()
{
	auto sz = new SizeSpin(2);
	layout()->addRow("Size",sz);
	auto sym = new SymmetryCombo;
	layout()->addRow("Symmetry",sym);
	auto alpha = new QDoubleSpinBox;
	alpha->setRange(.01,2.);
	alpha->setValue(1.);
	layout()->addRow("Alpha",alpha);
	addGenerator("Draw",[=] () {
		return paint_stripes(sz->value(),sym->group(),alpha->value());
	});
}

#endif
