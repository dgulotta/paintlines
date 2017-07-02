/***************************************************************************
 *   Copyright (C) 2014 by Daniel Gulotta                                  *
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
#include "lib/converters.hpp"
#include "converterwidget.hpp"
#include "randomizewidget.hpp"

ConverterWidget::ConverterWidget()
{
	QFormLayout *layout = this->layout();
	randomizeWidget = new RandomizeWidget;
	layout->addRow(randomizeWidget);
	spinSize = new QSpinBox();
	spinSize->setMinimum(1);
	spinSize->setMaximum(65536);
	spinSize->setValue(256);
	layout->addRow(tr("Size"),spinSize);
	layout->addRow(tr("Model"),comboModel.box());
	buttonHyperbolic = new QPushButton(tr("Make hyperbolic"));
	buttonHyperbolic->setEnabled(false);
	layout->addRow(buttonHyperbolic);
	buttonHexStretch = new QPushButton("&Hexagonal Stretch");
	buttonHexStretch->setEnabled(false);
	layout->addRow(buttonHexStretch);
	buttonRestore = new QPushButton("&Restore");
	buttonRestore->setEnabled(false);
	layout->addRow(buttonRestore);
	connect(buttonHyperbolic,&QPushButton::clicked,this,&ConverterWidget::makeHyperbolic);
	connect(buttonHexStretch,&QPushButton::clicked,this,&ConverterWidget::hexagonalStretch);
	connect(buttonRestore,&QPushButton::clicked,this,&ConverterWidget::restore);
}

void ConverterWidget::makeConnections(MainForm *f)
{
	connect(f,&MainForm::newImage,this,&ConverterWidget::imageUpdated);
	connect(f,&MainForm::newImage,randomizeWidget,&RandomizeWidget::imageUpdated);
	connect(this,&ImageGeneratorWidget::newImage,f,&MainForm::newImage);
	connect(randomizeWidget,&RandomizeWidget::newImage,f,&MainForm::newImage);
}

void ConverterWidget::imageUpdated(const ImageData &data)
{
	if(data.original) {
		buttonRestore->setEnabled(false);
		const auto *sc = data.img.sym_view;
		if(sc)
			restoreData=data;
		else
			restoreData.img.data.reset();
		buttonHexStretch->setEnabled((bool)sc);
		buttonHyperbolic->setEnabled((bool)sc);
	}
	else
		buttonRestore->setEnabled(true);
}

void ConverterWidget::restore()
{
	emit newImage(restoreData);
}

void ConverterWidget::hexagonalStretch()
{
	auto img = hexagonal_stretch(*(restoreData.img.sym_view));
	emit newImage(ImageData(std::move(img),nullptr,false));
}

void ConverterWidget::makeHyperbolic()
{
	auto img = make_hyperbolic(*(restoreData.img.sym_view),
		comboModel.value(),spinSize->value());
	emit newImage(ImageData(std::move(img),nullptr,false));
}
