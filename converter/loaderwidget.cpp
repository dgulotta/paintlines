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
#include "../imagedata.h"
#include "loaderwidget.h"

void LoaderWidget::openFile()
{
	QString s = QFileDialog::getOpenFileName();
	if(s.isEmpty()) return;
	open(QImage(s));
}

bool LoaderWidget::open(const QImage &img)
{
	if(img.isNull()) {
		QMessageBox::information(this,"converter",tr("Failed to load image."));
		return false;
	}
	if(img.height()!=img.width()) {
		QMessageBox::information(this,"converter",tr("The image must be square."));
		return false;
	}
	if(img.height()%2!=0) {
		QMessageBox::information(this,"converter",tr("The image size must be even."));
		return false;
	}
	image = symmetric_canvas<color_t>(img.height(),(symgroup)comboSymmetry->group());
	canvas<color_t> &base_image = image.unsafe_get_canvas();
	for(int j=0;j<img.height();j++)
		for(int i=0;i<img.width();i++) {
			QRgb pix = img.pixel(i,j);
			base_image(i,j)=color_t(qRed(pix),qGreen(pix),qBlue(pix));
		}
	emit newImage(ImageData(image));
	return true;
}

LoaderWidget::LoaderWidget()
{
	QFormLayout *layout = new QFormLayout;
	comboSymmetry = new SymmetryCombo(false);
	layout->addRow(tr("Symmetry"),comboSymmetry);
	QPushButton *buttonOpen = new QPushButton(tr("Open file"));
	layout->addRow(buttonOpen);
	buttonPaste = new QPushButton(tr("Paste from clipboard"));
	layout->addRow(buttonPaste);
	setLayout(layout);
	connect(QApplication::clipboard(),&QClipboard::dataChanged,this,&LoaderWidget::checkPasteEnabled);
	connect(buttonOpen,&QPushButton::clicked,this,&LoaderWidget::openFile);
	connect(buttonPaste,&QPushButton::clicked,this,&LoaderWidget::paste);
	connect(comboSymmetry,(void(QComboBox::*)(int))&QComboBox::currentIndexChanged,this,&LoaderWidget::symmetryChanged);
	checkPasteEnabled();
}

void LoaderWidget::symmetryChanged(int n)
{
	image.unsafe_set_symmetry_group((symgroup)n);
}

std::function<QImage()> LoaderWidget::mimeToImage(const QMimeData *mime)
{
	if(mime->hasImage())
		return [=] { return qvariant_cast<QImage>(mime->imageData()); };
	else if(mime->hasUrls()&&mime->urls().size()==1&&mime->urls()[0].isLocalFile())
		return [=] { return QImage(mime->urls()[0].toLocalFile()); };
	else
		return {};
}

void LoaderWidget::handleDragEnter(QDragEnterEvent *event)
{
	if(mimeToImage(event->mimeData()))
		event->acceptProposedAction();
}

bool LoaderWidget::handleDrop(QDropEvent *event)
{
	auto f = mimeToImage(event->mimeData());
	if(f) return open(f());
	else return false;
}

void LoaderWidget::checkPasteEnabled()
{
	buttonPaste->setEnabled(QApplication::clipboard()->mimeData()->hasImage());
}

void LoaderWidget::paste()
{
	open(QApplication::clipboard()->image());
}
