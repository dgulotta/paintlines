/***************************************************************************
 *   Copyright (C) 2014 by Daniel Gulotta                                  *
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

#ifndef _CA_H
#define _CA_H

#include <tuple>
#include <vector>
#include "canvas.hpp"
#include "color.hpp"

template<typename T>
class ca_canvas
{
public:
	ca_canvas(int w, int h) : _canvas(w,h,T()) {}
	T get(int x, int y, T d=T()) const { if(in_range(x,y)) return _canvas(x,y); else return d; }
	T get_nearest(int x, int y) const { fix(x,y); return _canvas(x,y); }
	void set(int x, int y, T s) { _canvas(x,y)=s; }
	int height() const { return _canvas.height(); }
	int width() const { return _canvas.width(); }
private:
	canvas<T> _canvas;
	bool in_range(int x, int y) const {
		return x>=0&&y>=0&&x<_canvas.width()&&y<_canvas.height();
	}
	void fix(int &x, int &y) const {
		if(x<0) x=0;
		else if(x>=_canvas.width()) x=_canvas.width()-1;
		if(y<0) y=0;
		else if(y>=_canvas.height()) y=_canvas.height()-1;
	}
};

class rule
{
public:
	rule(int c, int a, int d, const std::vector<int> &b, const std::vector<int> &s) : rule(c,a,d)
	{
		for(int i : b) { birth[i]=1; }
		for(int i : s) { survival[i]=1; }
	}
	int get_new_state(const ca_canvas<uint8_t> &c, int x, int y) {
		int sum=corner_weight*(c.get(x-1,y-1)+c.get(x+1,y-1)+c.get(x-1,y+1)+c.get(x+1,y+1))
			+adjacent_weight*(c.get(x,y-1)+c.get(x-1,y)+c.get(x+1,y)+c.get(x,y+1));
		if(c.get(x,y))
			return survival[sum];
		else
			return birth[sum];
	}
	int get_dead_steps() { return dead_steps; }
	static const rule plumes;
	static const rule fireworks;
	static const rule bubbles3;
	static const rule conway;
	static const rule dustclouds;
	static const rule seeds;
	static const rule coral;
	static const rule diamoeba;
	static const rule flakes;
	static const rule bombers;
	static const rule prairieonfire;
	static const rule softfreeze;
	static const rule wanderers;
	static const rule banners;
	static const rule sponge;
	static const rule pyramids;
	static const rule streaks;
	static const rule beams;
	static const rule tubes;
	static const rule tendrils;
	static const rule streets;
	static const rule gnats;
	static const rule spill;
	static const rule polluters;
	static const rule blob;
	static const rule colony;
	static const rule threefour;
	static const rule ebbandflow;
private:
	rule(int c, int a, int d)
		: corner_weight(c), adjacent_weight(a), dead_steps(d),
		birth(1+4*(c+a),0), survival(1+4*(c+a),0) {}
	int corner_weight;
	int adjacent_weight;
	int dead_steps;
	std::vector<char> birth;
	std::vector<char> survival;
};

class ca
{
public:
	ca(int w, int h, const rule &r) : current(w,h), old(w,h),
		in_queue(w,h), first_on(w,h), next_on(w,h), turn(1), therule(r) {}
	void set(int x, int y, int s);
	void run_until(int tstop);
	void run_for(int nturns) { run_until(turn+nturns); }
	canvas<color_t> get_diff_image(double intensity);
	void random_rectangle(int l, int t, int r, int b);
private:
	void wakeup(int x, int y);
	void wakeup_adjacent(int x, int y);
	int get_state(int x, int y);
	void process(int x, int y);
	ca_canvas<uint8_t> current;
	ca_canvas<uint8_t> old;
	ca_canvas<uint8_t> in_queue;
	ca_canvas<int> first_on;
	canvas<int> next_on;
	std::vector<std::tuple<int,int>> awake;
	int turn;
	rule therule;
};

#endif
