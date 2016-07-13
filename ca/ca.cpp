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

#include <utility>
#include "ca.h"
#include "../randgen.h"

using namespace std;

const rule rule::plumes(1,2,99,{4,5,6,12},{4,5,6,7,8});
const rule rule::fireworks(1,4,99,{6,7,8,9,10,20},{4,6,7,8,9,10,11,12,13});
const rule rule::bubbles3(4,1,7,{6,9},{6,7,8,9,10,11,12,13});
const rule rule::conway(1,1,1,{3},{2,3});
const rule rule::dustclouds(3,2,99,{6,7,8,9,10,20},{6,7,8,9,10,11,12,13});
const rule rule::seeds(1,1,1,{2},{});
const rule rule::coral(1,1,1,{3},{4,5,6,7,8});
const rule rule::diamoeba(1,1,1,{3,5,6,7,8},{5,6,7,8});
const rule rule::flakes(1,1,1,{3},{0,1,2,3,4,5,6,7,8});
const rule rule::bombers(1,1,24,{2,4},{3,4,5});
const rule rule::prairieonfire(1,1,5,{3,4},{3,4,5});
const rule rule::softfreeze(1,1,5,{3,8},{1,3,4,5,8});
const rule rule::wanderers(1,1,4,{3,5,6,7,8},{3,4,5});
const rule rule::banners(1,1,4,{3,4,5,7},{1,3,5,7});
const rule rule::sponge(4,1,99,{6,7,8,9,10,20},{6,7,8,9,10,11,12,13});
const rule rule::pyramids(2,3,99,{7,8,10},{6,7,8,9,10,11,12,13});
const rule rule::streaks(2,1,5,{3,4},{2,5});
const rule rule::beams(2,1,7,{4,5,6,7},{3,4,5,6});
const rule rule::tubes(2,1,5,{4,5,6,7,11},{3,4,5,7,8,10});
const rule rule::tendrils(2,3,3,{4,6,7,8,10},{5,6,8,10});
const rule rule::streets(1,5,9,{3,4,7,8,9,10,11,12,13,14,16,17,20,21},{2,3,4,6,8,9,10,11,12,13,15,16,17,21});
const rule rule::gnats(1,5,2,{2,3,4,7,10,11},{3,4,5,6,7});
const rule rule::spill(1,1,100,{3,6},{2,3,4});
const rule rule::polluters(1,1,4,{3,4,5,6},{3,4,5});
const rule rule::blob(1,1,1,{3,6,7},{4,5,6,7});
const rule rule::colony(1,1,1,{3,6,8},{4,5,6,7});
const rule rule::threefour(1,1,1,{3,4},{3,4});
const rule rule::ebbandflow(1,1,17,{3,6},{0,1,2,4,7,8});

void ca::set(int x, int y, int s)
{
	current->set(x,y,s);
	wakeup(x,y);
	wakeup_adjacent(x,y);
}

void ca::wakeup(int x, int y)
{
	if(!in_queue.get(x,y,1)) {
		awake.emplace_back(x,y);
		in_queue.set(x,y,1);
	}
}

void ca::wakeup_adjacent(int x, int y)
{
	wakeup(x-1,y-1);
	wakeup(x,y-1);
	wakeup(x+1,y-1);
	wakeup(x-1,y);
	wakeup(x+1,y);
	wakeup(x-1,y+1);
	wakeup(x,y+1);
	wakeup(x+1,y+1);
}

void ca::process(int x, int y)
{
	if(turn<next_on(x,y)) {
		current->set(x,y,0);
		return;
	}
	int s=therule.get_new_state(*old,x,y);
	current->set(x,y,s);
	if((!s)&&old->get(x,y)) {
		next_on(x,y)=turn+therule.get_dead_steps();
	}
	else if(s&&first_on.get(x,y)==0) {
		first_on.set(x,y,turn);
		wakeup_adjacent(x,y);
	}
}

void ca::run_until(int tstop)
{
	int x,y;
	for(;turn<tstop;turn++) {
		std::swap(current,old);
		for(int i=awake.size();--i>=0;) {
			tie(x,y) = awake[i];
			process(x,y);
		}
	}
}

canvas<color_t> ca::get_diff_image(double intensity)
{
	canvas<color_t> img(first_on.width(),first_on.height());
	for(int y=0;y<first_on.height();y++)
		for(int x=0;x<first_on.width();x++) {
			int xd=first_on.get_nearest(x+1,y-1)+2*first_on.get_nearest(x+1,y)+first_on.get_nearest(x+1,y+1)
				-first_on.get_nearest(x-1,y-1)-2*first_on.get_nearest(x-1,y)-first_on.get_nearest(x-1,y+1);
			int yd=first_on.get_nearest(x-1,y+1)+2*first_on.get_nearest(x,y+1)+first_on.get_nearest(x+1,y+1)
				-first_on.get_nearest(x-1,y-1)-2*first_on.get_nearest(x,y-1)-first_on.get_nearest(x+1,y-1);
			double md = sqrt((xd*xd+yd*yd)*intensity);
			uint8_t m = md>255?255:(uint8_t)md;
			img(x,y)=color_t(m,m,m);
		}
	return img;
}

void ca::random_rectangle(int l, int t, int r, int b)
{
	for(int y=t;y<=b;y++)
		for(int x=l;x<=r;x++)
			set(x,y,random_int(2));
}
