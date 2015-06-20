/***************************************************************************
 *   Copyright (C) 2014-2015 by Daniel Gulotta                             *
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

#include "../randgen.h"
#include "trap.h"

using std::tie;
using std::tuple;
using std::function;
using std::make_tuple;

#define PT(a,b) [=] (double x, double y) { double cx = cos(x), cy = cos(y), \
	sx = sin(x), sy = sin(y); return make_tuple((a),(b)); }

#include <cstdio>

tuple<int,int,int,int> random_matrix(int cmax=1)
{
	int p,q,r,s,t,d;
	do {
		p = random_range_inclusive(-cmax,cmax);
		q = random_range_inclusive(-cmax,cmax);
		r = random_range_inclusive(-cmax,cmax);
		s = random_range_inclusive(-cmax,cmax);
		t = abs(p+s);
		d = p*s-q*r;
	} while(d>1||t-d>1);
	return make_tuple(p,q,r,s);
}

double random_torsion(int n)
{
	if(n==0) return random_angle();
	else return random_int(n)*(2*M_PI/n);
}

iterfunc randfunc_p1(symgroup sg)
{
	double a0=random_angle(), a3=random_normal(1.), a4=random_normal(1.);
	double a5=random_normal(1.), a6=random_normal(1.);
	double b0=random_angle(), b3=random_normal(1.), b4=random_normal(1.);
	double b5=random_normal(1.), b6=random_normal(1.);
	int a1, a2, b1, b2;
	tie(a1,a2,b1,b2)=random_matrix();
	return PT(a0+a1*x+a2*y+a3*cx+a4*sx+a5*cy+a6*sy,
		b0+b1*x+b2*y+b3*cx+b4*sx+b5*cy+b6*sy);
}

iterfunc randfunc_p2(symgroup sg)
{
	if(random_bool()) {
		double a0=random_torsion(2), a4=random_normal(1.), a6=random_normal(1.);
		double b0=random_torsion(2), b4=random_normal(1.), b6=random_normal(1.);
		int a1, a2, b1, b2;
		tie(a1,a2,b1,b2)=random_matrix();
		return PT(a0+a1*x+a2*y+a4*sx+a6*sy,b0+b1*x+b2*y+b4*sx+b6*sy);
	}
	else {
		double a0=random_angle(), a3=random_normal(1.), a5=random_normal(1.);
		double b0=random_angle(), b3=random_normal(1.), b5=random_normal(1.);
		return PT(a0+a3*cx+a5*cy,b0+b3*cx+b5*cy);
	}
}

iterfunc randfunc_cm(symgroup sg)
{
	if(random_bool()) {
		double a0=random_angle(), a3=random_normal(1.), a4=random_normal(1.);
		double a5=random_normal(1.), a6=random_normal(1.);
		int a1,a2;
		do {
			a1 = random_range_inclusive(-1,1);
			a2 = random_range_inclusive(-1,1);
		} while(a1!=0&&a2!=0);
		return PT(a0+a1*x+a2*y+a3*cx+a4*sx+a5*cy+a6*sy,
			a0+a2*x+a1*y+a5*cx+a6*sx+a3*cy+a4*sy);
	}
	else {
		double a0=random_angle(), a3=random_normal(1.), a4=random_normal(1.);
		double b0=random_angle(), b3=random_normal(1.), b4=random_normal(1.);
		int a1, b1;
		do {
			a1 = random_range_inclusive(-1,1);
			b1 = random_range_inclusive(-1,1);
		} while(abs(a1+b1)>=2);
		return PT(a0+a1*(x+y)+a3*(cx+cy)+a4*(sx+sy),
			b0+b1*(x+y)+b3*(cx+cy)+b4*(sx+sy));
	}
}

iterfunc randfunc_cmm(symgroup sg)
{
	if(random_bool()) {
		double a0=random_torsion(2), a4=random_normal(1.), a6=random_normal(1.);
		int a1, a2;
		do {
			a1 = random_range_inclusive(-1,1);
			a2 = random_range_inclusive(-1,1);
		} while(a1*a1+a2*a2>=2);
		return PT(a0+a1*x+a2*y+a4*sx+a6*sy,a0+a2*x+a1*y+a6*sx+a4*sy);
	}
	else {
		double a0=random_torsion(2), a4=random_normal(1.), a6=random_normal(1.);
		int a1=random_range_inclusive(-1,1), a2=random_range_inclusive(-1,1);
		return PT(a0+a1*x+a2*y+a4*sx+a6*sy,a0-a2*x-a1*y-a6*sx-a4*sy);
	}
}

iterfunc randfunc_pm(symgroup sg)
{
	if(random_bool()) {
		double a0=random_torsion(2);
		double a4=random_normal(1.);
		double b0=random_angle(), b3=random_normal(1.), b5=random_normal(1.);
		double b6=random_normal(1.);
		int a1=random_range_inclusive(-1,1), b2=random_range_inclusive(-1,1);
		return PT(a0+a1*x+a4*sx,b0+b2*y+b3*cx+b5*sy+b6*cy);
	}
	else {
		double a0=random_angle(), a3=random_normal(1.), a5=random_normal(1.);
		double a6=random_normal(1.);
		double b0=random_angle(), b3=random_normal(1.), b5=random_normal(1.);
		double b6=random_normal(1.);
		int a2=random_range_inclusive(-1,1), b2=random_range_inclusive(-1,1);
		return PT(a0+a2*y+a3*cx+a5*cy+a6*sy,b0+b2*y+b3*cx+b5*cy+b6*sy);
	}
}

iterfunc randfunc_pg(symgroup sg)
{
	int a1=random_sign(), b2=random_range_inclusive(-1,1);
	double a0=random_angle(), a6=random_normal(1.);
	double b0=(b2-1)*M_PI_2+random_torsion(2), b3=random_normal(1.), b4=random_normal(1.);
	double b5=random_normal(1.);
	return PT(a0+a1*x+a6*sy,b0+b2*y+b3*cx+b4*sx+b5*cy);
}

iterfunc randfunc_pgg(symgroup sg)
{
	int a1=random_sign(), b2=random_sign();
	double a0=random_torsion(2), a6=random_normal(1.);
	double b0=random_torsion(2), b4=random_normal(1.);
	if(random_bool()) {
		return PT(a0+a1*x+a6*sy,b0+b2*y+b4*sx);
	}
	else {
		return PT(a0+a1*y+a6*sx,b0+b2*x+b4*sy);
	}
}

iterfunc randfunc_p4(symgroup sg)
{
	int a1, a2;
	do {
		a1=random_range_inclusive(-1,1);
		a2=random_range_inclusive(-1,1);
	} while(a1*a1+a2*a2>1);
	double a0=random_torsion(2), a5=random_normal(1.), a6=random_normal(1.);
	return PT(a0+a1*x+a2*y+a5*sx+a6*sy,a0-a2*x+a1*y-a6*sx+a5*sy);
}


iterfunc random_iterfunc(symgroup sg)
{
	switch(sg)
	{
	case SYM_CM:
		return randfunc_cm(sg);
	case SYM_CMM:
		return randfunc_cmm(sg);
	case SYM_P2:
		return randfunc_p2(sg);
	case SYM_P4:
		return randfunc_p4(sg);
	case SYM_PG:
		return randfunc_pg(sg);
	case SYM_PGG:
		return randfunc_pgg(sg);
	case SYM_PM:
		return randfunc_pm(sg);
	default:
		return randfunc_p1(sg);
	}
}

function<double(double,double)> distfunc(symgroup sg)
{
	switch(sg)
	{
	case SYM_PG:
	case SYM_PGG:
		return [] (double x, double y) { return cos(x+y)+cos(x-y); };
	default:
		return [] (double x, double y) { return cos(x)+cos(y); };
	}
}

using std::max;
#define PT(a,b) [=] (double x, double y) { double cx = cos(x), cy = cos(y), \
	sx = sin(x), sy = sin(y); return make_tuple((a),(b)); }

using std::tie;
using std::function;

void drawtrap(symmetric_canvas<color_t> &c)
{
	canvas<color_t> &cc=c.unsafe_get_canvas();
	iterfunc f = random_iterfunc(c.group());
	auto d = distfunc(c.group());
	for(int yy=0;yy<c.size();yy++)
		for(int xx=0;xx<c.size();xx++) {
		double x = xx*2*M_PI/c.size(), y = yy*2*M_PI/c.size(), dm=-2;
		for(int i=0;i<100;i++) {
			tie(x,y)=f(x,y);
			dm = max(dm,d(x,y));
		}
		uint8_t v=colorchop(64*(dm+2));
		cc(xx,yy)=color_t(v,v,v);
	}
}
