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
using std::vector;

#define PT(a,b) [=] (double x, double y) { double cx = cos(x), cy = cos(y), \
	sx = sin(x), sy = sin(y); return make_tuple((a),(b)); }

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

const vector<transformation<double>> p3m1_trans = transformation<double>::group(symgroup::P3M1,0.);
const vector<transformation<double>> p4m_trans = transformation<double>::group(symgroup::P4M,0.);

iterfunc randfunc_p31m(symgroup sg)
{
	if(random_bool()) {
		int a1=random_range_inclusive(-1,1);
		double a3=random_normal(.5), a4=random_normal(.5), a5=random_normal(.5), a6=random_normal(.5);
		auto tr = random_choice(p3m1_trans);
		return [=] (double x, double y) {
			complex<double> ex=polar(1.,x), ey=polar(1.,y), ez=1./(ex*ey);
			complex<double> eyz=ey/ez, ezx=ez/ex, exy=ex/ey;
			complex<double> x1=(ezx-exy), y1=(exy-eyz);
			return tr(make_tuple(a1*x+a3*(ey-ez).real()+a4*(2.*ex-ey-ez).imag()+a5*x1.real()+a6*x1.imag(),
					a1*y+a3*(ez-ex).real()+a4*(2.*ey-ex-ez).imag()+a5*y1.real()+a6*y1.imag()));
		};
	}
	else {
		double a0=random_angle(), b0=random_angle();
		double a3=random_normal(1.), a4=random_normal(1.), a5=random_normal(1.);
		double b3=random_normal(1.), b4=random_normal(1.), b5=random_normal(1.);
		return [=] (double x, double y) {
			complex<double> ex=polar(1.,x), ey=polar(1.,y), ez=1./(ex*ey);
			complex<double> eyz=ey/ez, ezx=ez/ex, exy=ex/ey;
			complex<double> s=eyz+ezx+exy;
			double r = ex.real()+ey.real()+ez.real();
			return make_tuple(a0+a3*r+a4*s.real()+a5*s.imag(),b0+b3*r+a4*s.real()+a5*s.imag());
		};
	}
}

