/***************************************************************************
 *   Copyright (C) 2008, 2013 by Daniel Gulotta                            *
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

#ifndef _BASICFORM_H
#define _BASICFORM_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QPushButton>
#include <vector>
#include "../canvas.h"
#include "../color.h"

class QBoxLayout;
class QComboBox;
class QLabel;
class QSpinBox;

class RestoreButton : public QPushButton {
	Q_OBJECT
public:
	RestoreButton() : QPushButton(tr("Restore")) {}
public slots:
	void enable() { setEnabled(true); }
	void disable() { setEnabled(false); }
};

class ImageSaver : public QObject {
	Q_OBJECT
public:
	ImageSaver(QWidget *parent) : QObject(parent) {}
	virtual bool save(const QString &s) { return pix.save(s); }
public slots:
	void newImage(const QPixmap &p) { pix=p; }
protected:
	QPixmap pix;
};

#ifdef MULTIPAGE

#include <QMessageBox>
#include "../layer.h"
#include "../magick.h"

class LayeredImageSaver : public ImageSaver {
	Q_OBJECT
public:
	LayeredImageSaver(QWidget *parent) : ImageSaver(parent) {}
	virtual bool save(const QString &s) {
		if(layers&&(s.toUpper().endsWith("TIF")||s.toUpper().endsWith("TIFF"))&&
			QMessageBox::question((QWidget *)parent(),QCoreApplication::applicationName(),tr("Save individiual layers?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes) {
			return save_multilayer(pix.width(),pix.height(),*layers,s.toStdString());
		}
		else {
			return ImageSaver::save(s);
		}
	}
public slots:
	void newLayeredImage(const std::vector<layer> *l) { layers=l; }
protected:
	const std::vector<layer> *layers;
};

#endif

class BasicForm : public QMainWindow
{
	Q_OBJECT
public:
	BasicForm();
	virtual ~BasicForm();
	static QPixmap makePixmap(const canvas<color_t> &src);
signals:
	void newImage(QPixmap p); 
protected slots:
	bool saveAs();
	void baseInit();
	virtual void draw() = 0;
protected:
	virtual void init() = 0;
	static QComboBox * newSymmetryCombo();
	static QSpinBox * newSizeSpin();
	static QSpinBox * newColorSpin();
	static const QStringList symmetryStrings;
	ImageSaver *saver;
	QMenuBar *menu;
	QMenu *menuFile;
	QAction *actionSaveAs;
	QAction *actionExit;
	QLabel *labelImage;
	QLayout *sideLayout;
	QPushButton *buttonDraw;
};

#endif
