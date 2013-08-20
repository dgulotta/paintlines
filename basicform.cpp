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

#include "basicform.h"

BasicForm::BasicForm()
{
	QMetaObject::invokeMethod(this,"baseInit",Qt::QueuedConnection);
}

void BasicForm::baseInit()
{
	menu = menuBar();
	menuFile = menu->addMenu(tr("&File"));
	actionSaveAs = menuFile->addAction(tr("&Save As"));
	actionExit = menuFile->addAction(tr("E&xit"));
	labelImage = new QLabel;
	labelImage->setAlignment(Qt::AlignLeft|Qt::AlignTop);
	init();
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(sideLayout);
	mainLayout->addWidget(labelImage);
	mainLayout->addStretch(1);
	QWidget *w = new QWidget;
	w->setLayout(mainLayout);
	QScrollArea *a = new QScrollArea;
	a->setWidgetResizable(true);
	a->setWidget(w);
	setCentralWidget(a);
	resize(800,600);
	connect(buttonDraw,SIGNAL(clicked()),this,SLOT(draw()));
	connect(actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAs()));
	connect(actionExit,SIGNAL(triggered()),this,SLOT(close()));
	connect(this,SIGNAL(newImage(QPixmap)),labelImage,SLOT(setPixmap(const QPixmap &)));
	connect(this,SIGNAL(newImage(QPixmap)),saver,SLOT(newImage(const QPixmap &)));
}

BasicForm::~BasicForm()
{
}

bool BasicForm::saveAs()
{
	QString s=QFileDialog::getSaveFileName();
	if(!s.isEmpty())
		return saver->save(s);
	else
    	return false;
}

QPixmap BasicForm::makePixmap(const canvas<color_t> &src)
{
	QImage image(src.width(),src.height(),QImage::Format_RGB32);
	for(int j=0;j<src.height();j++)
		for(int i=0;i<src.width();i++) {
			color_t col = src(i,j);
      		image.setPixel(i,j,qRgb(col.red,col.green,col.blue));
		}
  	return QPixmap::fromImage(image);
}

const QStringList BasicForm::symmetryStrings = {
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

QComboBox * BasicForm::newSymmetryCombo()
{
	QComboBox *comboSymmetry = new QComboBox;
	comboSymmetry->addItems(symmetryStrings);
	return comboSymmetry;
}

QSpinBox * BasicForm::newSizeSpin() {
	QSpinBox *spinSize = new QSpinBox;
	spinSize->setMinimum(2);
	spinSize->setMaximum(65536);
	spinSize->setSingleStep(2);
	spinSize->setValue(256);
	return spinSize;
}

QSpinBox * BasicForm::newColorSpin() {
	QSpinBox *spinColors = new QSpinBox;
	spinColors->setValue(25);
	return spinColors;
}
