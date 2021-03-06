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

#ifndef _RANDOMIZEWIDGET_H
#define _RANDOMIZEWIDGET_H

#include <memory>
#include <QWidget>

class QPushButton;
class QSpinBox;
struct color_t;
struct ImageData;

template<typename T>
class symmetric_canvas;

class RandomizeWidget : public QWidget
{
	Q_OBJECT
public:
	RandomizeWidget(QWidget *parent=nullptr);
signals:
	void newImage(const ImageData &data);
public slots:
	void imageUpdated(const ImageData &data);
	void randomize();
private:
	std::shared_ptr<symmetric_canvas<color_t>> img;
	QPushButton *buttonRandomize;
	QSpinBox *spinXTiles;
	QSpinBox *spinYTiles;
};

#endif
