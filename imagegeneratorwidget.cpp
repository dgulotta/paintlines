/***************************************************************************
 *   Copyright (C) 2013-2015 by Daniel Gulotta                             *
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
#include "imagegeneratorwidget.h"

const QStringList ImageGeneratorWidget::symmetryStrings = {
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

QComboBox * ImageGeneratorWidget::newSymmetryCombo()
{
	QComboBox *comboSymmetry = new QComboBox;
	comboSymmetry->addItems(symmetryStrings);
	return comboSymmetry;
}

QComboBox * ImageGeneratorWidget::newSymmetryCombo(const std::vector<int> &v)
{
	QComboBox *comboSymmetry = new QComboBox;
	for(int g : v)
		comboSymmetry->addItem(symmetryStrings[g],g);
	return comboSymmetry;
}

QSpinBox * ImageGeneratorWidget::newSizeSpin() {
	QSpinBox *spinSize = new QSpinBox;
	spinSize->setMinimum(2);
	spinSize->setMaximum(65536);
	spinSize->setSingleStep(2);
	spinSize->setValue(256);
	return spinSize;
}

QSpinBox * ImageGeneratorWidget::newColorSpin() {
	QSpinBox *spinColors = new QSpinBox;
	spinColors->setValue(25);
	return spinColors;
}

