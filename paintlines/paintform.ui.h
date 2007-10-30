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

void PaintForm::fileNew()
{
    
}


void PaintForm::fileOpen()
{
    
}


void PaintForm::fileSave()
{
    
}


void PaintForm::fileSaveAs()
{
    QString s=QFileDialog::getSaveFileName();
    if(!s.isEmpty()) PaintFrame->save(s,"PNG");
}


void PaintForm::filePrint()
{
    
}


void PaintForm::fileExit()
{
    
}


void PaintForm::editUndo()
{
    
}


void PaintForm::editRedo()
{
    
}


void PaintForm::editCut()
{
    
}


void PaintForm::editCopy()
{
    
}


void PaintForm::editPaste()
{
    
}


void PaintForm::editFind()
{
    
}


void PaintForm::helpIndex()
{
    
}


void PaintForm::helpContents()
{
    
}


void PaintForm::helpAbout()
{
    
}


void PaintForm::Draw()
{
    int size=EditSize->text().toInt();
    if(size&1) {
	QMessageBox::information(this,"Paintlines","The size must be even.");
    }
    else if (size<0) {
	QMessageBox::information(this,"Paintlines","The size must be nonnegative.");
    }
    else {
	vector<paintrule> pr(3);
	pr[0].type=(ruletype)ComboType1->currentItem();
	pr[1].type=(ruletype)ComboType2->currentItem();
	pr[2].type=(ruletype)ComboType3->currentItem();
	pr[0].weight=SpinWeight1->value();
	pr[1].weight=SpinWeight2->value();
	pr[2].weight=SpinWeight3->value();
	pr[0].pastel=CheckPastel1->isChecked();
	pr[1].pastel=CheckPastel2->isChecked();
	pr[2].pastel=CheckPastel3->isChecked();
	PaintFrame->set_rules(pr);
	symgroup sg=(symgroup)ComboSymmetry->currentItem();
	PaintFrame->draw(size,SpinColors->value(),sg);
	switch(sg) {
	case SYM_CMM:
	case SYM_P2:
	case SYM_P4:
	case SYM_P4G:
	case SYM_P4M:
	case SYM_PGG:
	case SYM_PMG:
	case SYM_PMM:
	case SYM_P3:
	case SYM_P31M:
	case SYM_P3M1:
	case SYM_P6:
	case SYM_P6M:
	    ButtonRandomize->setEnabled(true);
	    break;
	default:
	    ButtonRandomize->setEnabled(false);
	}
	ButtonRestore->setEnabled(false);
    }
}


void PaintForm::Randomize()
{
    PaintFrame->painterwidget::randomize(SpinXTile->value(),SpinYTile->value());
    ButtonRestore->setEnabled(true);
}


void PaintForm::Restore()
{
    PaintFrame->restore();
    ButtonRestore->setEnabled(false);
}
