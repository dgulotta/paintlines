/***************************************************************************
 *   Copyright (C) 2016 by Daniel Gulotta                                  *
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

#ifndef _HYPERBOLIC_SYMMETRY_CHOOSER_H
#define _HYPERBOLIC_SYMMETRY_CHOOSER_H

#include <string>
#include <vector>
#include <QWidget>
#include "hyperbolic_group.h"

class QComboBox;
class QSpinBox;
class QStackedLayout;

class HyperbolicSymmetryItem : public QWidget
{
	Q_OBJECT
public:
	HyperbolicSymmetryItem(const fundamental_domain_family &fdf,const std::vector<int> &defaults);
	fundamental_domain domain() const;
	fundamental_domain_family domain_family() const { return family; }
	QString group_name() const { return name; }
private:
	std::vector<QSpinBox *> spins;
	fundamental_domain_family family;
	QString name;
};

class HyperbolicSymmetryChooser : public QWidget
{
	Q_OBJECT
public:
	HyperbolicSymmetryChooser(bool add_default=true);
	fundamental_domain domain() const { return currentItem()->domain(); }
	fundamental_domain_family domain_family() const { return currentItem()->domain_family(); }
	void add(const fundamental_domain_family &fdf,const std::vector<int> &defaults);
	void addDefaultItems();
private:
	QComboBox *familyCombo;
	QStackedLayout *itemStack;
	HyperbolicSymmetryItem * currentItem() const;
};

#endif
