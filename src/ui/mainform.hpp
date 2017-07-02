/***************************************************************************
 *   Copyright (C) 2008, 2013-2014 by Daniel Gulotta                       *
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

#ifndef _MAINFORM_H
#define _MAINFORM_H

#include <vector>
#include <QImage>
#include <QLabel>
#include <QMainWindow>

class ImageGeneratorWidget;
class LoaderWidget;
class QActionGroup;
class QDragEnterEvent;
class QDropEvent;
class QStackedWidget;
struct ImageData;
struct layer;

class ImageWidget : public QLabel
{
	Q_OBJECT
	Q_PROPERTY(bool tiled READ tiled WRITE setTiled)
public:
	ImageWidget()
		: imageIsTileable(false), tilingEnabled(false)
		{
			setAlignment(Qt::AlignLeft|Qt::AlignTop);
		}
	QSize minimumSizeHint() const { return pix.size(); }
	bool tiled() const { return tilingEnabled; }
public slots:
	void setImage(const ImageData &data);
	void setTiled(bool b) { tilingEnabled = b;  recomputeTiling(); }
	bool saveAs();
	bool saveLayers();
	void copy();
signals:
	void enableSave(bool);
	void enableSaveLayers(bool);
private:
	void recomputeTiling();
	const std::vector<layer> *layers;
	QPixmap pix;
	QImage img;
	bool imageIsTileable;
	bool tilingEnabled;
};

class MainForm : public QMainWindow
{
	Q_OBJECT
public:
	MainForm();
signals:
	void newImage(const ImageData &data);
private:
	QAction *addDesign(const QString &name, ImageGeneratorWidget *w);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	QStackedWidget *designs;
	ImageWidget *imageWidget;
	LoaderWidget *loader;
	QAction *actionLoader;
	QMenu *menuDesign;
	QActionGroup *designActions;
	QDockWidget *designsDock;
};

#endif

