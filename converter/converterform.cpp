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

#include <QtGui>
#include "converterform.h"
#include "../hyperbolic_painter.h"
#include "../randomizewidget.h"

void ConverterForm::open()
{
	QString s = QFileDialog::getOpenFileName();
	if(s.isEmpty()) return;
	open(s);
}

void ConverterForm::open(const QString &s)
{
	open(QImage(s));
}

void ConverterForm::open(const QImage &img)
{
	if(img.isNull()) {
		QMessageBox::information(this,"converter",tr("Failed to load image."));
		return;
	}
	if(img.height()!=img.width()) {
		QMessageBox::information(this,"converter",tr("The image must be square."));
		return;
	}
	if(img.height()%2!=0) {
		QMessageBox::information(this,"converter",tr("The image size must be even."));
		return;
	}
	randomizeWidget->setEnabled(true);
	buttonDraw->setEnabled(true);
	image = symmetric_canvas<color_t>(img.height(),(symgroup)comboSymmetry->currentIndex());
	canvas<color_t> &base_image = image.unsafe_get_canvas();
	for(int j=0;j<img.height();j++)
		for(int i=0;i<img.width();i++) {
			QRgb pix = img.pixel(i,j);
			base_image(i,j)=color_t(qRed(pix),qGreen(pix),qBlue(pix));
		}
	emit newImage(QPixmap::fromImage(img),true);
	emit newCanvas(&image);
}

void ConverterForm::init()
{
	setAcceptDrops(true);
	QFormLayout *layout = new QFormLayout;
	comboSymmetry = newSymmetryCombo();
	layout->addRow(tr("Symmetry"),comboSymmetry);
	randomizeWidget = new RandomizeWidget;
	randomizeWidget->setEnabled(false);
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
	buttonDraw = new QPushButton(tr("Make hyperbolic"));
	buttonDraw->setEnabled(false);
	layout->addRow(buttonDraw);
	buttonRestore = new RestoreButton;
	layout->addRow(buttonRestore);
	checkTiled = newTileCheck();
	layout->addRow(checkTiled);
	saver = new ImageSaver(this);
	sideLayout = layout;
	menuFile->addAction(tr("&Open"),this,SLOT(open()));
	actionPaste = menuFile->addAction(tr("&Paste from clipboard"),this,SLOT(paste()));
	connect(QApplication::clipboard(),SIGNAL(dataChanged()),this,SLOT(checkPasteEnabled()));
	checkPasteEnabled();
	connect(buttonRestore,SIGNAL(clicked()),this,SLOT(updateImage()));
	connect(this,SIGNAL(newImage(QPixmap,bool)),buttonRestore,SLOT(disable()));
	connect(this,SIGNAL(newHyperbolicImage(QPixmap)),buttonRestore,SLOT(enable()));
	connect(this,SIGNAL(newHyperbolicImage(QPixmap)),labelImage,SLOT(setPixmapNonTileable(const QPixmap &)));
	connect(this,SIGNAL(newHyperbolicImage(QPixmap)),saver,SLOT(newImage(const QPixmap &)));
	connect(this,SIGNAL(newCanvas(const symmetric_canvas<color_t> *)),randomizeWidget,SLOT(imageUpdated(const symmetric_canvas<color_t> *)));
	connect(randomizeWidget,SIGNAL(newImage(QPixmap)),buttonRestore,SLOT(enable()));
	connect(randomizeWidget,SIGNAL(newImage(QPixmap)),labelImage,SLOT(setPixmapTileable(const QPixmap &)));
	connect(randomizeWidget,SIGNAL(newImage(QPixmap)),saver,SLOT(newImage(const QPixmap &)));
	connect(comboSymmetry,SIGNAL(currentIndexChanged(int)),this,SLOT(symmetryChanged(int)));
}

void ConverterForm::draw()
{
	emit newHyperbolicImage(makePixmap(make_hyperbolic(image,(projtype)comboModel->currentIndex(),spinSize->value())));
}

void ConverterForm::symmetryChanged(int n)
{
	image.unsafe_set_symmetry_group((symgroup)n);
}

void ConverterForm::updateImage()
{
	emit newImage(makePixmap(image.as_canvas()),true);
}

void ConverterForm::dragEnterEvent(QDragEnterEvent *event)
{
	const QMimeData *mime = event->mimeData();
	if(mime->hasImage())
		event->acceptProposedAction();
	else if(mime->hasUrls()&&mime->urls()[0].isLocalFile())
		event->acceptProposedAction();
}

void ConverterForm::dropEvent(QDropEvent *event)
{
	const QMimeData *mime = event->mimeData();
	if(mime->hasImage())
		open(qvariant_cast<QImage>(mime->imageData()));
	else if(mime->hasUrls()&&mime->urls()[0].isLocalFile())
		open(mime->urls()[0].toLocalFile());
}

void ConverterForm::checkPasteEnabled()
{
	actionPaste->setEnabled(QApplication::clipboard()->mimeData()->hasImage());
}

void ConverterForm::paste()
{
	open(QApplication::clipboard()->image());
}
