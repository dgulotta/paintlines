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
#include <complex>

using std::tie;
using std::tuple;
using std::function;
using std::make_tuple;
using std::complex;
using std::polar;

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

iterfunc randfunc_pmg(symgroup sg)
{
	if(random_bool()) {
		double a0=random_torsion(2), b0=random_torsion(2);
		int a1=random_sign(), b2=random_range_inclusive(-1,1);
		double a3=random_normal(1.), b3=random_normal(1.), b4=random_normal(1.);
		return PT(a0+a1*x+a3*cx*sy,b0+b2*y+b3*sx+b4*sy);
	}
	else {
		double a0=random_angle(), b0=random_angle();
		double a3=random_normal(1.), a4=random_normal(1.);
		double b3=random_normal(1.), b4=random_normal(1.);
		return PT(a0+a3*cy+a4*sx*sy,b0+b3*cy+b4*sx*sy);
	}
}

iterfunc randfunc_pmm(symgroup sg)
{
	if(random_bool()) {
		double a0=random_torsion(2), b0=random_torsion(2);
		int a1=random_range_inclusive(-1,1), b2=random_range_inclusive(-1,1);
		double a3=random_normal(1.), a4=random_normal(1.);
		double b3=random_normal(1.), b4=random_normal(1.);
		if(random_bool())
			return PT(a0+a1*x+a3*sx+a4*sx*cy,b0+b2*y+b3*sy+b4*sy*cx);
		else
			return PT(a0+a1*y+a3*sy+a4*sy*cx,b0+b2*x+b3*sx+b4*sx*cy);
	}
	else {
		double a0=random_angle(), b0=random_angle();
		double a3=random_normal(1.), a4=random_normal(1.), a5=random_normal(1.);
		double b3=random_normal(1.), b4=random_normal(1.), b5=random_normal(1.);
		return PT(a0+a3*cx+a4*cy+a5*cx*cy,b0+a3*cx+a4*cy+b5*cx*cy);
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

const int mat6[7][4] = {
	{ 0, 0, 0, 0 },
	{ 1, 0, 0, 1 },
	{ 0, 1,-1,-1 },
	{-1,-1, 1, 0 },
	{-1, 0, 0,-1 },
	{ 0, -1,1, 1 },
	{ 1, 1,-1, 0 },
};

const double sq1_3 = sqrt(1./3.);

iterfunc randfunc_p3(symgroup sg)
{
	int z=random_int(7);
	const int *m=mat6[z];
	double a0=random_torsion(3);
	double a3=random_normal(1./3.), a4=random_normal(sq1_3);
	double a5=random_normal(1./3.), a6=random_normal(sq1_3);
	auto tr = PT(a0+m[0]*x+m[1]*y
		+a3*2*sx+(a4-a3)*sy+(a4+a3)*(sx*cy+cx*sy)
		+a5*2*cx+(a6-a5)*cy+(a6+a5)*(sx*sy-cx*cy),
		a0+m[2]*x+m[3]*y
		+a3*2*sy-(a4+a3)*sx+(a3-a4)*(sx*cy+cx*sy)
		+a5*2*cy-(a6+a5)*cx+(a5-a6)*(sx*sy-cx*cy));
	if(random_bool()) {
		return tr;
	}
	else {
		return [=] (double x, double y) { return tr(y,x); };
	}
}

iterfunc randfunc_p3m1(symgroup sg)
{
	if(random_bool()) {
		double a0=random_torsion(3);
		int a1=random_range_inclusive(-1,1);
		double a3=random_normal(.5), a4=random_normal(.5), a5=random_normal(.5);
		double a6=random_normal(.5);
		function<tuple<double,double>(double,double)> tr;
		int z=random_int(6);
		switch(z) {
			case 0:
				tr = [] (double x, double y) { return make_tuple(x,y); };
			case 1:
				tr = [] (double x, double y) { return make_tuple(y,-x-y); };
			case 2:
				tr = [] (double x, double y) { return make_tuple(-x-y,x); };
			case 3:
				tr = [] (double x, double y) { return make_tuple(y,x); };
			case 4:
				tr = [] (double x, double y) { return make_tuple(x,-y-x); };
			default:
				tr = [] (double x, double y) { return make_tuple(-y-x,y); };
		}
		return [=] (double x, double y) {
			complex<double> ex=polar(1.,x), ey=polar(1.,y), ez=1./(ex*ey);
			complex<double> eyz=ey/ez, ezx=ez/ex, exy=ex/ey;
			complex<double> x1 = 2.*ex-ey-ez;
			complex<double> y1 = 2.*ey-ex-ez;
			return tr(a0+a1*x+a3*x1.real()+a4*x1.imag()+a5*(ezx-exy).imag()+a6*(2.*eyz-exy-ezx).real(),a0+a1*y+a3*y1.real()+a4*y1.imag()+a5*(exy-eyz).imag()+a6*(2.*ezx-exy-eyz).real());
		};
	}
	else {
		double a0 = random_angle(), b0=random_angle();
		double a3 = random_normal(1.), a4=random_normal(1.);
		double b3 = random_normal(1.), b4=random_normal(1.);
		return PT(a0+a3*(sx+sy-(sx*cy+cx*sy))+a4*(cx+cy+(cx*cy-sx*sy)),
				b0+b3*(sx+sy-(sx*cy+cx*sy))+b4*(cx+cy+(cx*cy-sx*sy)));
	}
}

iterfunc randfunc_p6(symgroup sg)
{
	int z=random_int(7);
	const int *m=mat6[z];
	double a3=random_normal(sq1_3), a4=random_normal(1);
	auto tr = PT(m[0]*x+m[1]*y+a3*2*sx+(a4-a3)*sy+(a4+a3)*(sx*cy+cx*sy),
		m[2]*x+m[3]*y+a3*2*sy-(a4+a3)*sx+(a3-a4)*(sx*cy+cx*sy));
	if(random_bool()) {
		return tr;
	}
	else {
		return [=] (double x, double y) { return tr(y,x); };
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
	if(random_bool())
		return PT(a0+a1*x+a2*y+a5*sx+a6*sy,a0-a2*x+a1*y-a6*sx+a5*sy);
	else
		return PT(a0-a1*x+a2*y-a5*sx+a6*sy,a0+a2*x+a1*y+a6*sx+a5*sy);
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
	case SYM_P3:
		return randfunc_p3(sg);
	case SYM_P3M1:
		return randfunc_p3m1(sg);
	case SYM_P4:
		return randfunc_p4(sg);
	case SYM_P6:
		return randfunc_p6(sg);
	case SYM_PG:
		return randfunc_pg(sg);
	case SYM_PGG:
		return randfunc_pgg(sg);
	case SYM_PM:
		return randfunc_pm(sg);
	case SYM_PMG:
		return randfunc_pmg(sg);
	case SYM_PMM:
		return randfunc_pmm(sg);
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
	case SYM_PMG:
		return [] (double x, double y) { return cos(x+y)-cos(x-y); };
	case SYM_P3:
	case SYM_P3M1:
	case SYM_P6:
		return [] (double x, double y) { return (cos(x)+cos(y)+cos(x+y)-.75)*(8./9.); };
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
