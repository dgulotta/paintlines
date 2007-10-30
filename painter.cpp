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

#define PAINTER_INDEX_ONE(i,j) index=mod(i,size)+size*mod(j,size)
#define PAINTER_INDEX_TWO(i,j) \
ii=(i); \
jj=(j); \
(this->*f)(ii,jj); \
index2=mod(ii+size*k,width)+width*mod(jj+size*l,height);

void painter::rot0(int &i, int &j) {
}

void painter::rot60(int &i, int &j) {
  j+=i;
  i-=j;
}

void painter::rot120(int &i, int &j) {
  i=-(i+j);
  j=-(i+j);
}

void painter::rot180(int &i, int &j) {
  i=-i;
  j=-j;
}

void painter::rot240(int &i, int &j) {
  j=-(i+j);
  i=-(i+j);
}

void painter::rot300(int &i, int &j) {
  i+=j;
  j-=i;
}

void painter::trans1_3(int &i, int &j) {
  i+=(size+1)/3;
  j+=(size+1)/3;
}

void painter::transrot120(int &i, int &j) {
  int inew=-(i+j+(2*size+1)/3);
  j=i+(size+1)/3;
  i=inew;
}

void painter::transrot240(int &i, int &j) {
  int inew=j+(size+1)/3;
  j=-(i+j+(2*size+1)/3);
  i=inew;
}

void painter::randomize_p3m1_helper
(vector<unsigned char> &r, vector<unsigned char> &g, vector<unsigned char> &b,
 int a, int z, int i, int j, void (painter::*f)(int &, int &))
{
  int index, index2, ii, jj;
  switch(a) {
  case 0:
    PAINTER_INDEX_TWO(i,j);
    if(z&1) {
      PAINTER_INDEX_ONE(i,j);
    }
    else {
      PAINTER_INDEX_ONE(-j,i+j);
    }
    PAINTER_COPY_RGB;
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
  case 1:
    PAINTER_INDEX_ONE(i,j);
    PAINTER_INDEX_TWO(i,j);
    PAINTER_COPY_RGB;
    PAINTER_INDEX_ONE((size+1)/3-i,(size+1)/3-j);
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
  case 2:
    PAINTER_INDEX_ONE(-j,i+j);
    PAINTER_INDEX_TWO(i,j);
    PAINTER_COPY_RGB;
    PAINTER_INDEX_ONE(j-(size+1)/3,(2*size+1)/3-(i+j));
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
  case 3:
    PAINTER_INDEX_ONE((size+1)/3-i,(size+1)/3-j);
    PAINTER_INDEX_TWO(i,j);
    PAINTER_COPY_RGB;
    PAINTER_INDEX_ONE(i,j);
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
  case 4:
    PAINTER_INDEX_TWO(i,j);
    if(z&1) {
      PAINTER_INDEX_ONE((size+1)/3-i,(size+1)/3-j);
    }
    else {
      PAINTER_INDEX_ONE((size+1)/3+j,(size+1)/3-(i+j));
    }
    PAINTER_COPY_RGB;
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
  case 5:
    PAINTER_INDEX_ONE((size+1)/3+j,(size+1)/3-(i+j));
    PAINTER_INDEX_TWO(i,j);
    PAINTER_COPY_RGB;
   PAINTER_INDEX_ONE(i+j-(size+1)/3,(2*size+1)/3-i);
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
  case 6:
    PAINTER_INDEX_ONE(j-(size+1)/3,(2*size+1)/3-(i+j));
    PAINTER_INDEX_TWO(i,j);
    PAINTER_COPY_RGB;
    PAINTER_INDEX_ONE(-j,i+j);
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
  case 7:
   PAINTER_INDEX_ONE(i+j-(size+1)/3,(2*size+1)/3-i);
    PAINTER_INDEX_TWO(i,j);
    PAINTER_COPY_RGB;
    PAINTER_INDEX_ONE((size+1)/3+j,(size+1)/3-(i+j));
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
  case 8:
    PAINTER_INDEX_TWO(i,j);
    if(z&1) {
      PAINTER_INDEX_ONE(j-(size+1)/3,(2*size+1)/3-(i+j));
    }
    else {
      PAINTER_INDEX_ONE(i+j-(size+1)/3,(2*size+1)/3-i);
    }
    PAINTER_COPY_RGB;
    PAINTER_INDEX_TWO((size+1)/3-i,(size+1)/3-j);
    PAINTER_COPY_RGB;
    break;
    //default:
    //std::cout << "VERY BAD! " << a << std::endl;
  }
}

void painter::randomize(int _xtiles, int _ytiles, vector<unsigned char> &r,
		 vector<unsigned char> &g, vector<unsigned char> &b)
{
  xtiles=_xtiles;
  ytiles=_ytiles;
  width=xtiles*size;
  height=ytiles*size;
  int i,j;
  int qsize=halfsize/2, sizeq1=size1+halfsize-qsize, sizesq=size*size;
  int bigsize=width*height;
  r.resize(bigsize);
  g.resize(bigsize);
  b.resize(bigsize);
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
    {
      int yt=ytiles*3;
      int ntiles=xtiles*yt;
      vector<unsigned char>types(ntiles);
      for(i=0;i<ntiles;i++)
	types[i]=rand()%3;
      for(k=0;k<xtiles;k++)
	for(l=0;l<ytiles;l++) {
	  //int a=3*types[3*l+yt*k]+types[3*l+yt*k+1];
	  int z=rand();
	  //std::cout << a;
	  //if(a%4==0) std::cout << ' ' << (z&1);
	  //std::cout << std::endl;
	  for(i=0;3*i<=size+1;i++)
	    for(j=0;3*(i+j)<=size+1;j++) {
	      randomize_p3m1_helper(r,g,b,3*types[3*l+yt*k]+types[3*l+yt*k+1],z,i,j,&painter::rot0);
	      randomize_p3m1_helper(r,g,b,3*types[3*l+yt*k]+types[3*l+yt*mod(k-1,xtiles)+2],z,i,j,&painter::rot60);
	      randomize_p3m1_helper(r,g,b,3*types[3*l+yt*k]+types[3*l+yt*mod(k-1,xtiles)+1],z,i,j,&painter::rot120);
	      randomize_p3m1_helper(r,g,b,3*types[3*l+yt*k]+types[3*mod(l-1,ytiles)+yt*mod(k-1,xtiles)+2],z,i,j,&painter::rot180);
	      randomize_p3m1_helper(r,g,b,3*types[3*l+yt*k]+types[3*mod(l-1,ytiles)+yt*k+1],z,i,j,&painter::rot240);
	      randomize_p3m1_helper(r,g,b,3*types[3*l+yt*k]+types[3*mod(l-1,ytiles)+yt*k+2],z,i,j,&painter::rot300);
	      randomize_p3m1_helper(r,g,b,3*types[3*l+yt*k+1]+types[3*l+yt*k+2],z,i,j,&painter::trans1_3);
	      randomize_p3m1_helper(r,g,b,3*types[3*l+yt*mod(k-1,xtiles)+1]+types[3*l+yt*mod(k-2,xtiles)+2],z,i,j,&painter::transrot120);
	      randomize_p3m1_helper(r,g,b,3*types[3*mod(l-1,ytiles)+yt*k+1]+types[3*mod(l-2,ytiles)+yt*k+2],z,i,j,&painter::transrot240);
	    }
	}
    }
  }
}
