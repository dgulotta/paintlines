/***************************************************************************
 *   Copyright (C) 2013-2014 by Daniel Gulotta                             *
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
#include "converterform.h"
#include "converters.h"
#include "../hyperbolic_painter.h"
#include "../randomizewidget.h"

void ConverterForm::open()
{
	QString s = QFileDialog::getOpenFileName();
	if(s.isEmpty()) return;
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
	updateImage();
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
	buttonHexStretch = new QPushButton("Hexagonal Stretch");
	layout->addRow(buttonHexStretch);
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
	connect(buttonHexStretch,SIGNAL(clicked()),this,SLOT(hexStretch()));
	connect(this,SIGNAL(newImage(const ImageData &)),buttonRestore,SLOT(newImage(const ImageData &)));
	connect(this,SIGNAL(newImage(const ImageData &)),randomizeWidget,SLOT(imageUpdated(const ImageData &)));
	connect(randomizeWidget,SIGNAL(newImage(const ImageData &)),this,SIGNAL(newImage(const ImageData &)));
	connect(comboSymmetry,SIGNAL(currentIndexChanged(int)),this,SLOT(symmetryChanged(int)));
}

void ConverterForm::draw()
{
	emit newImage(ImageData(make_hyperbolic(image,(projtype)comboModel->currentIndex(),spinSize->value())));
}

void ConverterForm::hexStretch()
{
	emit newImage(ImageData(hexagonal_stretch(image).as_canvas(),true));	
}

void ConverterForm::symmetryChanged(int n)
{
	image.unsafe_set_symmetry_group((symgroup)n);
}

void ConverterForm::updateImage()
{
	emit newImage(ImageData(image.as_canvas(),image));
}

std::function<QImage()> ConverterForm::mimeToImage(const QMimeData *mime)
{
	if(mime->hasImage())
		return [=] { return qvariant_cast<QImage>(mime->imageData()); };
	else if(mime->hasUrls()&&mime->urls().size()==1&&mime->urls()[0].isLocalFile())
		return [=] { return QImage(mime->urls()[0].toLocalFile()); };
	else
		return {};
}

void ConverterForm::dragEnterEvent(QDragEnterEvent *event)
{
	if(mimeToImage(event->mimeData()))
		event->acceptProposedAction();
}

void ConverterForm::dropEvent(QDropEvent *event)
{
	auto f = mimeToImage(event->mimeData());
	if(f) open(f());
}

void ConverterForm::checkPasteEnabled()
{
	actionPaste->setEnabled(QApplication::clipboard()->mimeData()->hasImage());
}

void ConverterForm::paste()
{
	open(QApplication::clipboard()->image());
}
