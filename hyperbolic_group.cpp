/***************************************************************************
 *   Copyright (C) 2005-2008, 2013, 2016 by Daniel Gulotta                 *
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

#include <algorithm>
#include "randgen.h"
#include "hyperbolic_group.h"

using namespace std;
using std::placeholders::_1;

const double ZMAX = 50.;

hyperbolic_symmetry_group::hyperbolic_symmetry_group(const vector<generator> &gens,flip_type f)
: generators(gens)
{
	vector<hyperbolic_transformation> queue;
	queue.push_back(hyperbolic_transformation::identity);
	transformations.push_back(hyperbolic_transformation::identity);
	hyperbolic_coord ip = normalize(hyperbolic_coord(1,2,100));
	for(int i=0;i<queue.size();i++) {
		for(auto it = generators.begin();it!=generators.end();++it) {
			hyperbolic_transformation t = queue[i]*(it->trans);
			hyperbolic_coord p=t.inverse(ip);
			//if(p.z>ZMAX) continue;
			//if(any_of(generators.begin(),generators.end(),[&](generator &g) { return abs(t(g.edge).z)>ZMAX;})) continue;
			if(it->inside(p)) continue;
			if(abs(t.zz)>ZMAX) continue;
			if(!all_of(generators.begin(),it,bind(&generator::inside,_1,p))) continue;
			queue.push_back(t);
			if(f==flip_type::ALL||
					(f==flip_type::ALTERNATING&&!t.is_flip())||
					(f==flip_type::RANDOM&&random_bool()))
				transformations.push_back(t);
			else
				flips.push_back(t);
		}
	}
	switch(f) {
		case flip_type::ALL:
			fdfunc=&hyperbolic_symmetry_group::fundamental_domain;
			break;
		case flip_type::ALTERNATING:
			fdfunc=&hyperbolic_symmetry_group::fundamental_domain_alternating;
			break;
		case flip_type::RANDOM:
			fdfunc=&hyperbolic_symmetry_group::fundamental_domain_random;
	}
}

hyperbolic_coord hyperbolic_symmetry_group::random_symmetry(const hyperbolic_coord &c) const {
	double i=random_uniform();
	return transformations[pow(transformations.size(),i)](c);
}

hyperbolic_coord fundamental_domain_point(const vector<generator> &gens, hyperbolic_coord c)
{
	while(true) {
		auto it=find_if_not(gens.begin(),gens.end(),bind(&generator::inside,_1,c));
		if(it==gens.end())
			return c;
		c=it->trans(c);
	}
}

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain(const hyperbolic_coord &hc) const {
	return make_tuple(fundamental_domain_point(generators,hc),false);
}

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain_alternating(const hyperbolic_coord &hc) const {
	hyperbolic_coord c(hc);
	bool any=true;
	bool flip=false;
	while(any) {
		any=false;
		for(auto &t: generators) {
			if(!t.inside(c)) {
				c=t.trans(c);
				flip^=t.is_flip;
				any=true;
				break;
			}
		}
	}
	return make_tuple(c,flip);
}

tuple<hyperbolic_coord,bool> hyperbolic_symmetry_group::fundamental_domain_random(const hyperbolic_coord &hc) const {
	hyperbolic_coord c(hc);
	bool any=true;
	long index=0;
	while(any) {
		any=false;
		int i=0;
		for(auto &t: generators) {
			++i;
			if(!t.inside(c)) {
				c=t.trans(c);
				index=index*(generators.size()+1)+i;
				any=true;
				break;
			}
		}
	}
	std::lock_guard<std::mutex> lock(mutex);
	const auto &it = random_flips.find(index);
	if(it!=random_flips.end())
		return make_tuple(c,it->second);
	else {
		bool b = random_bool();
		random_flips.emplace(index,b);
		return make_tuple(c,b);
	}
}

fundamental_domain_family::edge_type fundamental_domain_family::type_of_edge(int i) const
{
	if(edges[i].opposite==i)
		if(edges[i].flip)
			return edge_type::MIRROR;
		else
			return edge_type::ROT180;
	else
		if(edges[i].flip)
			return edge_type::GLIDE;
		else
			return edge_type::ROT_TRANS;
}

void fundamental_domain_family::other_edge(int &e, const int &v) const
{
	e=(2*v-1+edges.size()-e)%edges.size();
}

void fundamental_domain_family::other_vertex(const int &e, int &v) const
{
	v=(2*e+1-v+edges.size())%edges.size();
}

void fundamental_domain_family::other_corner(int &e, int &v) const
{
	int eold=e;
	e=edges[eold].opposite;
	v=(e-eold+v+edges.size())%edges.size();
	if(!edges[eold].flip)
		other_vertex(e,v);
}

fundamental_domain_family::fundamental_domain_family(const vector<side> &e)
:edges(e)
{
	vector<uint8_t> vmark(edges.size(),0);
	vector<uint8_t> emark(edges.size(),0);
	for(int i=0;i<edges.size();i++) {
		if(!emark[i]&&type_of_edge(i)==edge_type::MIRROR) {
			mirrors.emplace_back();
			int e=i, v=i;
			while(!emark[e]) {
				emark[e]=1;
				mirrors.back().emplace_back();
				while(!vmark[v]) {
					vmark[v]=1;
					mirrors.back().back().push_back(v);
					other_edge(e,v);
					other_corner(e,v);
				}
				other_vertex(e,v);
			}
		}
	}
	for(int i=0;i<edges.size();i++) {
		if(!vmark[i]) {
			rotations.emplace_back();
			int e=i, v=i;
			while(!vmark[v]) {
				vmark[v]=1;
				rotations.back().push_back(v);
				other_edge(e,v);
				other_corner(e,v);
			}
		}
	}
}

int fundamental_domain_family::rotation_count() const
{
	return rotations.size();
}

vector<int> fundamental_domain_family::mirror_counts() const
{
	vector<int> n(mirrors.size());
	transform(mirrors.begin(),mirrors.end(),n.begin(),bind(&vector<vector<int>>::size,_1));
	return n;
}

vector<int> fundamental_domain_family::rotation_point_counts() const
{
	vector<int> n(rotations.size());
	transform(rotations.begin(),rotations.end(),n.begin(),bind(&vector<int>::size,_1));
	return n;
}

vector<vector<int>> fundamental_domain_family::mirror_point_counts() const
{
	vector<vector<int>> n(mirrors.size());
	for(int i=0;i<mirrors.size();i++) {
		n[i].resize(mirrors[i].size());
		transform(mirrors[i].begin(),mirrors[i].end(),n[i].begin(),bind(&vector<int>::size,_1));
	}
	return n;
}

int fundamental_domain_family::rot180s() const
{
	int n=0;
	for(int i=0;i<edges.size();i++)
		if(type_of_edge(i)==edge_type::ROT180) n++;
	return n;
}

bool fundamental_domain_family::orientable() const
{
	for(int i=0;i<edges.size();i++)
		if(type_of_edge(i)==edge_type::GLIDE) return false;
	return true;
}

int fundamental_domain_family::loops() const
{
	int e = edges.size();
	int v = rotations.size();
	for(int i=0;i<edges.size();i++)
		if(edges[i].opposite==i) {
			e++;
			if(!edges[i].flip)
				v++;
		}
	for(const auto &s : mirrors)
		v+=s.size();
	return 1+e/2-v-mirrors.size();
}

struct rational
{
	rational(int n, int d) : num(n), denom(d) {}
	rational(int n) : rational(n,1) {}
	rational() : rational(0) {}
	void operator += (const rational &r) { num = num*r.denom+denom*r.num; denom*=r.denom; }
	bool operator <= (int n) { return num<=denom*n; }
	int num, denom;
};

fundamental_domain fundamental_domain_family::domain(const vector<int> &rots) const
{
	if(edges.size()!=3&&edges.size()!=4)
		throw std::logic_error("Not implemented");
	vector<double> angles(edges.size());
	auto it = rots.cbegin();
	rational ext_angle_sum;
	for(auto &v: rotations) {
		int ang_frac = (*it)*v.size();
		ext_angle_sum += rational(ang_frac-2,(*it));
		double ang=2*M_PI/ang_frac;
		for(int j: v)
			angles[j]=ang;
		++it;
	}
	for(auto &m: mirrors)
		for(auto &v: m) {
			int ang_frac = (*it)*v.size();
			ext_angle_sum += rational(ang_frac-1,(*it));
			double ang=M_PI/ang_frac;
			for(int j : v)
				angles[j]=ang;
			++it;
		}
	if(ext_angle_sum<=2)
		throw std::domain_error("Bad parameters for hyperbolic group");
	hyperbolic_polygon p=(edges.size()==3)?
		hyperbolic_polygon(angles[0],angles[1],angles[2]):
		hyperbolic_polygon(angles[0],angles[1],angles[2],angles[3]);
	vector<generator> gens;
	for(int i=0;i<edges.size();i++)
		gens.emplace_back(p.trans(i,edges[i].opposite,edges[i].flip),p.edge(i));
	return fundamental_domain{gens,p,this};
}
