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

#include "basicform.h"
#include "painterwidget.h"

BasicForm::BasicForm()
{
	QMetaObject::invokeMethod(this,"init",Qt::QueuedConnection);
}

void BasicForm::init()
{
  menu = menuBar();
  menuFile = menu->addMenu(tr("&File"));
  actionSaveAs = menuFile->addAction(tr("&Save As"));
  actionExit = menuFile->addAction(tr("E&xit"));
  QHBoxLayout *mainLayout = new QHBoxLayout;
  QVBoxLayout *sideLayout = new QVBoxLayout;
  sideLayout->addWidget(new QLabel(tr("Symmetry Group")));
  comboSymmetry = new QComboBox;
  comboSymmetry->addItem(tr("CM (*x)"));
  comboSymmetry->addItem(tr("CMM (2*22)"));
  comboSymmetry->addItem(tr("P1 (o)"));
  comboSymmetry->addItem(tr("P2 (2222)"));
  comboSymmetry->addItem(tr("P3 (333)"));
  comboSymmetry->addItem(tr("P31M (3*3)"));
  comboSymmetry->addItem(tr("P3M1 (*333)"));
  comboSymmetry->addItem(tr("P4 (442)"));
  comboSymmetry->addItem(tr("P4G (4*2)"));
  comboSymmetry->addItem(tr("P4M (*442)"));
  comboSymmetry->addItem(tr("P6 (632)"));
  comboSymmetry->addItem(tr("P6M (*632)"));
  comboSymmetry->addItem(tr("PG (xx)"));
  comboSymmetry->addItem(tr("PGG (22x)"));
  comboSymmetry->addItem(tr("PM (**)"));
  comboSymmetry->addItem(tr("PMG (22*)"));
  comboSymmetry->addItem(tr("PMM (*2222)"));
  sideLayout->addWidget(comboSymmetry);
  sideLayout->addWidget(new QLabel(tr("Size")));
  spinSize = new QSpinBox;
  spinSize->setMinimum(2);
  spinSize->setMaximum(65536);
  spinSize->setSingleStep(2);
  spinSize->setValue(256);
  sideLayout->addWidget(spinSize);
  addWidgets(sideLayout);
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
  painter = createPainterWidget();
  mainLayout->addWidget(painter);
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

BasicForm::~BasicForm()
{
}

void BasicForm::draw()
{
  if(spinSize->value()&1) {
    QMessageBox::information(this,QCoreApplication::applicationName(),"The size must be even.");
  }
  else {
  	draw(spinSize->value(),comboSymmetry->currentIndex());
    buttonRandomize->setEnabled(true);
    buttonRestore->setEnabled(false);
  }
}

void BasicForm::randomize()
{
  painter->randomize(spinXTiles->value(),spinYTiles->value());
  buttonRestore->setEnabled(true);
}

void BasicForm::restore()
{
  painter->restore();
  buttonRestore->setEnabled(false);
}

bool BasicForm::saveAs()
{
  QString s=QFileDialog::getSaveFileName();
  if(!s.isEmpty()) {
    return painter->save(s);
  }
  else {
    return false;
  }
}
