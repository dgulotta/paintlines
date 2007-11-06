/***************************************************************************
 *   Copyright (C) 2005-2007 by Daniel Gulotta                             *
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

#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qcolordialog.h>

void CloudsForm::fileNew()
{

}


void CloudsForm::fileOpen()
{

}


void CloudsForm::fileSave()
{

}


void CloudsForm::fileSaveAs()
{
  QString s=QFileDialog::getSaveFileName();
  if(!s.isEmpty()) CloudsFrame->save(s,"PNG");
}


void CloudsForm::filePrint()
{

}


void CloudsForm::fileExit()
{

}


void CloudsForm::editUndo()
{

}


void CloudsForm::editRedo()
{

}


void CloudsForm::editCut()
{

}


void CloudsForm::editCopy()
{

}


void CloudsForm::editPaste()
{

}


void CloudsForm::editFind()
{

}


void CloudsForm::helpIndex()
{

}


void CloudsForm::helpContents()
{

}


void CloudsForm::helpAbout()
{

}

void CloudsForm::Draw()
{
  int size=EditSize->text().toInt();
  if(size&1) {
    QMessageBox::information(this,"Paintclouds","The size must be even.");
  }
  else if (size<0) {
    QMessageBox::information(this,"Paintclouds","The size must be nonnegative.");
  }
  else {
    symgroup sg=symgroup(ComboSymmetry->currentItem()+int(SYM_CM_O));
    QColor c=ButtonColor1->paletteBackgroundColor();
    CloudsFrame->set_color1(c.red(),c.green(),c.blue());
    c=ButtonColor2->paletteBackgroundColor();
    CloudsFrame->set_color2(c.red(),c.green(),c.blue());
    c=ButtonColor3->paletteBackgroundColor();
    CloudsFrame->set_color3(c.red(),c.green(),c.blue());
    switch(sg) {
	case SYM_CMM_O:
	case SYM_P2_O:
	case SYM_P4_O:
	case SYM_P4G_O:
	case SYM_P4M_O:
	case SYM_PGG_O:
	case SYM_PMG_O:
	case SYM_PMM_O:
	    ButtonRandomize->setEnabled(true);
	    break;
	default:
	    ButtonRandomize->setEnabled(false);
	}
    CloudsFrame->draw(size,sg);
    //CloudsFrame->draw(size,sg,255,255,0,0,255,255,255,0,255);
  }
}

void CloudsForm::Randomize()
{
    CloudsFrame->painterwidget::randomize(SpinXTile->value(),
					   SpinYTile->value());
    ButtonRestore->setEnabled(true);
}


void CloudsForm::Restore()
{
    CloudsFrame->restore();
    ButtonRestore->setEnabled(false);
}



void CloudsForm::SetColor1()
{
  QColor c=QColorDialog::getColor(ButtonColor1->paletteBackgroundColor(),this);
  if(c.isValid()) {
    ButtonColor1->setPaletteBackgroundColor(c);
    ButtonColor1->setDown(false);
  }
}


void CloudsForm::SetColor2()
{
  QColor c=QColorDialog::getColor(ButtonColor2->paletteBackgroundColor(),this);
  if(c.isValid()) {
    ButtonColor2->setPaletteBackgroundColor(c);
    ButtonColor2->setDown(false);
  }
}


void CloudsForm::SetColor3()
{
  QColor c=QColorDialog::getColor(ButtonColor3->paletteBackgroundColor(),this);
  if(c.isValid()) {
    ButtonColor3->setPaletteBackgroundColor(c);
    ButtonColor3->setDown(false);
  }
}
