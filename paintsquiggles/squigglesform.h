/***************************************************************************
 *   Copyright (C) 2008, 2013 by Daniel Gulotta                            *
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

#ifndef _SQUIGGLESFORM_H
#define _SQUIGGLESFORM_H

#include "../basicform.h"

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class paintsquiggleswidget;

class SquigglesForm : public BasicForm
{
  Q_OBJECT
protected:
  virtual void addWidgets(QBoxLayout *sideLayout);
  virtual painterwidget * createPainterWidget();
  virtual void draw(int sz, int sym_index);
  QSpinBox *spinColors;
  QDoubleSpinBox *spinAlpha;
  QDoubleSpinBox *spinExponent;
  QDoubleSpinBox *spinThickness;
  QDoubleSpinBox *spinSharpness;
  paintsquiggleswidget *squiggles;
};

#endif
