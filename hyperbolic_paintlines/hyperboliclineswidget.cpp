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

#include <QDoubleSpinBox>
#include "../hyperbolic_symmetry_chooser.h"
#include "../imagedata.h"
#include "../inputwidgets.h"
#include "hyperboliclineswidget.h"
#include "hyperbolic_paintlines.h"

HyperbolicLinesWidget::HyperbolicLinesWidget()
{
	EnumComboAdapter<projtype> comboModel;
	layout()->addRow(tr("Model"),comboModel.box());
	auto chooser = new HyperbolicSymmetryChooser;
	chooser->addDefaultItems();
	layout()->addRow(chooser);
	EnumComboAdapter<flip_type> comboSubset;
	layout()->addRow(tr("Subset"),comboSubset.box());
	auto spinSize=new SizeSpin;
	layout()->addRow(tr("Size"),spinSize);
	auto spinColors=new QSpinBox;
	spinColors->setValue(DEFAULT_NCOLORS);
	layout()->addRow(tr("Colors"),spinColors);
	auto spinThickness = new QDoubleSpinBox;
	spinThickness->setValue(1);
	layout()->addRow(tr("Thickness"),spinThickness);
	auto spinSharpness = new QDoubleSpinBox;
	spinSharpness->setValue(1.5);
	layout()->addRow(tr("Sharpness"),spinSharpness);
	addGenerator("Draw",[=] () {
		hyperbolic_symmetry_group sg(chooser->domain().generators,
			comboSubset.value());
		double th = spinThickness->value();
		hyperbolic_lines_param params{
			spinSize->value(),
			(float)(5*th),
			(float)(5*th*th),
			(float)(spinSharpness->value()),
			comboModel.value()};
		grids.resize(spinColors->value());
		generate(grids.begin(),grids.end(),[&sg,&params] () { return paint_hyperbolic_lines(sg,params); });
		layers.resize(grids.size());
		for(int i=0;i<grids.size();i++) {
			layers[i].pixels=&(grids[i]);
			layers[i].color=default_color_generator();
			layers[i].pastel=false;
		}
		canvas<color_t> img(spinSize->value(),spinSize->value());
		merge(img,layers);
		return img;
	},&layers);
}
