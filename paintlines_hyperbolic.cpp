/***************************************************************************
 *   Copyright (C) 2005 by Daniel Gulotta                                  *
 *   dgulotta@mit.edu                                                      *
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

#include "paintlines_hyperbolic.h"

hyperbolic_symmetry_group hyperbolic_3mirror(int m1, int m2, int m3)
{
  double cos1=cos(M_PI/m1);
  double sin1=sin(M_PI/m1);
  double cos2=cos(M_PI/m2);
  double sin2=sin(M_PI/m2);
  double cos3=cos(M_PI/m3);
  double sin3=sin(M_PI/m3);
  double z1=(cos2+cos1*cos3)/(sin1*sin3);
  double x1=sqrt(z1*z1-1.);
  double z2=(cos3+cos1*cos2)/(sin2*sin3);
  double x2=sqrt(z2*z2-1.);
}
