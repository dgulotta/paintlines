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
#include <memory>

#include "hyperboliclinesform.h"
#include "hyperbolic_paintlineswidget.h"

using std::auto_ptr;

static const char *angle1str = "Angle 1";
static const char *angle2str = "Angle 2";
static const char *angle3str = "Angle 3";
static const char *anglesum = "Sum of Angles";
static const char *anglemirror = "Mirror Angle";
static const char *anglerot = "Rotation angle";
static const char *angleother = "Other angles";

static const char *anglestrings[][3] = {
  { anglesum, NULL, NULL },
  { anglemirror, angleother, NULL },
  { anglesum, NULL, NULL },
  { angle1str, angle2str, NULL },
  { anglerot, angleother, NULL },
  { angle1str, angle2str, angle3str },
  { anglesum, NULL, NULL },
  { anglesum, NULL, NULL },
  { angle1str, angle2str, angle3str },
  { angle1str, angle2str, angle3str }
};

static const char *twopiover = "2 * Pi /";
static const char *piover = "Pi /";
static const char *halfpiover = "Pi / 2 *";

static const char *pistrings[][3] = {
  { piover, NULL, NULL },
  { piover, halfpiover, NULL },
  { twopiover, NULL, NULL },
  { piover, piover, NULL },
  { twopiover, halfpiover, NULL },
  { piover, piover, piover },
  { twopiover, NULL, NULL },
  { piover, NULL, NULL },
  { piover, piover, piover },
  { piover, piover, piover }
};

static const int minangles[][3] = {
  { 2, 0, 0 },
  { 2, 2, 0 },
  { 3, 0, 0 },
  { 3, 3, 0 },
  { 3, 2, 0 },
  { 2, 2, 2 },
  { 3, 0, 0 },
  { 2, 0, 0 },
  { 2, 2, 2 },
  { 2, 2, 2 },
};

static const int initangles[][3] = {
  {2,0,0},
  {3,2,0},
  {3,0,0},
  {5,4,0},
  {5,2,0},
  {5,4,2},
  {3,0,0},
  {2,0,0},
  {3,2,2},
  {3,2,2}
};

HyperbolicLinesForm::HyperbolicLinesForm()
  : oldsymm(0)
{
  int i,j;
  menu = menuBar();
  menuFile = menu->addMenu(tr("&File"));
  actionSaveAs = menuFile->addAction(tr("&Save As"));
  actionExit = menuFile->addAction(tr("E&xit"));
  QHBoxLayout *mainLayout = new QHBoxLayout;
  QVBoxLayout *sideLayout = new QVBoxLayout;
  sideLayout->addWidget(new QLabel(tr("Model")));
  comboModel = new QComboBox;
  comboModel->addItem(tr("Poincare"));
  comboModel->addItem(tr("Klein"));
  sideLayout->addWidget(comboModel);
  sideLayout->addWidget(new QLabel(tr("Symmetry type")));
  comboSymmetry = new QComboBox;
  comboSymmetry->addItem(tr("CM-like (*ax)"));
  comboSymmetry->addItem(tr("CMM-like (2*ab)"));
  comboSymmetry->addItem(tr("P2-like (a222)"));
  comboSymmetry->addItem(tr("P4-like (ab2)"));
  comboSymmetry->addItem(tr("P4G-like (a*b)"));
  comboSymmetry->addItem(tr("P4M-like (*abc)"));
  comboSymmetry->addItem(tr("PGG-like (a2x)"));
  comboSymmetry->addItem(tr("PMG-like (22*a)"));
  comboSymmetry->addItem(tr("(a*bc)"));
  comboSymmetry->addItem(tr("(*abc2)"));
  sideLayout->addWidget(comboSymmetry);
  QGridLayout *angleLayout = new QGridLayout;
  for(j=0;j<10;j++)
    for(i=0;i<3;i++)
      angles[j][i]=initangles[j][i];
  for(i=0;i<3;i++) {
    labelAngle[i]=new QLabel (anglestrings[0][i]);
    labelPi[i]=new QLabel(pistrings[0][i]);
    spinAngle[i]=new QSpinBox;
    spinAngle[i]->setValue(angles[0][i]);
    spinAngle[i]->setEnabled(pistrings[0][i]!=NULL);
    spinAngle[i]->setMinimum(minangles[0][i]);
  }
  angleLayout->addWidget(labelAngle[0],0,0,1,2);
  angleLayout->addWidget(labelAngle[1],2,0,1,2);
  angleLayout->addWidget(labelAngle[2],4,0,1,2);
  angleLayout->addWidget(labelPi[0],1,0);
  angleLayout->addWidget(labelPi[1],3,0);
  angleLayout->addWidget(labelPi[2],5,0);
  angleLayout->addWidget(spinAngle[0],1,1);
  angleLayout->addWidget(spinAngle[1],3,1);
  angleLayout->addWidget(spinAngle[2],5,1);
  sideLayout->addLayout(angleLayout);
  sideLayout->addWidget(new QLabel("Subset"));
  comboSubset=new QComboBox;
  comboSubset->addItem(tr("All"));
  comboSubset->addItem(tr("Det > 0"));
  comboSubset->addItem(tr("Random"));
  sideLayout->addWidget(comboSubset);
  sideLayout->addWidget(new QLabel(tr("Size")));
  spinSize=new QSpinBox;
  spinSize->setMinimum(1);
  spinSize->setMaximum(65536);
  spinSize->setValue(256);
  sideLayout->addWidget(spinSize);
  sideLayout->addWidget(new QLabel(tr("Colors")));
  spinColors=new QSpinBox;
  spinColors->setMinimum(1);
  spinColors->setMaximum(65536);
  spinColors->setValue(25);
  sideLayout->addWidget(spinColors);
  buttonDraw=new QPushButton(tr("Draw"));
  sideLayout->addWidget(buttonDraw);
  sideLayout->addStretch(1);
  mainLayout->addLayout(sideLayout);
  lines = new hyperbolic_paintlineswidget;
  mainLayout->addWidget(lines,1);
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
  connect(comboSymmetry,SIGNAL(currentIndexChanged(int)),this,SLOT(symmetryChanged(int)));
}

