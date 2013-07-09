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

#ifndef _HYPERBOLICLINESFORM_H
#define _HYPERBOLICLINESFORM_H

#include <QMainWindow>

class QMenuBar;
class QMenu;
class QAction;
class QGroupBox;
class QComboBox;
class QSpinBox;
class QLabel;
class QPushButton;
class QDoubleSpinBox;
class hyperbolic_paintlineswidget;

class HyperbolicLinesForm : public QMainWindow
{
  Q_OBJECT;
public:
  HyperbolicLinesForm();
private slots:
  void draw();
  void symmetryChanged(int n);
  bool saveAs();
private:
  QMenuBar *menu;
  QMenu *menuFile;
  QAction *actionSaveAs;
  QAction *actionExit;
  QComboBox *comboModel;
  QComboBox *comboSymmetry;
  QLabel *labelAngle[3];
  QLabel *labelPi[3];
  QSpinBox *spinAngle[3];
  QComboBox *comboSubset;
  QSpinBox *spinSize;
  QSpinBox *spinColors;
  QPushButton *buttonDraw;
  QDoubleSpinBox *spinThickness;
  QDoubleSpinBox *spinSharpness;
  hyperbolic_paintlineswidget *lines;
  int angles[11][3];
  int oldsymm;
};

#endif