iterfunc randfunc_p3m1(symgroup sg)
{
	if(random_bool()) {
		double a0=random_torsion(3);
		int a1=random_range_inclusive(-1,1);
		double a3=random_normal(.5), a4=random_normal(.5), a5=random_normal(.5);
		double a6=random_normal(.5);
		auto tr=random_choice(p3m1_trans);
		return [=] (double x, double y) {
			complex<double> ex=polar(1.,x), ey=polar(1.,y), ez=1./(ex*ey);
			complex<double> eyz=ey/ez, ezx=ez/ex, exy=ex/ey;
			complex<double> x1 = 2.*ex-ey-ez;
			complex<double> y1 = 2.*ey-ex-ez;
			return tr(make_tuple(a0+a1*x+a3*x1.real()+a4*x1.imag()+a5*(ezx-exy).imag()+a6*(2.*eyz-exy-ezx).real(),a0+a1*y+a3*y1.real()+a4*y1.imag()+a5*(exy-eyz).imag()+a6*(2.*ezx-exy-eyz).real()));
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

iterfunc randfunc_p6m(symgroup sg) {
	if(random_bool()) {
		int a1=random_range_inclusive(-1,1);
		double a3=random_normal(1.), a4=random_normal(1.);
		auto tr=random_choice(p3m1_trans);
		return [=] (double x, double y) {
			complex<double> ex=polar(1.,x), ey=polar(1.,y), ez=1./(ex*ey);
			complex<double> eyz=ey/ez, ezx=ez/ex, exy=ex/ey;
			return tr(make_tuple(a1*x+a3*(2*ex.imag()-ey.imag()-ez.imag())+a4*(ezx.imag()-exy.imag()),
						a1*y+a3*(2*ey.imag()-ex.imag()-ez.imag())+a4*(exy.imag()-eyz.imag())));
		};
	}
	else {
		double a0=random_angle(), b0=random_angle();
		double a3=random_normal(1.), a4=random_normal(1.);
		double b3=random_normal(1.), b4=random_normal(1.);
		return [=] (double x, double y) {
			complex<double> ex=polar(1.,x), ey=polar(1.,y), ez=1./(ex*ey);
			complex<double> eyz=ey/ez, ezx=ez/ex, exy=ex/ey;
			double r1 = ex.real()+ey.real()+ez.real();
			double r2 = eyz.real()+ezx.real()+exy.real();
			return make_tuple(a0+a3*r1+a4*r2,b0+b3*r1+b4*r2);
		};
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

iterfunc randfunc_p4m(symgroup sg)
{
	if(random_bool()) {
		auto tr = random_choice(p4m_trans);
		double a0=random_torsion(2);
		int a1=random_range_inclusive(-1,1);
		double a3=random_normal(1.), a4=random_normal(1.), a5=random_normal(1.);
		auto fn = PT(a0+a1*x+a3*sx+a4*sx*cy+a5*sx*cx,a0+a1*y+a3*sy+a4*sy*cx+a5*sy*cy);
		return [=] (double x, double y) { return tr(fn(x,y)); };
	}
	else {
		double a0=random_angle(), b0=random_angle();
		double a3=random_normal(1.), a4=random_normal(1.);
		double b3=random_normal(1.), b4=random_normal(1.);
		return PT(a0+a3*(cx+cy)+a4*cx*cy,b0+b3*(cx+cy)+b4*cx*cy);
	}
}

iterfunc randfunc_p4g(symgroup sg)
{
	double a0=random_torsion(2);
	int a1=random_sign();
	double a3=random_normal(1.), a4=random_normal(1.), a5=random_normal(1.);
	auto tr = random_choice(p4m_trans);
	auto fn = PT(a0+a1*x+a3*sy+a4*sx*cx+a5*sx*cy,a0+a1*y-a3*sx+a4*sy*cy+a5*sy*cx);
	return [=] (double x, double y) { return tr(fn(x,y)); };
}

iterfunc random_iterfunc(symgroup sg)
{
	switch(sg)
	{
	case symgroup::CM:
		return randfunc_cm(sg);
	case symgroup::CMM:
		return randfunc_cmm(sg);
	case symgroup::P2:
		return randfunc_p2(sg);
	case symgroup::P3:
		return randfunc_p3(sg);
	case symgroup::P31M:
		return randfunc_p31m(sg);
	case symgroup::P3M1:
		return randfunc_p3m1(sg);
	case symgroup::P4:
		return randfunc_p4(sg);
	case symgroup::P4G:
		return randfunc_p4g(sg);
	case symgroup::P4M:
		return randfunc_p4m(sg);
	case symgroup::P6:
		return randfunc_p6(sg);
	case symgroup::P6M:
		return randfunc_p6m(sg);
	case symgroup::PG:
		return randfunc_pg(sg);
	case symgroup::PGG:
		return randfunc_pgg(sg);
	case symgroup::PM:
		return randfunc_pm(sg);
	case symgroup::PMG:
		return randfunc_pmg(sg);
	case symgroup::PMM:
		return randfunc_pmm(sg);
	default:
		return randfunc_p1(sg);
	}
}

function<double(double,double)> distfunc(symgroup sg)
{
	switch(sg)
	{
	case symgroup::PG:
	case symgroup::PGG:
	case symgroup::P4G:
		return [] (double x, double y) { return cos(x+y)+cos(x-y); };
	case symgroup::PMG:
		return [] (double x, double y) { return cos(x+y)-cos(x-y); };
	case symgroup::P3:
	case symgroup::P31M:
	case symgroup::P3M1:
	case symgroup::P6:
	case symgroup::P6M:
		return [] (double x, double y) { return (cos(x)+cos(y)+cos(x+y)-.75)*(8./9.); };
	default:
		return [] (double x, double y) { return cos(x)+cos(y); };
	}
}

using std::max;

void drawtrap(symmetric_canvas<color_t> &c)
{
	canvas<color_t> &cc=c.unsafe_get_canvas();
	iterfunc f = random_iterfunc(c.group());
	auto d = distfunc(c.group());
	for(int yy=0;yy<c.size();yy++)
		for(int xx=0;xx<c.size();xx++) {
		double x = xx*2*M_PI/c.size(), y = yy*2*M_PI/c.size(), dm=-2;
		for(int i=0;i<20;i++) {
			tie(x,y)=f(x,y);
			if(i>=5)
				dm = max(dm,d(x,y));
		}
		uint8_t v=colorchop(64*(dm+2));
		cc(xx,yy)=color_t(v,v,v);
	}
}

typedef complex<double> cx;
typedef std::function<tuple<cx,cx,cx,cx>(cx,cx,cx,cx)> qpiterfunc;
typedef std::function<double(cx,cx,cx,cx)> qpdistfunc;

template<typename T>
complex<T> ei(const T& q)
{
	return polar((T)1,q);
}

typedef std::function<tuple<double,double,double,double>(double,double,double,double)> qiterfunc;
typedef std::function<double(double, double, double, double)> qdistfunc;
typedef std::function<tuple<double,double,double,double>(double,double)> qembfunc;
using std::conj;

#define QPT(a,b,c,d) [] (double x, double y, double z, double w) { return make_tuple((a),(b),(c),(d)); }

qiterfunc qrot10[10] = {
	QPT(x,y,z,w),
	QPT(-y,-z,-w,x+y+z+w),
	QPT(z,w,-x-y-z-w,x),
	QPT(-w,x+y+z+w,-x,-y),
	QPT(-x-y-z-w,x,y,z),
	QPT(-x,-y,-z,-w),
	QPT(y,z,w,-x-y-z-w),
	QPT(-z,-w,x+y+z+w,-x),
	QPT(w,-x-y-z-w,x,y),
	QPT(x+y+z+w,-x,-y,-z),
};

qiterfunc qrot8[8] = {
	QPT(x,y,z,w),
	QPT(y,z,w,-x),
	QPT(z,w,-x,-y),
	QPT(w,-x,-y,-z),
	QPT(-x,-y,-z,-w),
	QPT(-y,-z,-w,x),
	QPT(-z,-w,x,y),
	QPT(-w,x,y,z),
};

qiterfunc qrot12[12] = {
	QPT(x,y,z,w),
	QPT(y,z,w,z-x),
	QPT(z,w,z-x,w-y),
	QPT(w,z-x,w-y,-x),
	QPT(z-x,w-y,-x,-y),
	QPT(w-y,-x,-y,-z),
	QPT(-x,-y,-z,-w),
	QPT(-y,-z,-w,x-z),
	QPT(-z,-w,x-z,y-w),
	QPT(-w,x-z,y-w,x),
	QPT(x-z,y-w,x,y),
	QPT(y-w,x,y,z),
};

qiterfunc random_qiterfunc5()
{
	double a0=random_torsion(5);
	int z=random_int(11);
	qiterfunc a1 = z<10 ? qrot10[z] : qiterfunc();
	complex<double> a3(random_normal(.2),random_normal(.2));
	complex<double> a4(random_normal(.2),random_normal(.2));
	complex<double> a5(random_normal(.2),random_normal(.2));
	complex<double> a6(random_normal(.2),random_normal(.2));
	complex<double> a7(random_normal(.2),random_normal(.2));
	int flip = random_int(4);
	return [=] (double x, double y, double z, double w) {
		complex<double> ex = polar(1.,x), ey=polar(1.,y), ez=polar(1.,z);
		complex<double> ew = polar(1.,w), ev=conj(ex*ey*ez*ew);
		double xn = a0 + (a3*(ex-ey)+a4*(ey-ez)+a5*(ez-ew)+a6*(ew-ev)+a7*(ev-ex)).real();
		double yn = a0 + (a7*(ex-ey)+a3*(ey-ez)+a4*(ez-ew)+a5*(ew-ev)+a6*(ev-ex)).real();
		double zn = a0 + (a6*(ex-ey)+a7*(ey-ez)+a3*(ez-ew)+a4*(ew-ev)+a5*(ev-ex)).real();
		double wn = a0 + (a5*(ex-ey)+a6*(ey-ez)+a7*(ez-ew)+a3*(ew-ev)+a4*(ev-ex)).real();
		if(a1) {
			double dx, dy, dz, dw;
			tie(dx,dy,dz,dw)=a1(x,y,z,w);
			xn+=dx; yn+=dy; zn+=dz; wn+=dw;
		}
		switch(flip) {
			case 0: return make_tuple(xn,yn,zn,wn);
			case 1: return make_tuple(yn,wn,xn,zn);
			case 2: return make_tuple(zn,xn,wn,yn);
			default: return make_tuple(wn,zn,yn,xn);
		}
	};
}

qiterfunc random_qiterfunc10()
{
	int z=random_int(11);
	qiterfunc a1 = z<10 ? qrot10[z] : qiterfunc();
	double a3=random_normal(.5), a4=random_normal(.5);
	double a5=random_normal(.5), a6=random_normal(.5);
	int flip = random_int(4);
	return [=] (double x, double y, double z, double w) {
		double sx=sin(x), sy=sin(y), sz=sin(z), sw=sin(w), sv=-sin(x+y+z+w);
		double xn=a3*(sx-sy)+a4*(sy-sz)+a5*(sz-sw)+a6*(sw-sv);
		double yn=a3*(sy-sz)+a4*(sz-sw)+a5*(sw-sv)+a6*(sv-sx);
		double zn=a3*(sz-sw)+a4*(sw-sv)+a5*(sv-sx)+a6*(sx-sy);
		double wn=a3*(sw-sv)+a4*(sv-sx)+a5*(sx-sy)+a6*(sy-sz);
		if(a1) {
			double dx, dy, dz, dw;
			tie(dx,dy,dz,dw)=a1(x,y,z,w);
			xn+=dx; yn+=dy; zn+=dz; wn+=dw;
		}
		switch(flip) {
			case 0: return make_tuple(xn,yn,zn,wn);
			case 1: return make_tuple(yn,wn,xn,zn);
			case 2: return make_tuple(zn,xn,wn,yn);
			default: return make_tuple(wn,zn,yn,xn);
		}
	};
}

qiterfunc random_qiterfunc8()
{
	double a0=random_torsion(2);
	int z=random_int(9);
	qiterfunc a1 = z<8 ? qrot8[z] : qiterfunc();
	int flip=random_int(4);
	double a3=random_normal(.75), a4=random_normal(.75);
	double a5=random_normal(.75), a6=random_normal(.75);
	return [=] (double x, double y, double z, double w) {
		double sx=sin(x), sy=sin(y), sz=sin(z), sw=sin(w);
		double xn = a0+a3*sx+a4*sy+a5*sz+a6*sw;
		double yn = a0+a3*sy+a4*sz+a5*sw-a6*sx;
		double zn = a0+a3*sz+a4*sw-a5*sx-a6*sy;
		double wn = a0+a3*sw-a4*sx-a5*sy-a6*sz;
		if(a1) {
			double dx, dy, dz, dw;
			tie(dx,dy,dz,dw)=a1(x,y,z,w);
			xn+=dx; yn+=dy; zn+=dz; wn+=dw;
		}
		switch(flip) {
			case 0: return make_tuple(xn,yn,zn,wn);
			case 1: return make_tuple(xn,wn,-zn,yn);
			case 2: return make_tuple(xn,-yn,zn,-wn);
			default: return make_tuple(xn,-wn,-zn,-yn);
		}
	};
}

qiterfunc random_qiterfunc12()
{
	int z=random_int(13);
	qiterfunc a1 = z<12 ? qrot12[z] : qiterfunc();
	int flip=random_int(4);
	double a3=random_normal(.25), a4=random_normal(.25);
	double a5=random_normal(.25), a6=random_normal(.25);
	return [=] (double x, double y, double z, double w) {
		double sx=sin(x), sy=sin(y), sz=sin(z), sw=sin(w);
		double szx=sin(z-x), swy=sin(w-y);
		double xn=a3*(sx+sz)+a4*(sy+sw)+a5*(sz+szx)+a6*(sw+swy);
		double yn=a3*(sy+sw)+a4*(sz+szx)+a5*(sw+swy)+a6*(szx-sx);
		double zn=a3*(sz+szx)+a4*(sw+swy)+a5*(szx-sx)+a6*(swy-sy);
		double wn=a3*(sw+swy)+a4*(szx-sx)+a5*(swy-sy)+a6*(-sx-sz);
		if(a1) {
			double dx, dy, dz, dw;
			tie(dx,dy,dz,dw)=a1(x,y,z,w);
			xn+=dx; yn+=dy; zn+=dz; wn+=dw;
		}
		switch(flip) {
			case 0: return make_tuple(xn,yn,zn,wn);
			case 1: return make_tuple(xn,wn-yn,xn-zn,wn);
			case 2: return make_tuple(xn,-yn,zn,-wn);
			default: return make_tuple(xn,yn-wn,xn-zn,-wn);
		}
	};
}

double qdistfunc5(double x, double y, double z, double w) {
	return (cos(x)+cos(y)+cos(z)+cos(w)+cos(x+y+z+w))/5;
}

double qdistfunc8(double x, double y, double z, double w) {
	return (cos(x)+cos(y)+cos(z)+cos(w))/4;
}

double qdistfunc12(double x, double y, double z, double w) {
	return (cos(x)+cos(y)+cos(z)+cos(w)+cos(x-z)+cos(y-w))/6;
}

tuple<double,double,double,double> qembfunc5(double x, double y) {
	return make_tuple(
		0.30901699437494745*x+0.9510565162951535*y,
		-0.8090169943749473*x+0.5877852522924732*y,
		-0.8090169943749473*x-0.5877852522924732*y,
		0.30901699437494745*x-0.9510565162951535*y);
}

tuple<double,double,double,double> qembfunc8(double x, double y) {
	return make_tuple(x,M_SQRT1_2*(x+y),y,M_SQRT1_2*(y-x));
}

const double sq3_2 = sqrt(.75);

tuple<double,double,double,double> qembfunc12(double x, double y) {
	return make_tuple(x,sq3_2*x+.5*y,.5*x+sq3_2*y,y);
}

double qpdistfunc5(cx x, cx y, cx z, cx w) {
	return (x.real()+y.real()+z.real()+w.real()+(x*y*z*w).real())/5;
}

double qpdistfunc8(cx x, cx y, cx z, cx w) {
	return (x.real()+y.real()+z.real()+w.real())/4;
}

double qpdistfunc12(cx x, cx y, cx z, cx w) {
	return (x.real()+y.real()+z.real()+w.real()+(x*conj(z)).real()+(y*conj(w)).real())/6;
}

cx rncplx(double r=1.)
{
	return cx(random_normal(r),random_normal(r));
}

qpiterfunc random_qpiterfunc8()
{
	double a0=random_normal(8.), a1=random_normal(1.), a2=random_normal(1.);
	double a3=random_normal(1.), a4=random_normal(1.), a5=random_normal(1.);
	double a6=random_normal(1.), a7=random_normal(1.), a8=random_normal(1.);
	int n = random_int(4);
	return [=] (cx x, cx y, cx z, cx w) {
		cx xc = conj(x), yc=conj(y), zc=conj(z), wc=conj(w);
		cx xn = a0+a1*x+a2*y+a3*z+a4*w+a5*xc+a6*yc+a7*zc+a8*wc;
		cx yn = a0+a8*x+a1*y+a2*z+a3*w+a4*xc+a5*yc+a6*zc+a7*wc;
		cx zn = a0+a7*x+a8*y+a1*z+a2*w+a3*xc+a4*yc+a5*zc+a6*wc;
		cx wn = a0+a6*x+a7*y+a8*z+a1*w+a2*xc+a3*yc+a4*zc+a5*wc;
		xn = x*xn/conj(xn); yn=y*yn/conj(yn); zn=z*zn/conj(zn); wn=w*wn/conj(wn);
		switch(n) {
		case 0:
			return make_tuple(xn,wn,conj(zn),yn);
		case 1:
			return make_tuple(xn,conj(yn),zn,conj(wn));
		case 2:
			return make_tuple(xn,conj(wn),conj(zn),conj(yn));
		default:
			return make_tuple(xn,yn,zn,wn);
		}
	};
}

qpiterfunc random_qpiterfunc5()
{
	cx a0=10.*rncplx(), a1=rncplx(), a2=rncplx();
	cx a3=rncplx(), a4=rncplx(), a5=rncplx();
	cx a6=rncplx(), a7=rncplx(), a8=rncplx();
	cx a9=rncplx(), a10=rncplx();
	return [=] (cx x, cx y, cx z, cx w) {
		cx xc=conj(x), yc=conj(y), zc=conj(z), wc=conj(w), vc=x*y*z*w, v=conj(vc);
		cx qn=a0+a1*x+a2*y+a3*z+a4*w+a5*v+a6*xc+a7*yc+a8*zc+a9*wc+a10*vc;
		cx rn=a0+a5*x+a1*y+a2*z+a3*w+a4*v+a10*xc+a6*yc+a7*zc+a8*wc+a9*vc;
		cx sn=a0+a4*x+a5*y+a1*z+a2*w+a3*v+a9*xc+a10*yc+a6*zc+a7*wc+a8*vc;
		cx tn=a0+a3*x+a4*y+a5*z+a1*w+a2*v+a8*xc+a9*yc+a10*zc+a6*wc+a7*vc;
		cx un=a0+a2*x+a3*y+a4*z+a5*w+a1*v+a7*xc+a8*yc+a9*zc+a10*wc+a6*vc;
		cx xn=qn/rn, yn=rn/sn, zn=sn/tn, wn=tn/un;
		return make_tuple(x*xn/conj(xn),y*yn/conj(yn),z*zn/conj(zn),w*wn/conj(wn));
	};
}

qpiterfunc random_qpiterfunc10()
{
	double a0=random_normal(10.), a1=random_normal(1.), a2=random_normal(1.);
	double a3=random_normal(1.), a4=random_normal(1.), a5=random_normal(1.);
	double a6=random_normal(1.), a7=random_normal(1.), a8=random_normal(1.);
	double a9=random_normal(1.), aa=random_normal(1.);
	return [=] (cx x, cx y, cx z, cx w) {
		cx xc=conj(x), yc=conj(y), zc=conj(z), wc=conj(w), vc=x*y*z*w, v=conj(vc);
		cx qn=a0+a1*x+a2*y+a3*z+a4*w+a5*v+a6*xc+a7*yc+a8*zc+a9*wc+aa*vc;
		cx rn=a0+a5*x+a1*y+a2*z+a3*w+a4*v+aa*xc+a6*yc+a7*zc+a8*wc+a9*vc;
		cx sn=a0+a4*x+a5*y+a1*z+a2*w+a3*v+a9*xc+aa*yc+a6*zc+a7*wc+a8*vc;
		cx tn=a0+a3*x+a4*y+a5*z+a1*w+a2*v+a8*xc+a9*yc+aa*zc+a6*wc+a7*vc;
		cx un=a0+a2*x+a3*y+a4*z+a5*w+a1*v+a7*xc+a8*yc+a9*zc+aa*wc+a6*vc;
		cx xn=qn/rn, yn=rn/sn, zn=sn/tn, wn=tn/un;
		return make_tuple(x*xn/conj(xn),y*yn/conj(yn),z*zn/conj(zn),w*wn/conj(wn));
	};
}

qpiterfunc random_qpiterfunc12()
{
	double a0=random_normal(12.);
	vector<double> a(12);
	generate(a.begin(),a.end(),[]() { return random_normal(1.); });
	return [=] (cx x, cx y, cx z, cx w) {
		cx v=z*conj(x), u=w*conj(y);
		cx c[12]={x,y,z,w,v,u,conj(x),conj(y),conj(z),conj(w),conj(v),conj(u)};
		vector<cx> m(8,0.);
		for(int i=0;i<8;i++) {
			m[i]+=a0;
			for(int j=0;j<12;j++)
				m[i]+=a[j]*c[(i+j)%12];

		}
		cx xn=m[0]/m[4], yn=m[1]/m[5], zn=m[2]/m[6], wn=m[3]/m[7];
		return make_tuple(x*xn/conj(xn),y*yn/conj(yn),z*zn/conj(zn),w*wn/conj(wn));
	};
}

void drawquasitrap_poly(canvas<color_t> &c, int symmetry, double quasiperiod)
{
	double xr=random_angle(), yr=random_angle(), zr=random_angle(), wr=random_angle();
	double fct=2.*M_PI/quasiperiod;
	qpiterfunc f;
	qembfunc e;
	qpdistfunc d;
	if(symmetry==8) {
		f = random_qpiterfunc8();
		e = qembfunc8;
		d = qpdistfunc8;
	}
	else if(symmetry==10) {
		f = random_qpiterfunc10();
		e = qembfunc5;
		d = qpdistfunc5;
	}
	else if(symmetry==12) {
		f = random_qpiterfunc12();
		e = qembfunc12;
		d = qpdistfunc12;
	}
	else {
		f = random_qpiterfunc5();
		e = qembfunc5;
		d = qpdistfunc5;
	}
	for(int yy=0;yy<c.height();yy++)
		for(int xx=0;xx<c.width();xx++) {
			double x,y,z,w;
			tie(x,y,z,w)=e(fct*xx,fct*yy);
			x+=xr; y+=yr; z+=zr; w+=wr;
			cx px=ei(x), py=ei(y), pz=ei(z), pw=ei(w);
			for(int i=0;i<5;i++)
				tie(px,py,pz,pw)=f(px,py,pz,pw);
			double dm = d(px,py,pz,pw);
			uint8_t v=colorchop(128*(dm+1));
			c(xx,yy)=color_t(v,v,v);
		}
}

void drawquasitrap(canvas<color_t> &c, int symmetry, double quasiperiod)
{
	double xr=random_angle(), yr=random_angle(), zr=random_angle(), wr=random_angle();
	double fct=2.*M_PI/quasiperiod;
	qiterfunc f;
	qembfunc e;
	qdistfunc d;
	if(symmetry==8) {
		f = random_qiterfunc8();
		e = qembfunc8;
		d = qdistfunc8;
	}
	else if(symmetry==10) {
		f = random_qiterfunc10();
		e = qembfunc5;
		d = qdistfunc5;
	}
	else if(symmetry==12) {
		f = random_qiterfunc12();
		e = qembfunc12;
		d = qdistfunc12;
	}
	else {
		f = random_qiterfunc5();
		e = qembfunc5;
		d = qdistfunc5;
	}
	for(int yy=0;yy<c.height();yy++)
		for(int xx=0;xx<c.width();xx++) {
			double x,y,z,w;
			tie(x,y,z,w)=e(fct*xx,fct*yy);
			x+=xr; y+=yr; z+=zr; w+=wr;
			for(int i=0;i<15;i++)
				tie(x,y,z,w)=f(x,y,z,w);
			double dm = d(x,y,z,w);
			uint8_t v=colorchop(128*(dm+1));
			c(xx,yy)=color_t(v,v,v);
		}
}
