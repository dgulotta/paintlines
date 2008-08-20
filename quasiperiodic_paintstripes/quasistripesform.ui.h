/***************************************************************************
 *   Copyright (C) 2005-2008 by Daniel Gulotta                             *
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

#include <q3filedialog.h>
#include <qmessagebox.h>

void QuasiStripesForm::fileNew()
{

}


void QuasiStripesForm::fileOpen()
{

}


void QuasiStripesForm::fileSave()
{

}


void QuasiStripesForm::fileSaveAs()
{
  QString s=Q3FileDialog::getSaveFileName();
  if(!s.isEmpty()) StripesFrame->save(s,"PNG");
}


void QuasiStripesForm::filePrint()
{

}


void QuasiStripesForm::fileExit()
{

}


void QuasiStripesForm::editUndo()
{

}


void QuasiStripesForm::editRedo()
{

}


void QuasiStripesForm::editCut()
{

}


void QuasiStripesForm::editCopy()
{

}


void QuasiStripesForm::editPaste()
{

}


void QuasiStripesForm::editFind()
{

}


void QuasiStripesForm::helpIndex()
{

}


void QuasiStripesForm::helpContents()
{

}


void QuasiStripesForm::helpAbout()
{

}


void QuasiStripesForm::Draw()
{
  int size=EditSize->text().toInt();
  double alpha=EditAlpha->text().toDouble();
  if(size&1) {
    QMessageBox::information(this,"Paintstripes","The size must be even.");
  }
  else if (size<0) {
    QMessageBox::information(this,"Paintstripes","The size must be nonnegative.");
  }
  else if (alpha<=0.||alpha>2.) {
    QMessageBox::information(this,"Paintstripes","Alpha must be in (0,2].");
  }
  else {
    StripesFrame->set_alpha(alpha);
    StripesFrame->draw(size);
  }
}

void QuasiStripesForm::Randomize()
{
}


void QuasiStripesForm::Restore()
{
}
