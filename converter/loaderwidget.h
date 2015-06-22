/***************************************************************************
 *   Copyright (C) 2014 by Daniel Gulotta                                  *
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

#ifndef _LOADERWIDGET_H
#define _LOADERWIDGET_H

#include "../color.h"
#include "../imagegeneratorwidget.h"
#include "../symmetric_canvas.h"

class QDragEnterEvent;
class QDropEvent;
class QMimeData;
class QPushButton;

class LoaderWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	LoaderWidget();
	void handleDragEnter(QDragEnterEvent *event);
	bool handleDrop(QDropEvent *event);
protected slots:
	void openFile();
	void paste();
	void checkPasteEnabled();
	void symmetryChanged(int n);
protected:
	std::function<QImage()> mimeToImage(const QMimeData *mime);
	bool open(const QImage &img);
	SymmetryCombo *comboSymmetry;
	QPushButton *buttonPaste;
	symmetric_canvas<color_t> image;
};

#endif
