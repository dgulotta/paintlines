/***************************************************************************
 *   Copyright (C) 2008, 2014-2015 by Daniel Gulotta                       *
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

#include <QStringList>
#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <vector>
#include "symmetry.h"

struct ImageData;

class SymmetryCombo : public QComboBox
{
	Q_OBJECT
public:
	SymmetryCombo(bool random=true);
	SymmetryCombo(const std::vector<symgroup> &groups,bool random=true);
	symgroup group();
	static const QStringList symmetryStrings;
};

class SizeSpin : public QSpinBox
{
	Q_OBJECT
public:
	SizeSpin(int step=1);
protected:
	int valueFromText(const QString &text) const;
};

class ImageGeneratorWidget : public QWidget
{
	Q_OBJECT
public:
	static QSpinBox * newColorSpin();
signals:
	void newImage(const ImageData &data);
};

#endif
