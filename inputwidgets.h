/***************************************************************************
 *   Copyright (C) 2008, 2014-2016 by Daniel Gulotta                       *
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

#ifndef _INPUTWIDGETS_H
#define _INPUTWIDGETS_H

#include <QComboBox>
#include <QSpinBox>
#include <vector>
#include "color.h"
#include "symmetry.h"

class SymmetryCombo : public QComboBox
{
	Q_OBJECT
public:
	SymmetryCombo(bool random=true);
	SymmetryCombo(const std::vector<symgroup> &groups,bool random=true);
	symgroup group();
	static const QStringList symmetryStrings;
};

class SizeSpin : public QSpinBox
{
	Q_OBJECT
public:
	SizeSpin(int step=1);
protected:
	int valueFromText(const QString &text) const;
};

class ColorButton : public QWidget
{
	Q_OBJECT
public:
	ColorButton(QWidget *parent=0) : QWidget(parent) {
		setAutoFillBackground(true);
	}
	ColorButton(QColor col, QWidget *parent=0) : ColorButton(parent) {
		setPalette(QPalette(col));
	}
	color_t color() {
		QColor c = palette().color(QPalette::Window);
		return color_t(c.red(),c.green(),c.blue());
	}
protected:
	virtual void mousePressEvent(QMouseEvent *);
};

template<typename T>
class DataComboAdapter
{
public:
	DataComboAdapter(const std::vector<std::pair<const char *,T>> &v) : _box(new QComboBox) {
		for(auto &p : v)
			_box->addItem(p.first,QVariant::fromValue<T>(p.second));
	}
	T value() const { return qvariant_cast<T>(_box->currentData()); }
	QComboBox * box() { return _box; }
private:
	QComboBox *_box;
};

template<typename T>
class EnumComboAdapter
{
public:
	EnumComboAdapter() : _box(new QComboBox) {
		_box->addItems(defaultItems);
	}
	T value() const { return static_cast<T>(_box->currentIndex()); }
	QComboBox * box() { return _box; }
private:
	QComboBox *_box;
	static const QStringList defaultItems;
};

static const int DEFAULT_NCOLORS=25;

#endif
