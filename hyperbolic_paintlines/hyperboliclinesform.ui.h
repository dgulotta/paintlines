/***************************************************************************
 *   Copyright (C) 2005 by Daniel Gulotta                                  *
 *   dgulotta@mit.edu                                                      *
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

#include <qmessagebox.h>
#include <qfiledialog.h>

void HyperbolicLinesForm::fileNew()
{

}


void HyperbolicLinesForm::fileOpen()
{

}


void HyperbolicLinesForm::fileSave()
{

}


void HyperbolicLinesForm::fileSaveAs()
{
 QString s=QFileDialog::getSaveFileName();
    if(!s.isEmpty()) HyperbolicPaintFrame->save(s,"PNG");
}


void HyperbolicLinesForm::filePrint()
{

}


void HyperbolicLinesForm::fileExit()
{

}


void HyperbolicLinesForm::editUndo()
{

}


void HyperbolicLinesForm::editRedo()
{

}


void HyperbolicLinesForm::editCut()
{

}


void HyperbolicLinesForm::editCopy()
{

}


void HyperbolicLinesForm::editPaste()
{

}


void HyperbolicLinesForm::editFind()
{

}


void HyperbolicLinesForm::helpIndex()
{

}


void HyperbolicLinesForm::helpContents()
{

}


void HyperbolicLinesForm::helpAbout()
{

}


void HyperbolicLinesForm::Draw()
{
  int size=EditSize->text().toInt();
  if (size<0) {
	QMessageBox::information(this,"Paintlines","The size must be nonnegative.");
  }
  else {
    hyperbolic_symmetry_group sg;
    switch(ComboSymmetry->currentItem()) {
    case 0:
      sg=hyperbolic_glide_mirror(M_PI/6.,M_PI/6.);
      break;
    case 1:
      sg=hyperbolic_2mirror_180(3,M_PI/4.,M_PI/4.);
      break;
    case 2:
      sg=hyperbolic_3_180(2.*M_PI/9.,2.*M_PI/9.,2.*M_PI/9.);
      break;
    case 3:
      sg=hyperbolic_180_rotation(4,3);
      break;
    case 4:
      sg=hyperbolic_3mirror(4,3,3);
      break;
    case 5:
      sg=hyperbolic_glide_180(2.*M_PI/9,2.*M_PI/9.);
      break;
    case 6:
      sg=hyperbolic_mirror_2_180(M_PI/6.,M_PI/6.,M_PI/6.);
    }
    HyperbolicPaintFrame->draw(size,SpinColors->value(),sg);
  }

}
