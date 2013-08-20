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

#include "randomcolorwidget.h"
#include "randgen.h"
#include <QtGui>
#include <tuple>

RandomColorWidget::RandomColorWidget()
{
	QGridLayout * layout = new QGridLayout;
	layout->addWidget(new QLabel(tr("Get colors from image")),0,0,1,2);
	lineImagePath = new QLineEdit;
	layout->addWidget(lineImagePath,1,0,1,1);
	QToolButton *button = new QToolButton;
	button->setText(tr("..."));
	layout->addWidget(button,1,1,1,1);
	connect(button,SIGNAL(clicked()),this,SLOT(chooseImage()));
	checkCopyHue = new QCheckBox(tr("Use hue"));
	layout->addWidget(checkCopyHue,2,0,1,2);
	checkCopySaturation = new QCheckBox(tr("Use saturation"));
	layout->addWidget(checkCopySaturation,3,0,1,2);
	checkCopyLightness = new QCheckBox(tr("Use lightness"));
	layout->addWidget(checkCopyLightness,4,0,1,2);
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
}

color_t RandomColorWidget::generate()
{
	bool copyH = (!image.isNull())&&checkCopyHue->isChecked();
	bool copyS = (!image.isNull())&&checkCopySaturation->isChecked();
	bool copyL = (!image.isNull())&&checkCopyLightness->isChecked();
	QColor col;
	if(copyH||copyS||copyL)
		col = image.pixel(random_int(image.width()),random_int(image.height()));
	int h = copyH ? col.hslHue() : random_int(360);
	int s = copyS ? col.hslSaturation() : 255;
	int l = copyL ? col.lightness() : 128;
	col = QColor::fromHsl(h,s,l);
	return color_t(col.red(),col.green(),col.blue());
}

bool RandomColorWidget::load()
{
	if(!(checkCopyHue->isChecked()||checkCopySaturation->isChecked()||checkCopyLightness->isChecked())) {
		image = QImage();
		return true;
	}
	else if(image.load(lineImagePath->text()))
		return true;
	else {
		image = QImage();
		return false;
	}
}

void RandomColorWidget::chooseImage()
{
	QString s = QFileDialog::getOpenFileName(this,tr("Select an image to use"),lineImagePath->text());
	if(!s.isNull())
		lineImagePath->setText(s);
}