void HyperbolicLinesForm::draw()
{
  hyperbolic_symmetry_group *sg;
  flip_type ft = (flip_type)comboSubset->currentIndex();
  switch(comboSymmetry->currentIndex()) {
  case 0:
    sg=hyperbolic_symmetry_group::group_sax(spinAngle[0]->value(),ft);
    break;
  case 1:
    sg=hyperbolic_symmetry_group::group_2sab(spinAngle[0]->value(),spinAngle[1]->value(),ft);
    break;
  case 2:
    sg=hyperbolic_symmetry_group::group_a222(spinAngle[0]->value(),ft);
    break;
  case 3:
    sg=hyperbolic_symmetry_group::group_ab2(spinAngle[0]->value(),spinAngle[1]->value(),ft);
    break;
  case 4:
    sg=hyperbolic_symmetry_group::group_asb(spinAngle[0]->value(),spinAngle[1]->value(),ft);
    break;
  case 5:
    sg=hyperbolic_symmetry_group::group_sabc(spinAngle[0]->value(),spinAngle[1]->value(),spinAngle[2]->value(),ft);
    break;
  case 6:
    sg=hyperbolic_symmetry_group::group_a2x(spinAngle[0]->value(),ft);
    break;
  case 7:
    sg=hyperbolic_symmetry_group::group_22sa(spinAngle[0]->value(),ft);
    break;
  case 8:
	sg=hyperbolic_symmetry_group::group_asbc(spinAngle[0]->value(),spinAngle[1]->value(),spinAngle[2]->value(),ft);
	break;
   case 9:
	sg=hyperbolic_symmetry_group::group_sabcd(spinAngle[0]->value(),spinAngle[1]->value(),spinAngle[2]->value(),2,ft);
	break; }
  if(sg) {
    lines->draw(spinSize->value(),spinColors->value(),*sg,(projtype)comboModel->currentIndex());
	delete sg;
  }
  else QMessageBox::information(this,"Hyperbolic Paintlines",tr("The chosen group is not hyperbolic.  Try increasing the parameters."));
}

void HyperbolicLinesForm::symmetryChanged(int n)
{
  int i;
  for(i=0;i<3;i++) {
    angles[oldsymm][i]=spinAngle[i]->value();
    spinAngle[i]->setEnabled(pistrings[n][i]!=NULL);
    spinAngle[i]->setMinimum(minangles[n][i]);
    spinAngle[i]->setValue(angles[n][i]);
    labelAngle[i]->setText(anglestrings[n][i]);
    labelPi[i]->setText(pistrings[n][i]);
  }
  oldsymm=n;
}

bool HyperbolicLinesForm::saveAs()
{
  QString s=QFileDialog::getSaveFileName();
  if(!s.isEmpty()) {
    lines->save(s,"PNG");
    return true;
  }
  else {
    return false;
  }
}
