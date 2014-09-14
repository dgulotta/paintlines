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
 
#ifndef _CAWIDGET_H
#define _CAWIDGET_H

#include <QMetaType>
#include "../imagegeneratorwidget.h"
#include "ca.h"

Q_DECLARE_METATYPE(const rule*);

class QDoubleSpinBox;
class QPushButton;

class CAWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	CAWidget();
protected slots:
	void draw();
	void cont();
protected:
	void addRule(const QString &s, const rule &r);
	QSpinBox *spinWidth;
	QSpinBox *spinHeight;
	QComboBox *comboRule;
	QSpinBox *spinLeft;
	QSpinBox *spinRight;
	QSpinBox *spinTop;
	QSpinBox *spinBottom;
	QSpinBox *spinTurns;
	QDoubleSpinBox *spinIntensity;
	QPushButton *buttonContinue;
	QScopedPointer<ca> ca_sim;
};

#endif
