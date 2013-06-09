/***************************************************************************
 *   Copyright (C) 2008, 2013 by Daniel Gulotta                            *
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

#include <QtGui>

#include "squigglesform.h"
#include "paintsquiggleswidget.h"

void SquigglesForm::addWidgets(QBoxLayout *sideLayout)
{
  sideLayout->addWidget(new QLabel(tr("Colors")));
  spinColors=new QSpinBox;
  spinColors->setMinimum(1);
  spinColors->setMaximum(65536);
  spinColors->setValue(25);
  sideLayout->addWidget(spinColors);
  sideLayout->addWidget(new QLabel(tr("Alpha")));
  spinAlpha = new QDoubleSpinBox;
  spinAlpha->setMinimum(.01);
  spinAlpha->setMaximum(2.);
  spinAlpha->setValue(2.);
  sideLayout->addWidget(spinAlpha);
  sideLayout->addWidget(new QLabel(tr("Thickness")));
  spinThickness = new QDoubleSpinBox;
  spinThickness->setValue(1.);
  sideLayout->addWidget(spinThickness);
  sideLayout->addWidget(new QLabel(tr("Sharpness")));
  spinSharpness = new QDoubleSpinBox;
  spinSharpness->setValue(2.);
  sideLayout->addWidget(spinSharpness);
}

void SquigglesForm::draw(int sz, int sym_index)
{
	symgroup sg=symgroup(sym_index);
	squiggles->set_alpha(spinAlpha->value());
	squiggles->set_ncolors(spinColors->value());
	squiggles->set_thickness(spinThickness->value());
	squiggles->set_sharpness(spinSharpness->value());
	squiggles->draw(sz,sg);
}

painterwidget * SquigglesForm::createPainterWidget()
{
	squiggles=new paintsquiggleswidget;
	return squiggles;
}
