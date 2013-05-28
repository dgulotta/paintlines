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

#include <QtGui>

#include "cloudsform.h"
#include "paintcloudswidget.h"

void ColorButton::mousePressEvent(QMouseEvent *event)
{
  QColor col=QColorDialog::getColor(palette().color(QPalette::Window));
  if(col.isValid()) {
    setPalette(col);
  }
}

CloudsForm::CloudsForm()
{
}

CloudsForm::~CloudsForm()
{
}

painterwidget * CloudsForm::createPainterWidget() {
	clouds = new paintcloudswidget;
	return clouds;
}

void CloudsForm::addWidgets(QBoxLayout *sideLayout) {
  QGridLayout *colorLayout = new QGridLayout;
  colorLayout->addWidget(new QLabel(tr("Color 1")),0,0);
  color1 = new ColorButton(qRgb(255,255,0));
  colorLayout->addWidget(color1,0,1);
  colorLayout->addWidget(new QLabel(tr("Color 2")),1,0);
  color2 = new ColorButton(qRgb(255,0,255));
  colorLayout->addWidget(color2,1,1);
  colorLayout->addWidget(new QLabel(tr("Color 3")),2,0);
  color3 = new ColorButton(qRgb(0,255,255));
  colorLayout->addWidget(color3,2,1);
  sideLayout->addLayout(colorLayout);
}

void CloudsForm::draw(int sz, int sym_index)
{
	symgroup sg=symgroup(comboSymmetry->currentIndex()+int(SYM_CM_O));
	QColor c=color1->palette().color(QPalette::Window);
    clouds->set_color1(c.red(),c.green(),c.blue());
    c=color2->palette().color(QPalette::Window);
    clouds->set_color2(c.red(),c.green(),c.blue());
    c=color3->palette().color(QPalette::Window);
    clouds->set_color3(c.red(),c.green(),c.blue());
    buttonRandomize->setEnabled(true);
    buttonRestore->setEnabled(false);
    clouds->draw(spinSize->value(),sg);
}
