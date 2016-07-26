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

#include <QtWidgets>
#include "cloudswidget.h"
#include "paintclouds.h"
#include "hyperboliccloudswidget.h"
#include "../hyperbolic_symmetry_chooser.h"
#include "../imagedata.h"

HyperbolicCloudsWidget::HyperbolicCloudsWidget()
{
	QFormLayout *layout = new QFormLayout;
	comboModel = new QComboBox;
	comboModel->addItem(tr("Poincare"));
	comboModel->addItem(tr("Klein"));
	layout->addRow(tr("Model"),comboModel);
	chooser = new HyperbolicSymmetryChooser;
	chooser->addDefaultItems();
	layout->addRow(chooser);
	spinSize=new QSpinBox;
	spinSize->setMinimum(1);
	spinSize->setMaximum(65536);
	spinSize->setValue(256);
	layout->addRow(tr("Size"),spinSize);
	comboRandom = CloudsWidget::newComboRandom();
	layout->addRow(tr("Distribution"),comboRandom);
	color1 = new ColorButton(qRgb(255,255,0));
	layout->addRow(tr("Color 1"),color1);
	color2 = new ColorButton(qRgb(255,0,255));
	layout->addRow(tr("Color 2"),color2);
	color3 = new ColorButton(qRgb(0,255,255));
	layout->addRow(tr("Color 3"),color3);
	QPushButton *buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	setLayout(layout);
	connect(buttonDraw,&QPushButton::clicked,this,&HyperbolicCloudsWidget::draw);
}

static clouds_randfunc randfuncs[] = {
	&clouds_rand_cauchy,
	&clouds_rand_normal,
	&clouds_rand_exp_cos,
	&clouds_rand_sechsquare
};

void HyperbolicCloudsWidget::draw()
{
	fundamental_domain fd;
	try {
		fd=chooser->domain();
	} catch(const std::domain_error &) {
		QMessageBox::information(this,"Hyperbolic Paintlines",tr("The chosen group is not hyperbolic.  Try increasing the parameters."));
		return;
	}
	canvas<color_t> img=paint_hyperbolic_clouds(spinSize->value(),fd,
		static_cast<projtype>(comboModel->currentIndex()),color1->color(),
		color2->color(),color3->color(),randfuncs[comboRandom->currentIndex()]);
	emit newImage(ImageData(std::move(img)));
}
