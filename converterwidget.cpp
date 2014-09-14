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
#include "converter/converters.h"
#include "converterwidget.h"
#include "randomizewidget.h"

ConverterWidget::ConverterWidget()
{
	QFormLayout *layout = new QFormLayout;
	randomizeWidget = new RandomizeWidget;
	layout->addRow(randomizeWidget);
	spinSize = new QSpinBox();
	spinSize->setMinimum(1);
	spinSize->setMaximum(65536);
	spinSize->setValue(256);
	layout->addRow(tr("Size"),spinSize);
	comboModel = new QComboBox;
	comboModel->addItem(tr("Poincare"));
	comboModel->addItem(tr("Klein"));
	layout->addRow(tr("Model"),comboModel);
	buttonHyperbolic = new QPushButton(tr("Make hyperbolic"));
	buttonHyperbolic->setEnabled(false);
	layout->addRow(buttonHyperbolic);
	buttonHexStretch = new QPushButton("&Hexagonal Stretch");
	buttonHexStretch->setEnabled(false);
	layout->addRow(buttonHexStretch);
	buttonRestore = new QPushButton("&Restore");
	buttonRestore->setEnabled(false);
	layout->addRow(buttonRestore);
	setLayout(layout);
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
	if(data.parent==nullptr) {
		restoreData=data;
		buttonRestore->setEnabled(false);
		buttonHexStretch->setEnabled(data.sym_canvas!=nullptr);
		buttonHyperbolic->setEnabled(data.sym_canvas!=nullptr);
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
	emit newImage(ImageData(hexagonal_stretch(*(restoreData.sym_canvas)),&restoreData));
}

void ConverterWidget::makeHyperbolic()
{
	emit newImage(ImageData(make_hyperbolic(*(restoreData.sym_canvas),(projtype)comboModel->currentIndex(),spinSize->value()),false,nullptr,nullptr,&restoreData));
}
