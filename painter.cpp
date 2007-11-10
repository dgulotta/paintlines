/***************************************************************************
 *   Copyright (C) 2005-2007 by Daniel Gulotta                             *
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

#include "painter.h"
#include <iostream>

#define PAINTER_COPY_RGB r[index2]=red[index];g[index2]=green[index];\
b[index2]=blue[index];

#define PAINTER_NEW_COPY_RGB pt.copy(r,red); pt.copy(g,green); pt.copy(b,blue);

inline int gcd(int x, int y) {
  int t;
  while(y) {
    t=x%y;
    x=y;
    y=t;
  }
  return x;
}

void painter_transformation::operator () (int &x, int &y) const
{
  int xnew=xx*x+xy*y+x1;
  y=yx*x+yy*y+y1;
  x=xnew;
}

const painter_transformation pt_cmm_list[4] = 
  { { 1,0,0,0,1,0 }, { 0,1,0,1,0,0 }, { 0,-1,-1,-1,0,-1 }, {-1,0,-1,0,-1,-1}};

const painter_transformation pt_ident = {1,0,0,0,1,0};
const painter_transformation pt_hflip = {-1,0,-1,0,1,0};
const painter_transformation pt_vflip = {1,0,1,0,-1,-1};

void painter_transform::set_point(int x, int y) {
  int xnew=x, ynew=y;
  from(xnew,ynew);
  from_index=mod(xnew,size)+size*mod(ynew,size);
  xnew=x;
  ynew=y;
  to(xnew,ynew);
  to_index=mod(xnew,width)+width*mod(ynew,height);
}

/* This function is approaching a near-IOCCC level of unreadability.
 * I really should clean it up someday.
 */
