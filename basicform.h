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

#include <QMainWindow>

class QBoxLayout;
class QComboBox;
class QSpinBox;
class QPushButton;
class painterwidget;

class BasicForm : public QMainWindow
{
  Q_OBJECT
public:
  BasicForm();
  virtual ~BasicForm();
protected slots:
  virtual bool saveAs();
  void draw();
  void randomize();
  void restore();
  void init();
protected:
  virtual void addWidgets(QBoxLayout *sideLayout) = 0;
  virtual painterwidget * createPainterWidget() = 0;
  virtual void draw(int sz, int sym_index) = 0;
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
  painterwidget *painter;
};

#endif
