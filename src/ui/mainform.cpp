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
#include "converterwidget.hpp"
#include "cawidget.hpp"
#include "genwidgets.hpp"
#include "loaderwidget.hpp"
#include "imagedata.hpp"
#include "lineswidget.hpp"
#include "squiggleswidget.hpp"
#include "magick.hpp"
#include "mainform.hpp"

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
	QAction *actionSaveAs = menuFile->addAction("&Save As");
	actionSaveAs->setEnabled(false);
	QAction *actionSaveLayers = menuFile->addAction(tr("Save Individual &Layers"));
	actionSaveLayers->setEnabled(false);
#ifndef MULTIPAGE
	actionSaveLayers->setVisible(false);
#endif
	menuFile->addAction(tr("&Copy"),imageWidget,&ImageWidget::copy);
	menuFile->addAction(tr("E&xit"),this,&MainForm::close);
	QAction *actionTile = menuView->addAction(tr("&Tile"));
	addDesign("Cellular Automata",new CAWidget);
	addDesign("Clouds",new CloudsWidget);
#ifdef FFTWPROGS
	addDesign("Clusters",new ClustersWidget);
#endif
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
	addDesign("Walk",new WalkWidget);
	converterWidget->makeConnections(this);
	connect(this,&MainForm::newImage,imageWidget,&ImageWidget::setImage);
	connect(actionSaveAs,&QAction::triggered,imageWidget,&ImageWidget::saveAs);
	connect(actionSaveLayers,&QAction::triggered,imageWidget,&ImageWidget::saveLayers);
	connect(imageWidget,&ImageWidget::enableSave,actionSaveAs,&QAction::setEnabled);
	connect(imageWidget,&ImageWidget::enableSaveLayers,actionSaveLayers,&QAction::setEnabled);
	connect(actionTile,&QAction::toggled,imageWidget,&ImageWidget::setTiled);
	actionTile->setCheckable(true);
	actionTile->setChecked(true);
	designActions->actions()[0]->trigger();
	setAcceptDrops(true);
}

bool ImageWidget::saveAs()
{
	QString s=QFileDialog::getSaveFileName();
	if(!s.isEmpty()) {
		QImageWriter writer(s);
		bool b = writer.write(img);
		if(!b) {
			QString err = writer.errorString();
			if(writer.error()==QImageWriter::UnsupportedFormatError) {
				QTextStream st(&err,QIODevice::WriteOnly|QIODevice::Text);
				st << "\n\nSupported formats are:";
				for(auto fmt: writer.supportedImageFormats()) {
					st << "\n" << fmt;
				}
			}
			QMessageBox::critical(this,"Save failed",err);
		}
		return b;
	}
	return false;
}

bool ImageWidget::saveLayers()
{
#ifdef MULTIPAGE
	QString s=QFileDialog::getSaveFileName();
	if(!s.isEmpty()) {
		QString err = save_multilayer(img.width(),img.height(),*layers,s.toStdString());
		if(!err.isNull()) {
			QMessageBox::critical(this,"Save failed",err);
		}
		return err.isNull();
	}
#endif
	return false;
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

void ImageWidget::setImage(const ImageData &data)
{
	img = makeImage(data.img);
	pix = QPixmap::fromImage(img);
	QPalette pal(palette());
	pal.setBrush(QPalette::Background,pix);
	setPalette(pal);
	imageIsTileable = (bool)(data.img.wrap_view);
	layers=data.layers;
	recomputeTiling();
	updateGeometry();
	emit enableSave(true);
	emit enableSaveLayers(data.layers!=nullptr);
}

void ImageWidget::recomputeTiling()
{
	bool b = tilingEnabled&&imageIsTileable;
	setAutoFillBackground(b);
	setPixmap(b?QPixmap():pix);
}

void ImageWidget::copy()
{
	QApplication::clipboard()->setImage(img);
}
