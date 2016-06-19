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

#ifndef _FFTW_CXX_H
#define _FFTW_CXX_H

#include <complex>
#include <new>
#include <fftw3.h>

template <typename T>
T * check_alloc(T *t)
{
	if(t)
		return t;
	else
		throw std::bad_alloc();
}

inline std::complex<double> * allocate_complex(size_t s)
{
	return reinterpret_cast<std::complex<double> *>(check_alloc(fftw_alloc_complex(s)));
}

struct fftw_free_deleter
{
	void operator () (std::complex<double> *p) { fftw_free(static_cast<void *>(p)); }
};

struct fftw_plan_deleter
{
	void operator () (fftw_plan p) { fftw_destroy_plan(p); }
};

#endif
