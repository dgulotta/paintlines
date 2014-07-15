/***************************************************************************
 *   Copyright (C) 2013 by Daniel Gulotta                                  *
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

#ifndef _CONVERTERFORM_H
#define _CONVERTERFORM_H

#include "../basicform.h"
#include "../symmetric_canvas.h"

class QCheckBox;
class QDragEnterEvent;
class QDropEvent;
class RandomizeWidget;

class ConverterForm : public BasicForm
{
	Q_OBJECT
protected slots:
	virtual void draw();
	virtual void init();
	virtual void open();
	void hexStretch();
	void updateImage();
	void symmetryChanged(int n);
	void paste();
	void checkPasteEnabled();
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void open(const QString &s);
	void open(const QImage &img);
	QComboBox *comboSymmetry;
	QComboBox *comboModel;
	QSpinBox *spinSize;
	RandomizeWidget *randomizeWidget;
	QCheckBox *checkTiled;
	RestoreButton *buttonRestore;
	QPushButton *buttonHexStretch;
	QAction *actionPaste;
	symmetric_canvas<color_t> image;
};

#endif
