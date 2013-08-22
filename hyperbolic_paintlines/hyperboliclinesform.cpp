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
#include "hyperbolic_paintlines.h"

static const char *angle1str = "Angle 1";
static const char *angle2str = "Angle 2";
static const char *angle3str = "Angle 3";
static const char *anglesum = "Sum of Angles";
static const char *anglemirror = "Mirror Angle";
static const char *anglerot = "Rotation angle";
static const char *angleother = "Other angles";

static const char *anglestrings[][3] = {
  { anglesum, nullptr, nullptr },
  { anglemirror, angleother, nullptr },
  { anglesum, nullptr, nullptr },
  { angle1str, angle2str, nullptr },
  { anglerot, angleother, nullptr },
  { angle1str, angle2str, angle3str },
  { anglesum, nullptr, nullptr },
  { anglesum, nullptr, nullptr },
  { anglerot, angle2str, angle3str },
  { angle1str, angle2str, angle3str },
  { anglerot, anglesum, nullptr },
  { anglesum, nullptr, nullptr },
  { anglesum, nullptr, nullptr },
  { angle1str, angle2str, angle3str },
  { anglesum, nullptr, nullptr },
};

static const char *twopiover = "2 * Pi /";
static const char *piover = "Pi /";
static const char *halfpiover = "Pi / 2 *";

static const char *pistrings[][3] = {
  { piover, nullptr, nullptr },
  { piover, halfpiover, nullptr },
  { twopiover, nullptr, nullptr },
  { piover, piover, nullptr },
  { twopiover, halfpiover, nullptr },
  { piover, piover, piover },
  { twopiover, nullptr, nullptr },
  { piover, nullptr, nullptr },
  { twopiover, piover, piover },
  { piover, piover, piover },
  { twopiover, piover, nullptr },
  { piover, nullptr, nullptr },
  { twopiover, nullptr, nullptr },
  { twopiover, twopiover, twopiover },
  { twopiover, nullptr, nullptr },
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
  { 2, 1, 0 },
  { 2, 0, 0 },
  { 2, 0, 0 },
  { 2, 2, 2 },
  { 2, 0, 0 },
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
  {3,2,2},
  {3,1,0},
  {2,0,0},
  {2,0,0},
  {5,4,2},
  {2,0,0},
};

void HyperbolicLinesForm::init()
{
	oldsymm=0;
	int i,j;
	QFormLayout *layout = new QFormLayout;
	comboModel = new QComboBox;
	comboModel->addItem(tr("Poincare"));
	comboModel->addItem(tr("Klein"));
	layout->addRow(tr("Model"),comboModel);
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
	comboSymmetry->addItem(tr("(a2*b)"));
	comboSymmetry->addItem(tr("(**a)"));
	comboSymmetry->addItem(tr("(ao)"));
	comboSymmetry->addItem(tr("(abc)"));
	comboSymmetry->addItem(tr("(axx)"));
	layout->addRow(tr("Symmetry"),comboSymmetry);
	QGridLayout *angleLayout = new QGridLayout;
	for(j=0;j<15;j++)
		for(i=0;i<3;i++)
			angles[j][i]=initangles[j][i];
	for(i=0;i<3;i++) {
		labelAngle[i]=new QLabel (anglestrings[0][i]);
		labelPi[i]=new QLabel(pistrings[0][i]);
		spinAngle[i]=new QSpinBox;
		spinAngle[i]->setValue(angles[0][i]);
		spinAngle[i]->setEnabled(pistrings[0][i]!=nullptr);
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
	layout->addRow(angleLayout);
	comboSubset=new QComboBox;
	comboSubset->addItem(tr("All"));
	comboSubset->addItem(tr("Det > 0"));
	comboSubset->addItem(tr("Random"));
	layout->addRow(tr("Subset"),comboSubset);
	spinSize=new QSpinBox;
	spinSize->setMinimum(1);
	spinSize->setMaximum(65536);
	spinSize->setValue(256);
	layout->addRow(tr("Size"),spinSize);
	spinColors=newColorSpin();
	layout->addRow(tr("Colors"),spinColors);
	spinThickness = new QDoubleSpinBox;
	spinThickness->setValue(1);
	layout->addRow(tr("Thickness"),spinThickness);
	spinSharpness = new QDoubleSpinBox;
	spinSharpness->setValue(1.5);
	layout->addRow(tr("Sharpness"),spinSharpness);
	buttonDraw = new QPushButton(tr("Draw"));
	layout->addRow(buttonDraw);
	sideLayout=layout;
	lines = new hyperbolic_paintlines;
#ifdef MULTIPAGE
	saver = new LayeredImageSaver(this);
	connect(this,SIGNAL(newLayeredImage(const std::vector<layer> *)),saver,SLOT(newLayeredImage(const std::vector<layer> *)));
#else
	saver = new ImageSaver(this);
#endif
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
	break;
  case 10:
	sg=hyperbolic_symmetry_group::group_a2sb(spinAngle[0]->value(),spinAngle[1]->value(),ft);
	break;
  case 11:
  	sg=hyperbolic_symmetry_group::group_ssa(spinAngle[0]->value(),ft);
	break;
  case 12:
  	sg=hyperbolic_symmetry_group::group_ao(spinAngle[0]->value(),ft);
	break;
  case 13:
    sg=hyperbolic_symmetry_group::group_abc(spinAngle[0]->value(),spinAngle[1]->value(),spinAngle[2]->value(),ft);
	break;
  case 14:
  	sg=hyperbolic_symmetry_group::group_axx(spinAngle[0]->value());
  }
  if(sg) {
  	double t = spinThickness->value();
  	lines->set_thickness(5*t,5*t*t,spinSharpness->value());
	lines->set_projtype((projtype)comboModel->currentIndex());
	lines->set_ncolors(spinColors->value());
    lines->paint(spinSize->value(),*sg);
	emit newImage(makePixmap(lines->get_image()));
	emit newLayeredImage(&(lines->get_layers()));
	delete sg;
  }
  else QMessageBox::information(this,"Hyperbolic Paintlines",tr("The chosen group is not hyperbolic.  Try increasing the parameters."));
}

void HyperbolicLinesForm::symmetryChanged(int n)
{
  int i;
  for(i=0;i<3;i++) {
    angles[oldsymm][i]=spinAngle[i]->value();
    spinAngle[i]->setEnabled(pistrings[n][i]!=nullptr);
    spinAngle[i]->setMinimum(minangles[n][i]);
    spinAngle[i]->setValue(angles[n][i]);
    labelAngle[i]->setText(anglestrings[n][i]);
    labelPi[i]->setText(pistrings[n][i]);
  }
  oldsymm=n;
}
