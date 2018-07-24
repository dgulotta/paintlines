/***************************************************************************
 *   Copyright (C) 2008, 2014-2017 by Daniel Gulotta                       *
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

#ifndef _GENWIDGETS_HPP
#define _GENWIDGETS_HPP

#include "imagegeneratorwidget.hpp"

class CloudsWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	CloudsWidget();
};

class HyperbolicCloudsWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	HyperbolicCloudsWidget();
};

class HyperbolicLinesWidget : public ImageGeneratorWidget
{
	Q_OBJECT;
public:
	HyperbolicLinesWidget();
};

class TrapWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	TrapWidget();
};

class QuasiTrapWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	QuasiTrapWidget();
};

class WalkWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	WalkWidget();
};

#ifdef FFTWPROGS

class StripesWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	StripesWidget();
};

class ClustersWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	ClustersWidget();
};

class QuasiStripesWidget : public ImageGeneratorWidget
{
	Q_OBJECT
public:
	QuasiStripesWidget();
};

#endif
#endif
