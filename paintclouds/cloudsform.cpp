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
  menu = new QMenuBar;
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
  buttonDraw = new QPushButton(tr("Draw"));
  sideLayout->addWidget(buttonDraw);
  sideLayout->addStretch(1);
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
  mainLayout->addLayout(sideLayout);
  clouds = new paintcloudswidget;
  //mainLayout->addWidget(clouds,1,Qt::AlignLeft|Qt::AlignTop);
  mainLayout->addWidget(clouds,1);
  QWidget *w = new QWidget;
  w->setLayout(mainLayout);
  setCentralWidget(w);
  connect(buttonDraw,SIGNAL(clicked()),this,SLOT(draw()));
  connect(buttonRandomize,SIGNAL(clicked()),this,SLOT(randomize()));
  connect(buttonRestore,SIGNAL(clicked()),this,SLOT(restore()));
  connect(actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAs()));
  connect(actionExit,SIGNAL(triggered()),this,SLOT(close()));
}

void CloudsForm::draw()
{
  if(spinSize->value()&1) {
    QMessageBox::information(this,"Paintclouds","The size must be even.");
  }
  else {
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
}

void CloudsForm::randomize()
{
  clouds->randomize(spinXTiles->value(),spinYTiles->value());
  buttonRestore->setEnabled(true);
}

void CloudsForm::restore()
{
  clouds->restore();
  buttonRestore->setEnabled(false);
}

bool CloudsForm::saveAs()
{
  QString s=QFileDialog::getSaveFileName();
  if(!s.isEmpty()) {
    clouds->save(s,"PNG");
    return true;
  }
  else {
    return false;
  }
}
