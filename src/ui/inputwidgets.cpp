/***************************************************************************
 *   Copyright (C) 2013-2016 by Daniel Gulotta                             *
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

#include <QColorDialog>
#include <QDoubleSpinBox>
#include "inputwidgets.hpp"
#include "lib/randgen.hpp"

SymmetryCombo::SymmetryCombo(bool random)
{
	for(int g=0;g<symmetryStrings.size();g++)
		addItem(symmetryStrings[g],g);
	if(random)
		addItem("Random",-1);
}

SymmetryCombo::SymmetryCombo(const std::vector<symgroup> &v, bool random)
{
	for(auto g : v) {
		int n=static_cast<int>(g);
		addItem(symmetryStrings[n],n);
	}
	if(random)
		addItem("Random",-1);
}

symgroup SymmetryCombo::group()
{
	int g=currentData().toInt();
	if(g<0)
		g=itemData(random_int(count()-1)).toInt();
	return static_cast<symgroup>(g);
}

const QStringList SymmetryCombo::symmetryStrings = {
	tr("CM (*x)"),
	tr("CMM (2*22)"),
	tr("P1 (o)"),
	tr("P2 (2222)"),
	tr("P3 (333)"),
	tr("P31M (3*3)"),
	tr("P3M1 (*333)"),
	tr("P4 (442)"),
	tr("P4G (4*2)"),
	tr("P4M (*442)"),
	tr("P6 (632)"),
	tr("P6M (*632)"),
	tr("PG (xx)"),
	tr("PGG (22x)"),
	tr("PM (**)"),
	tr("PMG (22*)"),
	tr("PMM (*2222)"),
};

SizeSpin::SizeSpin(int step)
{
	setRange(step,0x10000-(0x10000%step));
	setSingleStep(step);
	setValue(256);
}

int SizeSpin::valueFromText(const QString &text) const
{
	int val=QSpinBox::valueFromText(text);
	int rem=val%singleStep();
	if(rem)
		val+=singleStep()-rem;
	return val;
}

void ColorButton::mousePressEvent(QMouseEvent *)
{
	QColor col=QColorDialog::getColor(palette().color(QPalette::Window));
	if(col.isValid()) {
		setPalette(col);
	}
}

enum class projtype;
enum class flip_type;

template<>
const QStringList EnumComboAdapter<projtype>::defaultItems=
	{"Poincare","Klein"};

template<>
const QStringList EnumComboAdapter<flip_type>::defaultItems=
	{"All","Det > 0","Random"};
