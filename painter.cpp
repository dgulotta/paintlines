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

void painter_transform::set_point(int x, int y) {
  from_index=mod(fxx*x+fxy*y+fx1,size)+size*mod(fyx*x+fyy*y+fy1,size);
  to_index=mod(txx*x+txy*y+tx1,width)+width*mod(tyx*x+tyy*y+ty1,height);
}

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