void painter::randomize(int xtiles, int ytiles, vector<unsigned char> &r,
		 vector<unsigned char> &g, vector<unsigned char> &b)
{
  int width=xtiles*size;
  int height=ytiles*size;
  int i,j, k, l;
  int qsize=halfsize/2, sizeq1=size1+halfsize-qsize, sizesq=size*size;
  int bigsize=width*height;
  r.resize(bigsize);
  g.resize(bigsize);
  b.resize(bigsize);
  // eventually all groups should use this
  painter_transform pt(size,xtiles,ytiles);
  switch(sg) {
  case SYM_P1:
  case SYM_PM:
    {
      vector<int> ev(xtiles*ytiles,0xF), eh(xtiles*ytiles,0xF), *es, *ec;
      int ip, jp, z;
      k=0;
      l=0;
      es=&ev;
      while(l<ytiles) {
	i=k;
	j=l;
	z=(~0x4)&(rand()|~0x1);
	do {
	  ip=(i+1)%xtiles;
	  jp=(j+1)%ytiles;
	  if(!(ev[ip+xtiles*j]&0x4)) {
	    j=jp;
	    eh[i+xtiles*j]&=z;
	    ec=&eh;
	  }
	  else if((!(eh[i+xtiles*jp]&0x4))||rand()&1) {
	    i=ip;
	    ev[i+xtiles*j]&=z;
	    ec=&ev;
	  }
	  else {
	    j=jp;
	    eh[i+xtiles*j]&=z;
	    ec=&eh;
	  }
	} while(i!=k||j!=l||ec!=es);
	(*es)[k+xtiles*l]&=z;
	while(!((*es)[k+xtiles*l]&0x4)) {
	  k++;
	  if(k>=xtiles) {
	    k=0;
	    l++;
	  }
	  if(l>=ytiles) {
	    if(es==&ev) {
	      l=0;
	      es=&eh;
	    }
	    else break;
	  }
	}
      }
      k=0;
      l=0;
      es=&ev;
      while(l<ytiles) {
	i=k;
	j=l;
	z=(~0x8)&(rand()|~0x2);
	do { 
	  ip=(i+1)%xtiles;
	  jp=(j+1)%ytiles;
	  if(!((ev[ip+xtiles*j]&0x8)||(eh[i+xtiles*jp]&0x8))) {
	    exit(0);
	  }
	  if(!(ev[ip+xtiles*j]&0x8)) {
	    j=jp;
	    eh[i+xtiles*j]&=z;
	    ec=&eh;
	  }
	  else if((!(eh[i+xtiles*jp]&0x8))||rand()&1) {
	    i=ip;
	    ev[i+xtiles*j]&=z;
	    ec=&ev;
	  }
	  else {
	    j=jp;
	    eh[i+xtiles*j]&=z;
	    ec=&eh;
	  }
	} while(i!=k||j!=l||ec!=es);
	(*es)[k+xtiles*l]&=z;
	while(!((*es)[k+xtiles*l]&0x8)) {
	  k++;
	  if(k>=xtiles) {
	    k=0;
	    l++;
	  }
	  if(l>=ytiles) {
	    if(es==&ev) {
	      l=0;
	      es=&eh;
	    }
	    else break;
	  }
	}
      }
      int newi, newj;
      for(k=0;k<xtiles;k++) {
	for(l=0;l<ytiles;l++) {
	  int el=ev[k+l*xtiles], er=ev[(k+1)%xtiles+l*xtiles]^0x3;
	  int et=eh[k+l*xtiles]^0x3, eb=eh[k+((l+1)%ytiles)*xtiles];
	  //std::cout << el << ' ' << et << ' ' << er << ' ' << eb << std::endl;
	  if((el^et)==3) {
	    /* Although the tiling group doesn't eliminate this case, there
	     * isn't really a tile for it.  So we cheat and smash two tiles
	     * together.
	     *
	     * We can't use painter_transform for this one because it
	     * doesn't know about projective transformations.
	     */
	    bool z=rand()&1;
	    for(i=0;i<halfsize;i++)
	      for(j=i;i+j<size;j++) {
		newj=j-i;
		newi=2*i;
		if(z) pt_hflip(newi,newj);
		pt_cmm_list[el](newi,newj);
		int index=mod(newi,size)+size*mod(newj,size);
		int index2=i+k*size+width*(j+l*size);
		PAINTER_COPY_RGB;
		index2=(size1-j)+k*size+width*(size1-i+l*size);
		PAINTER_COPY_RGB;
		newj=j+i;
		newi=2*i;
		if(z) pt_hflip(newi,newj);
		pt_cmm_list[el](newi,newj);
		index=mod(newi,size)+size*mod(newj,size);
		index2=(size1-i)+k*size+width*(j+l*size);
		PAINTER_COPY_RGB;
		index2=(size1-j)+k*size+width*(i+l*size);
		PAINTER_COPY_RGB;
	      }
	  }
	  else if((el^eb)==3) {
	    bool z=rand()&1;
	    for(i=0;i<halfsize;i++)
	      for(j=i;i+j<size;j++) {
		newj=j+i;
		newi=2*i;
		if(z) pt_hflip(newi,newj);
		pt_cmm_list[el](newi,newj);
		int index=mod(newi,size)+size*mod(newj,size);
		int index2=i+k*size+width*(j+l*size);
		PAINTER_COPY_RGB;
		index2=j+k*size+width*(i+l*size);
		PAINTER_COPY_RGB;
		newj=j-i;
		newi=2*i;
		if(z) pt_hflip(newi,newj);
		pt_cmm_list[el](newi,newj);
		index=mod(newi,size)+size*mod(newj,size);
		index2=(size1-i)+k*size+width*(j+l*size);
		PAINTER_COPY_RGB;
		index2=j+k*size+width*(size1-i+l*size);
		PAINTER_COPY_RGB;
	      }
	  }
	  else if(el==er) {
	    const painter_transformation &flip=(rand()&1?pt_ident:pt_hflip);
	    for(i=0;i<halfsize;i++) {
	      for(j=i;i+j<size;j++) {
		newi=i;
		newj=j;
		flip(newi,newj);
		pt_cmm_list[el](newi,newj);
		int index=mod(newi,size)+size*mod(newj,size);
		int index2=i+k*size+width*(j+l*size);
		PAINTER_COPY_RGB;
		index2=j+k*size+width*(i+l*size);
		PAINTER_COPY_RGB;
		index2=(size1-i)+k*size+width*(size1-j+l*size);
		PAINTER_COPY_RGB;
		index2=(size1-j)+k*size+width*(size1-i+l*size);
		PAINTER_COPY_RGB;
	      }
	    }
	  }
	  else {
	    const painter_transformation &flip=(((el^et)==0x1||(el^eb)==0x2)?pt_ident:pt_hflip);
	    for(i=0;i<halfsize;i++) {
	      for(j=i;i+j<size;j++) {
		newi=i;
		newj=j;
		flip(newi,newj);
		pt_cmm_list[el](newi,newj);
		int index=mod(newi,size)+size*mod(newj,size);
		int index2=i+k*size+width*(j+l*size);
		PAINTER_COPY_RGB;
		newi=i;
		newj=j;
		flip(newi,newj);
		pt_cmm_list[et](newi,newj);
		index=mod(newi,size)+size*mod(newj,size);
		index2=j+k*size+width*(i+l*size);
		PAINTER_COPY_RGB;
		newi=i;
		newj=j;
		flip(newi,newj);
		pt_cmm_list[er](newi,newj);
		index=mod(newi,size)+size*mod(newj,size);
		index2=(size1-i)+k*size+width*((size1-j)+l*size);
		PAINTER_COPY_RGB;
		newi=i;
		newj=j;
		flip(newi,newj);
		pt_cmm_list[eb](newi,newj);
		index=mod(newi,size)+size*mod(newj,size);
		index2=(size1-j)+k*size+width*((size1-i)+l*size);
		PAINTER_COPY_RGB;
	      }
	    }
	  }
	}
      }
    }
    break;
  case SYM_CM:
    {
      vector<int> ev(xtiles*ytiles,0x5), eh(xtiles*ytiles,0x5), *es, *ec;
      int ip, jp, z;
      k=0;
      l=0;
      es=&ev;
      while(l<ytiles) {
	i=k;
	j=l;
	z=(~0x4)&(rand()|~0x1);
	do {
	  ip=(i+1)%xtiles;
	  jp=(j+1)%ytiles;
	  if(!(ev[ip+xtiles*j]&0x4)) {
	    j=jp;
	    eh[i+xtiles*j]&=z;
	    ec=&eh;
	  }
	  else if((!(eh[i+xtiles*jp]&0x4))||rand()&1) {
	    i=ip;
	    ev[i+xtiles*j]&=z;
	    ec=&ev;
	  }
	  else {
	    j=jp;
	    eh[i+xtiles*j]&=z;
	    ec=&eh;
	  }
	} while(i!=k||j!=l||ec!=es);
	(*es)[k+xtiles*l]&=z;
	while(!((*es)[k+xtiles*l]&0x4)) {
	  k++;
	  if(k>=xtiles) {
	    k=0;
	    l++;
	  }
	  if(l>=ytiles) {
	    if(es==&ev) {
	      l=0;
	      es=&eh;
	    }
	    else break;
	  }
	}
      }
      int newi, newj;
      for(k=0;k<xtiles;k++) {
	for(l=0;l<ytiles;l++) {
	  int el=ev[k+l*xtiles], er=ev[(k+1)%xtiles+l*xtiles]^0x1;
	  int et=eh[k+l*xtiles]^0x1, eb=eh[k+((l+1)%ytiles)*xtiles];
	  if(el==er) {
	    for(i=0;i<size;i++)
	      for(j=0;i+j<size;j++) {
		int index=el?(size1-j+size*(size1-i)):(i+size*j);
		int index2=i+k*size+width*(j+l*size);
		PAINTER_COPY_RGB;
		index2=size1-j+k*size+width*(size1-i+l*size);
		PAINTER_COPY_RGB;
	      }
	  }
	  else {
	    for(i=0;i<size;i++)
	      for(j=0;j<size;j++) {
		newi=i;
		newj=j;
		if(et) pt_hflip(newi,newj);
		if(el) pt_vflip(newi,newj);
		int index=mod(newi,size)+size*mod(newj,size);
		int index2=i+k*size+width*(j+l*size);
		PAINTER_COPY_RGB;
	      }
	  }
	}
      }
    }
    break;
  case SYM_PG:
    {
      int xt2=xtiles*2;
      int yt2=ytiles*2;
      int d=gcd(xt2,yt2);
      int m=(xt2*yt2)/d;
      vector<int> v(xt2*yt2,0x3);
      int z;
      for(k=0;k<d;k+=2) {
	z=~(rand()&0x1);
	for(l=0;l<m;l++) {
	  v[(k+l)%xt2+(l%yt2)*xt2]&=z;
	  z^=0x1;
	}
      }
      for(k=1;k<d;k+=2) {
	z=~(rand()&0x1);
	for(l=0;l<m;l++) {
	  v[mod(k-l,xt2)+(l%yt2)*xt2]&=z;
	  z^=0x1;
	}
      }
      int dk, dl;
      for(k=0,l=0;l<yt2;) {
	z=rand();
	dk=1-2*((l^v[k+l*xt2])&1);
	dl=1-2*((k^v[k+l*xt2])&1);
	while(v[k+l*xt2]&0x2) {
	  v[k+l*xt2]&=~0x2;
	  if((v[k+l*xt2]^v[mod(k+dk,xt2)+l*xt2])&0x1) {
	    pt.set_to_trans(0,dk,k*halfsize,-dl,0,(l+dl)*halfsize);
	    dk*=-1;
	    l=mod(l+dl,yt2);
	  }
	  else {
	    pt.set_to_trans(dk,0,k*halfsize,0,dl,l*halfsize);
	    dl*=-1;
	    k=mod(k+dk,xt2);
	  }
	  if(z&1) {
	    pt.set_from_trans(1,0,0,0,1,0);
	  }
	  else {
	    pt.set_from_trans(1,0,0,0,-1,halfsize1);
	  }
	  for(j=(-halfsize-1)/2;j<=(halfsize)/2;j++) {
	    for(i=-1-j;i-j<=halfsize;i++) {
	      pt.set_point(i,j);
	      PAINTER_NEW_COPY_RGB;
	    }
	  }
	}
	do {
	  k++;
	  if(k>=xt2) {
	    k=0;
	    l++;
	  }
	  if(l>yt2) break;
	} while(!(v[k+l*xt2]&0x2));
      }
    }
    break;
  case SYM_CMM:
  case SYM_P2:
    for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++)
	  for(j=0;j<halfsize;j++) {
	    int index=((z&1)?i:(size1-i))+size*j;
	    int index2=i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=((z&2)?i:(size1-i))+size*j;
	    index2=size1-i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=((z&4)?i:(size1-i))+size*j;
	    index2=i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB;
	    index=((z&8)?i:(size1-i))+size*j;
	    index2=size1-i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB;
	  }
      }
    break;
  case SYM_PMG:
    for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++)
	  for(j=0;j<halfsize;j++) {
	    int index=mod(((z&1)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    int index2=i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=mod(((z&2)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    index2=size1-i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=mod(((z&4)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    index2=i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB;
	    index=mod(((z&8)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    index2=size1-i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB;
	  }
      }
    break;
  case SYM_PGG:
     for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++)
	  for(j=0;j<halfsize;j++) {
	    int index=mod(((z&1)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    int index2=i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=mod(((z&2)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    index2=size1-i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=mod(((z&4)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    index2=i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB;
	    index=mod(((z&8)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    index2=size1-i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB;
	  }
      }
    break;
  case SYM_P4:
  case SYM_P4G:
    for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++)
	  for(j=0;j<=i;j++) {
	    int index=(z&0x1)?(i+size*j):(j+size*i);
	    int index2=i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&0x2)?(i+size*j):(j+size*i);
	    index2=j+k*size+width*(i+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&0x4)?(i+size*j):(j+size*i);
	    index2=size1-i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&0x8)?(i+size*j):(j+size*i);
	    index2=j+k*size+width*(size1-i+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&0x10)?(i+size*j):(j+size*i);
	    index2=size1-i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&0x20)?(i+size*j):(j+size*i);
	    index2=size1-j+k*size+width*(size1-i+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&0x40)?(i+size*j):(j+size*i);
	    index2=i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&0x80)?(i+size*j):(j+size*i);
	    index2=size1-j+k*size+width*(i+l*size);
	    PAINTER_COPY_RGB;
	  }
      }
    break;
  case SYM_PMM:
  case SYM_P4M:
    for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<halfsize;i++) {
	  int end=halfsize+i;
	  for(j=halfsize1-i;j<=end;j++) {
	    int index=(z&1)?(i+size*j):
	      mod((halfsize1-j)+size*(halfsize1-i),sizesq);
	    int index2=i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&1)?((size1-i)+size*j):
	      mod((halfsize1-j)+size*(i-halfsize),sizesq);
	    index2=(size1-i)+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB;
	    index=(z&2)?(i+size*j):
	      mod((halfsize1-j)+size*(halfsize1-i),sizesq);
	    index2=mod((halfsize1-j)+k*size,width)+width*
	      (mod((halfsize1-i)+l*size,height));
	    PAINTER_COPY_RGB;
	    index=(z&2)?((size1-i)+size*j):
	      mod((halfsize1-j)+size*(i-halfsize),sizesq);
	    index2=mod((halfsize1-j)+k*size,width)+width*
	      (mod((i-halfsize)+l*size,height));
	    PAINTER_COPY_RGB;
	  }
	}
      }
    break;
  case SYM_P3:
  case SYM_P31M:
  case SYM_P6:
    /* I think the other randomizer is more interesting.
     * Maybe eventually multiple randomizers per symmetry group
     * will be supported.
   for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i*3<size;i++) {
	  for(j=i;j<(size-i)/2;j++) {
	    int index=(z&4)?(i+size*j):(j+size*i);
	    int index2=((k+1)%xtiles)*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB;
	    index=(z&8)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(((l+1)%ytiles)*size+i);
	    PAINTER_COPY_RGB;
	  }
	  for(;j<size-2*i;j++) {
	    int index=(z&16)?(i+size*j):(j+size*i);
	    int index2=((k+1)%xtiles)*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB;
	    index=(z&32)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(((l+1)%ytiles)*size+i);
	    PAINTER_COPY_RGB;
	  }
	  for(;j<size-i/2;j++) {
	    int index=(z&1)?(i+size*j):(j+size*i);
	    int index2=k*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB;
	    index=(z&2)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(l*size+i);
	    PAINTER_COPY_RGB; 
	  }
	}
	for(;i*3<2*size;i++)
	  for(j=i;j<size-i/2;j++) {
	    int index=(z&1)?(i+size*j):(j+size*i);
	    int index2=k*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB;
	    index=(z&2)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(l*size+i);
	    PAINTER_COPY_RGB; 
	  }
	for(;i<size;i++) {
	  for(j=2*(size-i);j<size-i/2;j++) {
	    int index=(z&4)?(i+size*j):(j+size*i);
	    int index2=k*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB;
	    index=(z&8)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(l*size+i);
	    PAINTER_COPY_RGB; 
	  }
	  for(;j<=i;j++) {
	    int index=(z&16)?(i+size*j):(j+size*i);
	    int index2=k*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB;
	    index=(z&32)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(l*size+i);
	    PAINTER_COPY_RGB;
	  }
	}
      }
   break;
    */
  case SYM_P3M1:
  case SYM_P6M:
#define PAINTER_RANDOMIZE_P3M1_LOOP \
	  for(i=0;3*i<=size+2;i++) \
            for(j=0;3*(i+j)<=size+2;j++) { \
	      pt.set_point(i,j); \
	      PAINTER_NEW_COPY_RGB; \
	    }
    {
      int yt=ytiles*3;
      int ntiles=xtiles*yt;
      vector<unsigned char>types(ntiles);
      for(i=0;i<ntiles;i++)
	types[i]=rand()%3;
      for(k=0;k<xtiles;k++)
	for(l=0;l<ytiles;l++) {
	  int z=rand();
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k],types[3*l+yt*k+1],z&0x1);
	  pt.set_to_trans(1,0,size*k,0,1,size*l);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k+1],types[3*l+yt*k],z&0x1);
	  pt.set_to_trans(0,-1,size*k+(size+1)/3,-1,0,size*l+(size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k],types[3*l+yt*mod(k-1,xtiles)+2],z&0x2);
	  pt.set_to_trans(-1,0,size*k,1,1,size*l);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*mod(k-1,xtiles)+2],types[3*l+yt*k],z&0x2);
	  pt.set_to_trans(0,1,size*k-(size+1)/3,-1,-1,size*l+(2*size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k],types[3*l+yt*mod(k-1,xtiles)+1],z&0x4);
	  pt.set_to_trans(-1,-1,size*k,1,0,size*l);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*mod(k-1,xtiles)+1],types[3*l+yt*k],z&0x4);
	  pt.set_to_trans(1,1,size*k-(2*size+1)/3,0,-1,size*l+(size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k],types[3*mod(l-1,ytiles)+yt*mod(k-1,xtiles)+2],z&0x8);
	  pt.set_to_trans(0,-1,size*k,-1,0,size*l);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*mod(l-1,ytiles)+yt*mod(k-1,xtiles)+2],types[3*l+yt*k],z&0x8);
	  pt.set_to_trans(1,0,size*k-(size+1)/3,0,1,size*l-(size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k],types[3*mod(l-1,ytiles)+yt*k+1],z&0x10);
	  pt.set_to_trans(0,1,size*k,-1,-1,size*l);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*mod(l-1,ytiles)+yt*k+1],types[3*l+yt*k],z&0x10);
	  pt.set_to_trans(-1,0,size*k+(size+1)/3,1,1,size*l-(2*size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k],types[3*mod(l-1,ytiles)+yt*k+2],z&0x20);
	  pt.set_to_trans(1,1,size*k,0,-1,size*l);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*mod(l-1,ytiles)+yt*k+2],types[3*l+yt*k],z&0x20);
	  pt.set_to_trans(-1,-1,size*k+(2*size+1)/3,1,0,size*l-(size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k+1],types[3*l+yt*k+2],z&0x40);
	  pt.set_to_trans(1,0,size*k+(size+1)/3,0,1,size*l+(size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*k+2],types[3*l+yt*k+1],z&0x40);
	  pt.set_to_trans(0,-1,size*k+(2*size+1)/3,-1,0,size*l+(2*size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*mod(k-1,xtiles)+1],types[3*l+yt*mod(k-2,xtiles)+2],z&0x80);
	  pt.set_to_trans(-1,-1,size*k-(2*size+1)/3,1,0,size*l+(size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*l+yt*mod(k-2,xtiles)+2],types[3*l+yt*mod(k-1,xtiles)+1],z&0x80);
	  pt.set_to_trans(1,1,size*k-(4*size+1)/3,0,-1,size*l+(2*size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*mod(l-1,ytiles)+yt*k+1],types[3*mod(l-2,ytiles)+yt*k+2],z&0x100);
	  pt.set_to_trans(0,1,size*k+(size+1)/3,-1,-1,size*l-(2*size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	  randomize_p3m1_choose_from_trans
	    (pt,types[3*mod(l-2,ytiles)+yt*k+2],types[3*mod(l-1,ytiles)+yt*k+1],z&0x100);
	  pt.set_to_trans(-1,0,size*k+(2*size+1)/3,1,1,size*l-(4*size+1)/3);
	  PAINTER_RANDOMIZE_P3M1_LOOP;
	}
    }
  }
}

void painter::randomize_p3m1_choose_from_trans(painter_transform &pt, int mt,
					       int nt, int z)
{
  //std::cout << mt << ' ' << nt << std::endl;
  if(mt==0) {
    if((nt==0&&z)||nt==1) {
      pt.set_from_trans(1,0,0,0,1,0);
    }
    else {
      pt.set_from_trans(0,-1,0,-1,0,0);
    }
  }
  else if(mt==1) {
    if((nt==1&&z)||nt==2) {
      pt.set_from_trans(1,0,(size+1)/3,0,1,(size+1)/3);
    }
    else{
      pt.set_from_trans(0,-1,(size+1)/3,-1,0,(size+1)/3);
    }
  }
  else {
    if((nt==2&&z)||nt==0) {
      pt.set_from_trans(1,0,-((size+1)/3),0,1,-((size+1)/3));
    }
    else {
      pt.set_from_trans(0,-1,-((size+1)/3),-1,0,-((size+1)/3));
    }
  }
}
