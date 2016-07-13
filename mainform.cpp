/***************************************************************************
 *   Copyright (C) 2008, 2013-2016 by Daniel Gulotta                       *
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
 
#include <QtWidgets>
#include "converterwidget.h"
#include "ca/cawidget.h"
#include "converter/loaderwidget.h"
#include "hyperbolic_paintlines/hyperboliclineswidget.h"
#include "paintclouds/cloudswidget.h"
#include "paintclouds/hyperboliccloudswidget.h"
#include "paintlines/lineswidget.h"
#include "trap/trapwidget.h"
#include "trap/quasitrapwidget.h"
#ifdef FFTWPROGS
#include "paintsquiggles/squiggleswidget.h"
#include "paintstripes/stripeswidget.h"
#include "quasiperiodic_paintstripes/quasistripeswidget.h"
#endif
#ifdef MULTIPAGE
#include "magick.h"
#endif
#include "mainform.h"

MainForm::MainForm()
{
	designs = new QStackedWidget;
	designsDock = new QDockWidget(tr("Design"));
	designsDock->setWidget(designs);
	designsDock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
	addDockWidget(Qt::LeftDockWidgetArea,designsDock);
	ConverterWidget *converterWidget = new ConverterWidget;
	QDockWidget *converterDock = new QDockWidget(tr("Postprocessing"));
	converterDock->setWidget(converterWidget);
	converterDock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
	tabifyDockWidget(designsDock,converterDock);
	designsDock->raise();
	imageWidget = new ImageWidget;
	QScrollArea *a = new QScrollArea;
	a->setWidgetResizable(true);
	a->setWidget(imageWidget);
	setCentralWidget(a);
	resize(1024,768);
	designActions = new QActionGroup(this);
	QMenu *menuFile = menuBar()->addMenu(tr("&File"));
	menuDesign = menuBar()->addMenu(tr("&Design"));
	QMenu *menuView = menuBar()->addMenu(tr("&View"));
	actionSaveAs = menuFile->addAction("&Save As");
	actionSaveAs->setEnabled(false);
	actionSaveLayers = menuFile->addAction(tr("Save Individual &Layers"));
	actionSaveLayers->setEnabled(false);
#ifndef MULTIPAGE
	actionSaveLayers->setVisible(false);
#endif
	menuFile->addAction(tr("&Copy"),this,SLOT(copy()));
	menuFile->addAction(tr("E&xit"),this,SLOT(close()));
	QAction *actionTile = menuView->addAction(tr("&Tile"));
	addDesign("Cellular Automata",new CAWidget);
	addDesign("Clouds",new CloudsWidget);
	addDesign("Hyperbolic Clouds",new HyperbolicCloudsWidget);
	addDesign("Hyperbolic Lines",new HyperbolicLinesWidget);
	loader = new LoaderWidget;
	actionLoader = addDesign("Import",loader);
	addDesign("Lines",new LinesWidget);
	addDesign("Orbit Trap",new TrapWidget);
	addDesign("Quasiperiodic Orbit Trap",new QuasiTrapWidget);
#ifdef FFTWPROGS
	addDesign("Quasiperiodic Stripes",new QuasiStripesWidget);
	addDesign("Squiggles",new SquigglesWidget);
	addDesign("Stripes",new StripesWidget);
#endif
	converterWidget->makeConnections(this);
	connect(this,&MainForm::newImage,imageWidget,&ImageWidget::setPixmap);
	connect(this,&MainForm::newImage,this,&MainForm::processNewImage);
	connect(actionSaveAs,&QAction::triggered,this,&MainForm::saveAs);
	connect(actionSaveLayers,&QAction::triggered,this,&MainForm::saveLayers);
	connect(actionTile,&QAction::toggled,imageWidget,&ImageWidget::setTiled);
	actionTile->setCheckable(true);
	actionTile->setChecked(true);
	designActions->actions()[0]->trigger();
	setAcceptDrops(true);
}

bool MainForm::saveAs()
{
	QString s=QFileDialog::getSaveFileName();
	if(!s.isEmpty()) return lastData.img.save(s);
	return false;
}

bool MainForm::saveLayers()
{
#ifdef MULTIPAGE
	QString s=QFileDialog::getSaveFileName();
	const QImage &img = lastData.img;
	if(!s.isEmpty()) return save_multilayer(img.width(),img.height(),*(lastData.layers),s.toStdString());
#endif
	return false;
}

void MainForm::processNewImage(const ImageData &data)
{
	actionSaveAs->setEnabled(true);
	actionSaveLayers->setEnabled(data.layers!=nullptr);
	lastData=data;
}

QAction *MainForm::addDesign(const QString &name, ImageGeneratorWidget *w)
{
	QAction *act = menuDesign->addAction(name);
	designs->addWidget(w);
	designActions->addAction(act);
	act->setCheckable(true);
	connect(act,&QAction::triggered,[=] () { designs->setCurrentWidget(w); designsDock->raise(); });
	connect(w,&ImageGeneratorWidget::newImage,this,&MainForm::newImage);
	return act;
}

void MainForm::dragEnterEvent(QDragEnterEvent *event)
{
	loader->handleDragEnter(event);
}

void MainForm::dropEvent(QDropEvent *event)
{
	if(loader->handleDrop(event))
		actionLoader->trigger();	
}

QImage MainForm::makeImage(const canvas<color_t> &src)
{
	QImage image(src.width(),src.height(),QImage::Format_RGB32);
	for(int j=0;j<src.height();j++)
		for(int i=0;i<src.width();i++) {
			color_t col = src(i,j);
      		image.setPixel(i,j,qRgb(col.red,col.green,col.blue));
		}
	return image;
}

QPixmap MainForm::makePixmap(const canvas<color_t> &src)
{
	return QPixmap::fromImage(makeImage(src));
}

void ImageWidget::setPixmap(const ImageData &data)
{
	_pixmap = QPixmap::fromImage(data.img);
	QPalette pal(palette());
	pal.setBrush(QPalette::Background,_pixmap);
	setPalette(pal);
	imageIsTileable = data.tileable;
	recomputeTiling();
	updateGeometry();
}

void ImageWidget::recomputeTiling()
{
	bool b = tilingEnabled&&imageIsTileable;
	setAutoFillBackground(b);
	QLabel::setPixmap(b?QPixmap():_pixmap);
}

void MainForm::copy()
{
	QApplication::clipboard()->setImage(lastData.img);
}
