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

#include "lib/paintclouds.hpp"
#include "genwidgets.hpp"
#include "hyperbolic_symmetry_chooser.hpp"
#include "inputwidgets.hpp"

Q_DECLARE_METATYPE(clouds_randfunc);

std::vector<std::pair<const char *,clouds_randfunc>> randfuncs = {
	{"Cauchy",&clouds_rand_cauchy},
	{"Normal",&clouds_rand_normal},
	{"Pseudo-Exponential",&clouds_rand_exp_cos},
	{"Sech^2",&clouds_rand_sechsquare}};

CloudsWidget::CloudsWidget()
{
	auto sz = new SizeSpin(2);
	layout()->addRow("Size",sz);
	auto sym = new SymmetryCombo;
	layout()->addRow("Symmetry",sym);
	auto col1 = new ColorButton(qRgb(255,255,0));
	layout()->addRow("Color 1",col1);
	auto col2 = new ColorButton(qRgb(255,0,255));
	layout()->addRow("Color 2",col2);
	auto col3 = new ColorButton(qRgb(0,255,255));
	layout()->addRow("Color 3",col3);
	DataComboAdapter<clouds_randfunc> rnd(randfuncs);
	layout()->addRow("Distribution",rnd.box());
	addGenerator("Draw",[=] () {
		return paint_clouds(sz->value(),sym->group(),col1->color(),
			col2->color(),col3->color(),rnd.value());
	});
}

HyperbolicCloudsWidget::HyperbolicCloudsWidget()
{
	auto sz = new SizeSpin;
	layout()->addRow("Size",sz);
	auto sym = new HyperbolicSymmetryChooser;
	layout()->addRow(sym);
	EnumComboAdapter<projtype> pt;
	layout()->addRow("Model",pt.box());
	auto col1 = new ColorButton(qRgb(255,255,0));
	layout()->addRow("Color 1",col1);
	auto col2 = new ColorButton(qRgb(255,0,255));
	layout()->addRow("Color 2",col2);
	auto col3 = new ColorButton(qRgb(0,255,255));
	layout()->addRow("Color 3",col3);
	DataComboAdapter<clouds_randfunc> rnd(randfuncs);
	layout()->addRow("Distribution",rnd.box());
	addGenerator("Draw",[=] () {
		return paint_hyperbolic_clouds(sz->value(),sym->domain(),pt.value(),
			col1->color(),col2->color(),col3->color(),rnd.value());
	});
}
