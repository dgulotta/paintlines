/***************************************************************************
 *   Copyright (C) 2013, 2014 by Daniel Gulotta                            *
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
#include "imagedata.hpp"
#include "lib/randomize.hpp"
#include "randomizewidget.hpp"

RandomizeWidget::RandomizeWidget(QWidget *parent)
:QWidget(parent)
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
	connect(buttonRandomize,&QPushButton::clicked,this,&RandomizeWidget::randomize);
}

void RandomizeWidget::imageUpdated(const ImageData &data)
{
	if(data.original) {
		const auto *sc = data.img.sym_view;
		if(sc)
			img=std::static_pointer_cast<symmetric_canvas<color_t>>(data.img.data);
		else
			img.reset();
		buttonRandomize->setEnabled((bool)sc);
	}
}

void RandomizeWidget::randomize()
{
	int xtiles = spinXTiles->value();
	int ytiles = spinYTiles->value();
	auto newimg = ::randomize(xtiles,ytiles,*img);
	emit newImage(ImageData(std::move(newimg),nullptr,false));
}
