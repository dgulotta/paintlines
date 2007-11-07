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

#include <qfiledialog.h>
#include <qmessagebox.h>

void StripesForm::fileNew()
{

}


void StripesForm::fileOpen()
{

}


void StripesForm::fileSave()
{

}


void StripesForm::fileSaveAs()
{
  QString s=QFileDialog::getSaveFileName();
  if(!s.isEmpty()) StripesFrame->save(s,"PNG");
}


void StripesForm::filePrint()
{

}


void StripesForm::fileExit()
{

}


void StripesForm::editUndo()
{

}


void StripesForm::editRedo()
{

}


void StripesForm::editCut()
{

}


void StripesForm::editCopy()
{

}


void StripesForm::editPaste()
{

}


void StripesForm::editFind()
{

}


void StripesForm::helpIndex()
{

}


void StripesForm::helpContents()
{

}


void StripesForm::helpAbout()
{

}


void StripesForm::Draw()
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
    symgroup sg=symgroup(ComboSymmetry->currentItem());
    switch(sg) {
	case SYM_CMM:
	case SYM_P2:
	case SYM_P3:
	case SYM_P31M:
	case SYM_P3M1:
	case SYM_P4:
	case SYM_P4G:
	case SYM_P4M:
	case SYM_P6:
	case SYM_P6M:
	case SYM_PGG:
	case SYM_PMG:
	case SYM_PMM:
	    ButtonRandomize->setEnabled(true);
	    break;
	default:
	    ButtonRandomize->setEnabled(false);
	}
    StripesFrame->set_alpha(alpha);
    StripesFrame->draw(size,sg);
  }
}

void StripesForm::Randomize()
{
    StripesFrame->painterwidget::randomize(SpinXTile->value(),
					   SpinYTile->value());
    ButtonRestore->setEnabled(true);
}


void StripesForm::Restore()
{
    StripesFrame->restore();
    ButtonRestore->setEnabled(false);
}
