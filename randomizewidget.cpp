/***************************************************************************
 *   Copyright (C) 2013 by Daniel Gulotta                                  *
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
#include "basicform.h"
#include "randomize.h"
#include "randomizewidget.h"

RandomizeWidget::RandomizeWidget(QWidget *parent)
:QWidget(parent), src(nullptr)
{
	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Randomization")));
	spinXTiles = new QSpinBox;
	spinXTiles->setMinimum(1);
	spinXTiles->setValue(2);
	layout->addRow(tr("X"),spinXTiles);
	spinYTiles = new QSpinBox;
	spinYTiles->setMinimum(1);
	spinYTiles->setValue(2);
	layout->addRow(tr("Y"),spinYTiles);
	buttonRandomize = new QPushButton(tr("Randomize"));
	buttonRandomize->setEnabled(false);
	layout->addRow(buttonRandomize);
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
	connect(buttonRandomize,SIGNAL(clicked()),this,SLOT(randomize()));
	
}

void RandomizeWidget::imageUpdated(const ImageData &data)
{
	if(data.sym_canvas) {
		src=data.sym_canvas;
		buttonRandomize->setEnabled(true);
	}
}

void RandomizeWidget::randomize()
{
	int xtiles = spinXTiles->value();
	int ytiles = spinYTiles->value();
	auto img = ::randomize(xtiles,ytiles,*src);
	emit newImage(ImageData(img.as_canvas(),true));
}
