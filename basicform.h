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

#ifndef _BASICFORM_H
#define _BASICFORM_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include "../canvas.h"
#include "../color.h"

class QBoxLayout;
class QCheckBox;
class QComboBox;
class QLabel;
class QSpinBox;

template<typename T>
class symmetric_canvas;

class layer;

class BasicForm;

struct ImageData
{
	ImageData(const QPixmap &pix, bool t=false, const symmetric_canvas<color_t> *sc=nullptr,
		const std::vector<layer> *l=nullptr) : pixmap(pix), tileable(t), sym_canvas(sc), layers(l) {}
	ImageData(const canvas<color_t> &c, bool t=false, const symmetric_canvas<color_t> *sc=nullptr,
		const std::vector<layer> *l=nullptr);
	ImageData(const canvas<color_t> &c, const symmetric_canvas<color_t> &sc, const std::vector<layer> *l=nullptr) : ImageData(c,true,&sc,l) {}
	QPixmap pixmap;
	bool tileable;
	const symmetric_canvas<color_t> *sym_canvas;
	const std::vector<layer> *layers;
};

class RestoreButton : public QPushButton {
	Q_OBJECT
public:
	RestoreButton() : QPushButton(tr("Restore")) { setEnabled(false); }
public slots:
	void newImage(const ImageData &data) { setEnabled(!data.sym_canvas); }
};

class ImageSaver : public QObject {
	Q_OBJECT
public:
	ImageSaver(QWidget *parent) : QObject(parent) {}
	virtual bool save(const QString &s) { return pix.save(s); }
public slots:
	virtual void newImage(const ImageData &data) { pix=data.pixmap; }
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
	LayeredImageSaver(QWidget *parent) : ImageSaver(parent), layers(nullptr) {}
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
	virtual void newImage(const ImageData &data) { pix=data.pixmap; layers=data.layers; }
protected:
	const std::vector<layer> *layers;
};

#endif

class ImageWidget : public QLabel
{
	Q_OBJECT
	Q_PROPERTY(QPixmap pixmap READ pixmap)
	Q_PROPERTY(bool tiled READ tiled WRITE setTiled)
public:
	ImageWidget()
		: imageIsTileable(false), tilingEnabled(false)
		{
			setAlignment(Qt::AlignLeft|Qt::AlignTop);
		}
	const QPixmap *pixmap() const { return &_pixmap; }
	QSize minimumSizeHint() const { return _pixmap.size(); }
	bool tiled() const { return tilingEnabled; }
public slots:
	void setPixmap(const ImageData &data);
	void setTiled(bool b) { tilingEnabled = b;  recomputeTiling(); }
private:
	void recomputeTiling();
	QPixmap _pixmap;
	bool imageIsTileable;
	bool tilingEnabled;
};

class BasicForm : public QMainWindow
{
	Q_OBJECT
public:
	BasicForm();
	virtual ~BasicForm();
	static QPixmap makePixmap(const canvas<color_t> &src);
signals:
	void newImage(const ImageData &data);
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
	QCheckBox * newTileCheck();
	ImageSaver *saver;
	QMenuBar *menu;
	QMenu *menuFile;
	QAction *actionSaveAs;
	QAction *actionExit;
	ImageWidget *labelImage;
	QLayout *sideLayout;
	QPushButton *buttonDraw;
};

#endif
