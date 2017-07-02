/***************************************************************************
 *   Copyright (C) 2008, 2014-2016 by Daniel Gulotta                       *
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

#ifndef _IMAGEGENERATORWIDGET_H
#define _IMAGEGENERATORWIDGET_H

#include <QFormLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>
#include "imagedata.hpp"

class ImageGeneratorWidget : public QWidget
{
	Q_OBJECT
public:
	ImageGeneratorWidget() { setLayout(new QFormLayout); }
	QFormLayout * layout() { return static_cast<QFormLayout *>(QWidget::layout()); }
	template<typename Fn>
	QPushButton * addGenerator(const QString &name, const Fn &f, const std::vector<layer> *l=nullptr, bool orig=true) {
		QPushButton *b = new QPushButton(name);
		layout()->addRow(b);
		connect(b,&QPushButton::clicked,[this,f,l,orig] {
			try {
				emit newImage(ImageData(f(),l,orig));
			} catch(const std::logic_error &e) {
				QMessageBox::information(this,"Paintlines",e.what());
			}
		});
		return b;
	}
signals:
	void newImage(const ImageData &data);
};

#endif
