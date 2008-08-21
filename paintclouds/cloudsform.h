/***************************************************************************
 *   Copyright (C) 2008 by Daniel Gulotta                                  *
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

#ifndef _CLOUDSFORM_H
#define _CLOUDSFORM_H

#include <QMainWindow>

class QGridLayout;
class QComboBox;
class QSpinBox;
class QPushButton;
class paintcloudswidget;

class CloudsForm : public QMainWindow
{
  Q_OBJECT
public:
  CloudsForm();
protected:
  //void closeEvent(QCloseEvent *event);
private slots:
  bool saveAs();
  void draw();
  void randomize();
  void restore();
private:
  QMenuBar *menu;
  QMenu *menuFile;
  QAction *actionSaveAs;
  QAction *actionExit;
  QComboBox *comboSymmetry;
  QSpinBox *spinSize;
  QPushButton *buttonDraw;
  QSpinBox *spinXTiles;
  QSpinBox *spinYTiles;
  QPushButton *buttonRandomize;
  QPushButton *buttonRestore;
  QWidget *color1;
  QWidget *color2;
  QWidget *color3;
  paintcloudswidget *clouds;
};

#endif
