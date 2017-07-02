/***************************************************************************
 *   Copyright (C) 2003-2005, 2013-2014, 2016 by Daniel Gulotta            *
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

#ifndef _PAINTLINES_H
#define _PAINTLINES_H

#include "symmetric_canvas.hpp"

void generate_smootharc(symmetric_canvas<uint8_t> &g);
void generate_smoothline2_beads(symmetric_canvas<uint8_t> &g);
void generate_smoothline2(symmetric_canvas<uint8_t> &g);
void generate_cluster(symmetric_canvas<uint8_t> &g);
void generate_flower(symmetric_canvas<uint8_t> &g);
void generate_cluster2(symmetric_canvas<uint8_t> &g);
void generate_open_string(symmetric_canvas<uint8_t> &g);
void generate_swirl(symmetric_canvas<uint8_t> &g);
void generate_orbit(symmetric_canvas<uint8_t> &g);
void generate_tree(symmetric_canvas<uint8_t> &g);
void generate_smoothline5(symmetric_canvas<uint8_t> &g);
void generate_granules(symmetric_canvas<uint8_t> &g);
void generate_star(symmetric_canvas<uint8_t> &g);
void generate_curl(symmetric_canvas<uint8_t> &g);

class paintlines_layer_generator
{
public:
	paintlines_layer_generator(symmetric_canvas<uint8_t> &g) : grid(g) {}
	int size() const { return grid.size(); }
	symgroup group() const { return grid.group(); }
	bool is_hexagonal() const { return sym_is_hexagonal(group()); }
	void drawsmootharc(double x1, double y1, double x2, double y2, double k,
		double var, double dist);
	void drawsmoothline2(double x1, double y1, double x2, double y2,
			double var, double dist);
	void drawcluster(double x, double y, double var, int maxdepth);
	void drawflower(double x, double y, int steps);
	void drawopenstring();
	void drawswirl();
	void draworbit();
	void drawtree();
	void drawgranules();
	void drawfractal(int x, int y, int d, int prob);
	void drawstar();
	void drawsmoothline5(double x1, double y1, double dx1, double dy1,
			double x2, double y2, double dx2, double dy2, double var, double dist);
	void drawdotsymmetric(int x, int y, int radius, double brightness);
private:
	symmetric_canvas<uint8_t> &grid;
	static void randomnormal_orthogonal(double &x, double &y, double var);
	static void randomnormal_hexagonal(double &x, double &y, double var);
	void randomnormal(double &x, double &y, double var) {
		if(is_hexagonal()) randomnormal_hexagonal(x,y,var);
		else randomnormal_orthogonal(x,y,var);
	}
	void drawpixelsymmetric(int x, int y, uint8_t alpha) { if(alpha>grid.get(x,y)) grid.set(x,y,alpha); }
	void drawcluster2(int x, int y, int d);
	void drawcluster3(int x, int y, int d);
	void drawcluster4(int x, int y, int d);
	void drawcluster5(int x, int y, int d);
	void drawcluster6(int x, int y, int d);
	void drawcluster7(int x, int y, int d);
	void drawcluster8(int x, int y, int d);
	void drawcluster9(int x, int y, int d);
	void drawcluster10(int x, int y, int d);
	void drawcluster11(double x, double y, double var, int maxdepth,
			double dist);
	void drawcluster12(int x, int y, int d, unsigned char myalpha);
	void drawcluster13(double x, double y, double var, int maxdepth);
	void drawcluster14(double x, double y, double var, int maxdepth);
	void drawcluster15(double x, double y, double m, double px, double py,
			int maxdepth);
	void drawcluster16(double x, double y, double x1, double y1,
			double x2, double y2, double x3, double y3,
			int depth);
	void drawcluster17(double x, double y, double x1, double y1,
			double x2, double y2, double x3, double y3,
			int depth);
	void drawcluster18(double x, double y, double x1, double y1,
			double x2, double y2, double x3, double y3,
			int depth);
	void drawcluster19(double x, double y, double x1, double y1,
			double x2, double y2, double x3, double y3,
			int depth);
	void drawcluster20(double x, double y, double x1, double y1,
			double x2, double y2, int depth);
	void drawline(double x1, double y1, double x2, double y2, double var,
			double dist);
	void drawsmoothline(double var, double steps);
	void drawsmoothline3(double x1, double y1, double x2, double y2,
			double var, double dist);
	void drawsmoothline4(double x1, double y1, double x2, double y2,
			double var, double dist);
	void drawtriangle(double x1, double y1, double x2, double y2, double x3,
			double y3, double var, double dist);
	void drawtree(double x, double y, double q, int depth);
	void drawtree_split(double x, double y, double q, int depth);
};

#endif
