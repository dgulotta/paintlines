/***************************************************************************
 *   Copyright (C) 2008 by Daniel Gulotta                                  *
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

#include "quasistripesform.h"
#include "quasiperiodic_paintstripeswidget.h"

QuasiStripesForm::QuasiStripesForm()
{
  menu = menuBar();
  menuFile = menu->addMenu(tr("&File"));
  actionSaveAs = menuFile->addAction(tr("&Save As"));
  actionExit = menuFile->addAction(tr("E&xit"));
  QHBoxLayout *mainLayout = new QHBoxLayout;
  QVBoxLayout *sideLayout = new QVBoxLayout;
  sideLayout->addWidget(new QLabel(tr("Size")));
  spinSize = new QSpinBox;
  spinSize->setMinimum(1);
  spinSize->setMaximum(65536);
  spinSize->setValue(256);
  sideLayout->addWidget(spinSize);
  sideLayout->addWidget(new QLabel(tr("Quasiperiod")));
  spinQuasiSize = new QSpinBox;
  spinQuasiSize->setMinimum(1);
  spinQuasiSize->setMaximum(256);
  spinQuasiSize->setValue(16);
  sideLayout->addWidget(spinQuasiSize);
  sideLayout->addWidget(new QLabel(tr("Alpha")));
  spinAlpha = new QDoubleSpinBox;
  spinAlpha->setMinimum(.01);
  spinAlpha->setMaximum(2.);
  spinAlpha->setValue(1.);
  sideLayout->addWidget(spinAlpha);
  buttonDraw = new QPushButton(tr("Draw"));
  sideLayout->addWidget(buttonDraw);
  sideLayout->addStretch(1);
  mainLayout->addLayout(sideLayout);
  stripes = new quasiperiodic_paintstripeswidget;
  mainLayout->addWidget(stripes);
  mainLayout->addStretch(1);
  QWidget *w = new QWidget;
  w->setLayout(mainLayout);
  QScrollArea *a = new QScrollArea;
  a->setWidgetResizable(true);
  a->setWidget(w);
  setCentralWidget(a);
  resize(800,600);
  connect(buttonDraw,SIGNAL(clicked()),this,SLOT(draw()));
  connect(actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAs()));
  connect(actionExit,SIGNAL(triggered()),this,SLOT(close()));
}

void QuasiStripesForm::draw()
{
  stripes->set_alpha(spinAlpha->value());
  stripes->draw(spinSize->value(),spinQuasiSize->value());
}

bool QuasiStripesForm::saveAs()
{
  QString s=QFileDialog::getSaveFileName();
  if(!s.isEmpty()) {
    stripes->save(s,"PNG");
    return true;
  }
  else {
    return false;
  }
}
