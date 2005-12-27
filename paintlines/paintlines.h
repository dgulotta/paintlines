/***************************************************************************
 *   Copyright (C) 2003-2005 by Daniel Gulotta                             *
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

#ifndef _PAINTLINES_H
#define _PAINTLINES_H

#include "../painter.h"

void randomnormal(double &x, double &y, double var);
void randomcauchy(double &x, double &y, double var);

enum ruletype {RULE_SMOOTH_ARC, RULE_SMOOTHLINE2_BEADS, RULE_CLUSTER,
	       RULE_CLUSTER2, RULE_SMOOTHLINE2, RULE_OPEN_STRING};

struct paintrule
{
  ruletype type;
  bool pastel;
  int weight;
  bool operator < (int n) {return weight<n;}
};

class paintlines : virtual public painter
{
 public:
  paintlines() : ncolors(0) {}
  ~paintlines() {}
  void paint(int sz, symgroup sym);
  void set_rules(const vector<paintrule> &r) {
    rules=r;
    int i;
    for(i=1;i<rules.size();i++)
      rules[i].weight+=rules[i-1].weight;
  }
  void set_ruletype(int n, ruletype r) {rules[n].type=r;}
  void set_weight(int n, int w) {rules[n].weight=w;}
  void set_pastel(int n, bool b) {rules[n].pastel=b;}
  void handle_rule(ruletype rt);
  const vector<unsigned char> & get_red() {return red;}
  const vector<unsigned char> & get_green() {return green;}
  const vector<unsigned char> & get_blue() {return blue;}
  symgroup get_symgroup() {return sg;}
  void set_ncolors(int n) {ncolors=n;}
 private:
  vector<unsigned char> red_brushes;
  vector<unsigned char> green_brushes;
  vector<unsigned char> blue_brushes;
  vector<int> last;
  vector<unsigned char> alpha;
  vector<bool> pastel;
  vector<paintrule> rules;
  int t;
  int ncolors;
  unsigned char tempalpha;
  void (painter::*drawfunc)(paintlines &,void (paintlines::*)(int,int),
			    int,int);
  void drawpixel(int x, int y);
  inline void drawpixelsymmetric(int x, int y, unsigned char myalpha) {
    tempalpha=myalpha;
    (((painter *)this)->*drawfunc)(*this,&paintlines::drawpixel,x,y);
  }
  void drawdotsymmetric(int x, int y, int radius, double brightness);
  void drawcluster(double x, double y, double var, int maxdepth);
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
  void drawsmoothline2(double x1, double y1, double x2, double y2,
		       double var, double dist);
  void drawsmoothline3(double x1, double y1, double x2, double y2,
		       double var, double dist);
  void drawsmoothline4(double x1, double y1, double x2, double y2,
		       double var, double dist);
  void drawsmootharc(double x1, double y1, double x2, double y2, double k,
		     double var, double dist);
  void drawtriangle(double x1, double y1, double x2, double y2, double x3,
		    double y3, double var, double dist);
};

#endif
