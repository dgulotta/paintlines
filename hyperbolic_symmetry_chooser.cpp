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

#include <algorithm>
#include <functional>
#include <QtWidgets>
#include "hyperbolic_symmetry_chooser.h"
#include "paintclouds/paintclouds.h"

using namespace std::placeholders;

HyperbolicSymmetryItem::HyperbolicSymmetryItem(const fundamental_domain_family &fdf,
	const std::vector<int> &defaults)
:family(fdf)
{
	int idx=0;
	auto layout = new QHBoxLayout;
	auto addToken = [this,layout] (const char *ch) {
		name+=ch;
		layout->addWidget(new QLabel(ch));
	};
	for(int i : fdf.rotation_point_counts())
	{
		auto spin = new QSpinBox;
		spins.push_back(spin);
		spin->setMinimum(1+2/i);
		spin->setValue(defaults[idx]);
		layout->addWidget(spin);
		name+='a'+(char)idx;
		++idx;
	}
	for(int i=fdf.rot180s();--i>=0;)
		addToken("2");
	for(auto &v : fdf.mirror_point_counts()) {
		addToken("*");
		for(int i: v) {
			auto spin = new QSpinBox;
			spins.push_back(spin);
			spin->setMinimum(1+1/i);
			spin->setValue(defaults[idx]);
			layout->addWidget(spin);
			name+='a'+(char)idx;
			++idx;
		}
	}
	int loops = fdf.loops();
	if(!fdf.orientable()) {
		loops--;
		addToken("x");
		if(loops%2)
			addToken("x");
	}
	loops/=2;
	while(--loops>=0)
		addToken("o");
	setLayout(layout);
}

fundamental_domain HyperbolicSymmetryItem::domain() const
{
	std::vector<int> values(spins.size());
	std::transform(spins.begin(),spins.end(),values.begin(),std::bind(&QSpinBox::value,_1));
	return family.domain(values);
}

HyperbolicSymmetryChooser::HyperbolicSymmetryChooser(QWidget *parent)
:QWidget(parent)
{
	familyCombo = new QComboBox;
	itemStack = new QStackedLayout;
	auto layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(familyCombo);
	layout->addLayout(itemStack);
	setLayout(layout);
	connect(familyCombo,(void(QComboBox::*)(int))&QComboBox::activated,itemStack,&QStackedLayout::setCurrentIndex);
}

void HyperbolicSymmetryChooser::add(const fundamental_domain_family &fdf,
	const std::vector<int> &defaults)
{
	auto item = new HyperbolicSymmetryItem(fdf,defaults);
	familyCombo->addItem(item->group_name());
	itemStack->addWidget(item);
}

void HyperbolicSymmetryChooser::addDefaultItems()
{
	auto csg = clouds_supported_groups();
	add(csg[0],{2});
	add(csg[1],{3,2});
	add(csg[2],{3});
	add(csg[3],{5,4});
	add(csg[4],{5,2});
	add(csg[5],{5,4,2});
	add(csg[6],{3});
	add(csg[7],{2});
	add(csg[8],{3,2,2});
	add(csg[9],{3,2,2,2});
	add(csg[10],{3,1});
	add(csg[11],{2,1});
	add(csg[12],{2});
	add(csg[13],{5,2,4});
	add(csg[14],{2});
	add(csg[15],{2,2,2});
}

HyperbolicSymmetryItem * HyperbolicSymmetryChooser::currentItem() const
{
	return static_cast<HyperbolicSymmetryItem *>(itemStack->currentWidget());
}
