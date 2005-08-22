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
    hyperbolic_symmetry_group sg(hyperbolic_3_180(2.*M_PI/9,2.*M_PI/9,2.*M_PI/9));
    HyperbolicPaintFrame->draw(size,1,sg);
  }

}
