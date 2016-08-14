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
 
#ifndef _CONVERTERWIDGET_H
#define _CONVERTERWIDGET_H

#include "imagedata.h"
#include "imagegeneratorwidget.h"
#include "inputwidgets.h"
#include "mainform.h"

enum class projtype;
class QSpinBox;
class QPushButton;
class RandomizeWidget;

class ConverterWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	ConverterWidget();
	void makeConnections(MainForm *f);
public slots:
	void imageUpdated(const ImageData &data);
	void restore();
	void makeHyperbolic();
	void hexagonalStretch();
private:
	QSpinBox *spinSize;
	EnumComboAdapter<projtype> comboModel;
	QPushButton *buttonHyperbolic;
	QPushButton *buttonRestore;
	QPushButton *buttonHexStretch;
	RandomizeWidget *randomizeWidget;
	ImageData restoreData;
};

#endif
