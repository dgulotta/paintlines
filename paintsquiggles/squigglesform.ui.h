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

#include <q3filedialog.h>
#include <qmessagebox.h>

void SquigglesForm::fileNew()
{

}


void SquigglesForm::fileOpen()
{

}


void SquigglesForm::fileSave()
{

}


void SquigglesForm::fileSaveAs()
{
  QString s=Q3FileDialog::getSaveFileName();
  if(!s.isEmpty()) StripesFrame->save(s,"PNG");
}


void SquigglesForm::filePrint()
{

}


void SquigglesForm::fileExit()
{

}


void SquigglesForm::editUndo()
{

}


void SquigglesForm::editRedo()
{

}


void SquigglesForm::editCut()
{

}


void SquigglesForm::editCopy()
{

}


void SquigglesForm::editPaste()
{

}


void SquigglesForm::editFind()
{

}


void SquigglesForm::helpIndex()
{

}


void SquigglesForm::helpContents()
{

}


void SquigglesForm::helpAbout()
{

}


void SquigglesForm::Draw()
{
  int size=EditSize->text().toInt();
  double alpha=EditAlpha->text().toDouble();
  if(size&1) {
    QMessageBox::information(this,"Paintsquiggles","The size must be even.");
  }
  else if (size<0) {
    QMessageBox::information(this,"Paintsquiggles","The size must be nonnegative.");
  }
  else if (alpha<=0.||alpha>2.) {
    QMessageBox::information(this,"Paintsquiggles","Alpha must be in (0,2].");
  }
  else {
    symgroup sg=symgroup(ComboSymmetry->currentItem());
    ButtonRandomize->setEnabled(true);
    StripesFrame->set_alpha(alpha);
    StripesFrame->set_ncolors(SpinColors->value());
    StripesFrame->draw(size,sg);
  }
}

void SquigglesForm::Randomize()
{
    StripesFrame->painterwidget::randomize(SpinXTile->value(),
					   SpinYTile->value());
    ButtonRestore->setEnabled(true);
}


void SquigglesForm::Restore()
{
    StripesFrame->restore();
    ButtonRestore->setEnabled(false);
}
