/***************************************************************************
 *   Copyright (C) 2005-2007 by Daniel Gulotta                             *
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

#include <qapplication.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <memory>
#include "../hyperbolic_painter.h"

using namespace std;

void HyperbolicLinesForm::fileSaveAs()
{
 QString s=QFileDialog::getSaveFileName();
 if(!s.isEmpty()) HyperbolicPaintFrame->save(s,"PNG");
}


void HyperbolicLinesForm::fileExit()
{
  QApplication::exit(0);
}


void HyperbolicLinesForm::editCopy()
{

}


void HyperbolicLinesForm::helpAbout()
{
  QMessageBox::information(this,"About Hyperbolic Paintlines",
				 "Hyperbolic Paintlines copyright (C) 2005-2007 Daniel Gulotta.\n\nThis program is free software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later version.  See COPYING for details.");
}


void HyperbolicLinesForm::SymmetryChanged(int n)
{
  *angle1=SpinAngle1->value();
  if(angle2) *angle2=SpinAngle2->value();
  if(angle3) *angle3=SpinAngle3->value();
  switch(n) {
  case 0:
    LabelAngle1->setText("Sum of angles");
    LabelAngle2->setText("");
    LabelAngle3->setText("");
    LabelPi1->setText("Pi/");
    LabelPi2->setEnabled(false);
    LabelPi3->setEnabled(false);
    SpinAngle1->setMinValue(2);
    SpinAngle2->setEnabled(false);
    SpinAngle3->setEnabled(false);
    angle1=&cm_1;
    angle2=NULL;
    angle3=NULL;
    SpinAngle1->setValue(cm_1);
    ComboSubset->setEnabled(true);
    break;
  case 1:
    LabelAngle1->setText("Mirror angle");
    LabelAngle2->setText("Other angles");
    LabelAngle3->setText("");
    LabelPi1->setText("Pi/");
    LabelPi2->setText("Pi/2*");
    LabelPi2->setEnabled(true);
    LabelPi3->setEnabled(false);
    SpinAngle1->setMinValue(2);
    SpinAngle2->setMinValue(2);
    SpinAngle2->setEnabled(true);
    SpinAngle3->setEnabled(false);
    angle1=&cmm_1;
    angle2=&cmm_2;
    angle3=NULL;
    SpinAngle1->setValue(cmm_1);
    SpinAngle2->setValue(cmm_2);
    ComboSubset->setEnabled(true);
    break;
  case 2:
    LabelAngle1->setText("Sum of angles");
    LabelAngle2->setText("");
    LabelAngle3->setText("");
    LabelPi1->setText("2 Pi/");
    LabelPi2->setEnabled(false);
    LabelPi3->setEnabled(false);
    SpinAngle1->setMinValue(3);
    SpinAngle2->setEnabled(false);
    SpinAngle3->setEnabled(false);
    angle1=&p2_1;
    angle2=NULL;
    angle3=NULL;
    SpinAngle1->setValue(p2_1);
    ComboSubset->setEnabled(false);
    break;
  case 3:
    LabelAngle1->setText("Angle 1");
    LabelAngle2->setText("Angle 2");
    LabelAngle3->setText("Angle 3");
    LabelPi1->setText("Pi/");
    LabelPi2->setText("Pi/");
    LabelPi3->setText("Pi/");
    LabelPi2->setEnabled(true);
    LabelPi3->setEnabled(true);
    SpinAngle1->setMinValue(2);
    SpinAngle2->setMinValue(2);
    SpinAngle3->setMinValue(2);
    SpinAngle2->setEnabled(true);
    SpinAngle3->setEnabled(false);
    angle1=&p4_1;
    angle2=&p4_2;
    angle3=&p4_3;
    SpinAngle1->setValue(p4_1);
    SpinAngle2->setValue(p4_2);
    //SpinAngle3->setValue(p4_3);
    ComboSubset->setEnabled(false);
    break;
  case 4:
    LabelAngle1->setText("Rotation angle");
    LabelAngle2->setText("Other angles");
    LabelAngle3->setText("");
    LabelPi1->setText("2 Pi/");
    LabelPi2->setText("Pi/2*");
    LabelPi2->setEnabled(true);
    LabelPi3->setEnabled(false);
    SpinAngle1->setMinValue(3);
    SpinAngle2->setMinValue(2);
    SpinAngle2->setEnabled(true);
    SpinAngle3->setEnabled(false);
    angle1=&p4g_1;
    angle2=&p4g_2;
    angle3=NULL;
    SpinAngle1->setValue(p4g_1);
    SpinAngle2->setValue(p4g_2);
    ComboSubset->setEnabled(true);
    break;
  case 5:
    LabelAngle1->setText("Angle 1");
    LabelAngle2->setText("Angle 2");
    LabelAngle3->setText("Angle 3");
    LabelPi1->setText("Pi/");
    LabelPi2->setText("Pi/");
    LabelPi3->setText("Pi/");
    LabelPi2->setEnabled(true);
    LabelPi3->setEnabled(true);
    SpinAngle1->setMinValue(2);
    SpinAngle2->setMinValue(2);
    SpinAngle2->setEnabled(true);
    SpinAngle3->setEnabled(true);
    angle1=&p4m_1;
    angle2=&p4m_2;
    angle3=&p4m_3;
    SpinAngle1->setValue(p4m_1);
    SpinAngle2->setValue(p4m_2);
    SpinAngle3->setValue(p4m_3);
    ComboSubset->setEnabled(true);
    break;
  case 6:
    LabelAngle1->setText("Sum of angles");
    LabelAngle2->setText("");
    LabelAngle3->setText("");
    LabelPi1->setText("2 Pi/");
    LabelPi2->setEnabled(false);
    LabelPi3->setEnabled(false);
    SpinAngle1->setMinValue(3);
    SpinAngle2->setEnabled(false);
    SpinAngle3->setEnabled(false);
    angle1=&pgg_1;
    angle2=NULL;
    angle3=NULL;
    SpinAngle1->setValue(pgg_1);
    ComboSubset->setEnabled(true);
    break;
  case 7:
    LabelAngle1->setText("Sum of angles");
    LabelAngle2->setText("");
    LabelAngle3->setText("");
    LabelPi1->setText("Pi/");
    LabelPi2->setEnabled(false);
    LabelPi3->setEnabled(false);
    SpinAngle1->setMinValue(2);
    SpinAngle2->setEnabled(false);
    SpinAngle3->setEnabled(false);
    angle1=&pmg_1;
    angle2=NULL;
    angle3=NULL;
    SpinAngle1->setValue(pmg_1);
    ComboSubset->setEnabled(true);
    break;
  }
}

void HyperbolicLinesForm::Draw()
{
  int size=EditSize->text().toInt();
  if (size<0) {
	QMessageBox::information(this,"Hyperbolic Paintlines",
				 "The size must be nonnegative.");
  }
  else {
    auto_ptr<hyperbolic_symmetry_group> sg;
    bool candraw;
    switch(ComboSymmetry->currentItem()) {
    case 0:
      cm_1=SpinAngle1->value();
      candraw=true;
      sg=auto_ptr<hyperbolic_symmetry_group>(hyperbolic_glide_mirror((M_PI/3.)/cm_1,(M_PI/3.)/cm_1,(flip_type)ComboSubset->currentItem()));
      break;
    case 1:
      cmm_1=SpinAngle1->value();
      cmm_2=SpinAngle2->value();
      candraw=(cmm_1>2||cmm_2>2);
      if(candraw) sg=auto_ptr<hyperbolic_symmetry_group>
	(hyperbolic_2mirror_180(cmm_1,M_PI_2/cmm_2,M_PI_2/cmm_2,
				(flip_type)ComboSubset->currentItem()));
      break;
    case 2:
      p2_1=SpinAngle1->value();
      candraw=true;
      sg=auto_ptr<hyperbolic_symmetry_group>
	(hyperbolic_3_180((2.*M_PI/3.)/p2_1,(2.*M_PI/3.)/p2_1,
			  (2.*M_PI/3.)/p2_1));
      break;
    case 3:
      p4_1=SpinAngle1->value();
      p4_2=SpinAngle2->value();
      p4_3=SpinAngle3->value();
      candraw=(p4_1*p4_2+p4_1*2+p4_2*2<p4_1*p4_2*2);
      if (candraw) sg=auto_ptr<hyperbolic_symmetry_group>
	(hyperbolic_180_rotation(p4_1,p4_2));
      break;
    case 4:
      p4g_1=SpinAngle1->value();
      p4g_2=SpinAngle2->value();
      candraw=(p4g_1+2*p4g_2<p4g_1*p4g_2);
      if(candraw) sg=auto_ptr<hyperbolic_symmetry_group>
		    (hyperbolic_mirror_rotation
		     (p4g_1,p4g_2,(flip_type)ComboSubset->currentItem()));
      break;
    case 5:
      p4m_1=SpinAngle1->value();
      p4m_2=SpinAngle2->value();
      p4m_3=SpinAngle3->value();
      candraw=(p4m_1*p4m_2+p4m_1*p4m_3+p4m_2*p4m_3<p4m_1*p4m_2*p4m_3);
      if(candraw) sg=auto_ptr<hyperbolic_symmetry_group>
		    (hyperbolic_3mirror(p4m_1,p4m_2,p4m_3,
					(flip_type)ComboSubset->currentItem()));
      break;
    case 6:
      pgg_1=SpinAngle1->value();
      candraw=true;
      sg=auto_ptr<hyperbolic_symmetry_group>
	(hyperbolic_glide_180((2.*M_PI/3.)/pgg_1,(2.*M_PI/3.)/pgg_1,
			      (flip_type)ComboSubset->currentItem()));
      break;
    case 7:
      pmg_1=SpinAngle1->value();
      candraw=true;
      sg=auto_ptr<hyperbolic_symmetry_group>
	(hyperbolic_mirror_2_180((M_PI/3.)/pmg_1,(M_PI/3.)/pmg_1,
				 (M_PI/3.)/pmg_1,
				 (flip_type)ComboSubset->currentItem()));
    }
    if(candraw) {
      projtype pt;
      if(ButtonPoincare->isChecked()) pt=POINCARE;
      else pt=KLEIN;
      HyperbolicPaintFrame->draw(size,SpinColors->value(),*sg,pt);
    }
    else QMessageBox::information
	   (this,"Hyperbolic Paintlines",
	    "The sum of the angles must be less than pi.");
  }
}

void HyperbolicLinesForm::init()
{
  cm_1=2;
  cmm_1=3;
  cmm_2=2;
  p2_1=3;
  p4_1=3;
  p4_2=3;
  p4_3=4;
  p4g_1=5;
  p4g_2=2;
  p4m_1=5;
  p4m_2=4;
  p4m_3=2;
  pgg_1=3;
  pmg_1=2;
  angle1=&cm_1;
  angle2=NULL;
  angle3=NULL;
}




