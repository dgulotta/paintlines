/***************************************************************************
 *   Copyright (C) 2005-2008 by Daniel Gulotta                             *
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

#ifndef _HYPERBOLIC_PAINTLINESWIDGET_H
#define _HYPERBOLIC_PAINTLINESWIDGET_H

#include "../basic_painterwidget.h"
#include "hyperbolic_paintlines.h"

class hyperbolic_paintlineswidget : public basic_painterwidget,
				    public hyperbolic_paintlines
{
    Q_OBJECT
public:
    hyperbolic_paintlineswidget(QWidget *parent=0);
    void draw(int sz, int n, hyperbolic_symmetry_group sym, projtype p);
};

#endif