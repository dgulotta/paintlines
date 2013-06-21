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

#ifndef _CLOUDSFORM_H
#define _CLOUDSFORM_H

#include <QWidget>
#include <QColor>
#include <QPalette>

#include "../basicform.h"

class QMouseEvent;
class paintcloudswidget;

class ColorButton : public QWidget
{
  Q_OBJECT
public:
  ColorButton(QWidget *parent=0) : QWidget(parent) {
    setAutoFillBackground(true);
  }
  ColorButton(QColor col, QWidget *parent=0) : QWidget(parent) {
    setPalette(QPalette(col));
    setAutoFillBackground(true);
  }
protected:
  virtual void mousePressEvent(QMouseEvent *event);
};

class CloudsForm : public BasicForm
{
  Q_OBJECT
public:
  CloudsForm();
  virtual ~CloudsForm();
private:
  virtual void addWidgets(QBoxLayout *sideLayout);
  virtual painterwidget * createPainterWidget();
  virtual void draw(int sz, int sym_index);
  ColorButton *color1;
  ColorButton *color2;
  ColorButton *color3;
  QComboBox *comboRandom;
  paintcloudswidget *clouds;
};

#endif
