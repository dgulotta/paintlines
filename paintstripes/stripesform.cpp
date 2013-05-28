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

#include "stripesform.h"
#include "paintstripeswidget.h"

StripesForm::StripesForm()
{
  menu = menuBar();
  menuFile = menu->addMenu(tr("&File"));
  actionSaveAs = menuFile->addAction(tr("&Save As"));
  actionExit = menuFile->addAction(tr("E&xit"));
  QHBoxLayout *mainLayout = new QHBoxLayout;
  QVBoxLayout *sideLayout = new QVBoxLayout;
  sideLayout->addWidget(new QLabel(tr("Symmetry Group")));
  comboSymmetry = new QComboBox;
  comboSymmetry->addItem(tr("CM"));
  comboSymmetry->addItem(tr("CMM"));
  comboSymmetry->addItem(tr("P1"));
  comboSymmetry->addItem(tr("P2"));
  comboSymmetry->addItem(tr("P3"));
  comboSymmetry->addItem(tr("P31M"));
  comboSymmetry->addItem(tr("P3M1"));
  comboSymmetry->addItem(tr("P4"));
  comboSymmetry->addItem(tr("P4G"));
  comboSymmetry->addItem(tr("P4M"));
  comboSymmetry->addItem(tr("P6"));
  comboSymmetry->addItem(tr("P6M"));
  comboSymmetry->addItem(tr("PG"));
  comboSymmetry->addItem(tr("PGG"));
  comboSymmetry->addItem(tr("PM"));
  comboSymmetry->addItem(tr("PMG"));
  comboSymmetry->addItem(tr("PMM"));
  sideLayout->addWidget(comboSymmetry);
  sideLayout->addWidget(new QLabel(tr("Size")));
  spinSize = new QSpinBox;
  spinSize->setMinimum(2);
  spinSize->setMaximum(65536);
  spinSize->setSingleStep(2);
  spinSize->setValue(256);
  sideLayout->addWidget(spinSize);
  sideLayout->addWidget(new QLabel(tr("Alpha")));
  spinAlpha = new QDoubleSpinBox;
  spinAlpha->setMinimum(.01);
  spinAlpha->setMaximum(2.);
  spinAlpha->setValue(1.);
  sideLayout->addWidget(spinAlpha);
  buttonDraw = new QPushButton(tr("Draw"));
  sideLayout->addWidget(buttonDraw);
  sideLayout->addWidget(new QLabel(tr("Tiles")));
  QHBoxLayout *tilesLayout = new QHBoxLayout;
  tilesLayout->addWidget(new QLabel(tr("X")));
  spinXTiles = new QSpinBox;
  spinXTiles->setMinimum(1);
  spinXTiles->setValue(2);
  tilesLayout->addWidget(spinXTiles);
  tilesLayout->addWidget(new QLabel(tr("Y")));
  spinYTiles = new QSpinBox;
  spinYTiles->setMinimum(1);
  spinYTiles->setValue(2);
  tilesLayout->addWidget(spinYTiles);
  sideLayout->addLayout(tilesLayout);
  buttonRandomize = new QPushButton(tr("Randomize"));
  buttonRandomize->setEnabled(false);
  sideLayout->addWidget(buttonRandomize);
  buttonRestore = new QPushButton(tr("Restore Original"));
  buttonRestore->setEnabled(false);
  sideLayout->addWidget(buttonRestore);
  sideLayout->addStretch(1);
  mainLayout->addLayout(sideLayout);
  stripes = new paintstripeswidget;
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
  connect(buttonRandomize,SIGNAL(clicked()),this,SLOT(randomize()));
  connect(buttonRestore,SIGNAL(clicked()),this,SLOT(restore()));
  connect(actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAs()));
  connect(actionExit,SIGNAL(triggered()),this,SLOT(close()));
}

void StripesForm::draw()
{
  if(spinSize->value()&1) {
    QMessageBox::information(this,"Paintstripes","The size must be even.");
  }
  else {
    symgroup sg=symgroup(comboSymmetry->currentIndex());
    stripes->set_alpha(spinAlpha->value());
    buttonRandomize->setEnabled(true);
    buttonRestore->setEnabled(false);
    stripes->draw(spinSize->value(),sg);
  }
}

void StripesForm::randomize()
{
  stripes->randomize(spinXTiles->value(),spinYTiles->value());
  buttonRestore->setEnabled(true);
}

void StripesForm::restore()
{
  stripes->restore();
  buttonRestore->setEnabled(false);
}

bool StripesForm::saveAs()
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
