/***************************************************************************
 *   Copyright (C) 2005-2006 by Daniel Gulotta                             *
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

#include "painter.h"

#define PAINTER_COPY_RGB r[index2]=red[index];g[index2]=green[index];\
b[index2]=blue[index];

void painter::randomize(int xtiles, int ytiles, vector<unsigned char> &r,
		 vector<unsigned char> &g, vector<unsigned char> &b)
{
  int width=xtiles*size, height=ytiles*size;
  int i,j,k,l;
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
	    PAINTER_COPY_RGB
	    index=((z&2)?i:(size1-i))+size*j;
	    index2=size1-i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB
	    index=((z&4)?i:(size1-i))+size*j;
	    index2=i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB
	    index=((z&8)?i:(size1-i))+size*j;
	    index2=size1-i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB
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
	    PAINTER_COPY_RGB
	    index=mod(((z&2)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    index2=size1-i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB
	    index=mod(((z&4)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    index2=i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB
	    index=mod(((z&8)?(i+qsize):(sizeq1-i))+size*j,sizesq);
	    index2=size1-i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB
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
	    PAINTER_COPY_RGB
	    index=mod(((z&2)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    index2=size1-i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB
	    index=mod(((z&4)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    index2=i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB
	    index=mod(((z&8)?(i+qsize):(sizeq1-i))+size*(j+qsize),sizesq);
	    index2=size1-i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB
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
	    PAINTER_COPY_RGB
	    index=(z&0x2)?(i+size*j):(j+size*i);
	    index2=j+k*size+width*(i+l*size);
	    PAINTER_COPY_RGB
	    index=(z&0x4)?(i+size*j):(j+size*i);
	    index2=size1-i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB
	    index=(z&0x8)?(i+size*j):(j+size*i);
	    index2=j+k*size+width*(size1-i+l*size);
	    PAINTER_COPY_RGB
	    index=(z&0x10)?(i+size*j):(j+size*i);
	    index2=size1-i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB
	    index=(z&0x20)?(i+size*j):(j+size*i);
	    index2=size1-j+k*size+width*(size1-i+l*size);
	    PAINTER_COPY_RGB
	    index=(z&0x40)?(i+size*j):(j+size*i);
	    index2=i+k*size+width*(size1-j+l*size);
	    PAINTER_COPY_RGB
	    index=(z&0x80)?(i+size*j):(j+size*i);
	    index2=size1-j+k*size+width*(i+l*size);
	    PAINTER_COPY_RGB
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
	    PAINTER_COPY_RGB
	    index=(z&1)?((size1-i)+size*j):
	      mod((halfsize1-j)+size*(i-halfsize),sizesq);
	    index2=(size1-i)+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB
	    index=(z&2)?(i+size*j):
	      mod((halfsize1-j)+size*(halfsize1-i),sizesq);
	    index2=mod((halfsize1-j)+k*size,width)+width*
	      (mod((halfsize1-i)+l*size,height));
	    PAINTER_COPY_RGB
	    index=(z&2)?((size1-i)+size*j):
	      mod((halfsize1-j)+size*(i-halfsize),sizesq);
	    index2=mod((halfsize1-j)+k*size,width)+width*
	      (mod((i-halfsize)+l*size,height));
	    PAINTER_COPY_RGB
	  }
	}
      }
    break;
  case SYM_P3:
  case SYM_P31M:
   for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i*3<size;i++) {
	  for(j=i;j<(size-i)/2;j++) {
	    int index=(z&4)?(i+size*j):(j+size*i);
	    int index2=((k+1)%xtiles)*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB
	    index=(z&8)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(((l+1)%ytiles)*size+i);
	    PAINTER_COPY_RGB
	  }
	  for(;j<size-2*i;j++) {
	    int index=(z&16)?(i+size*j):(j+size*i);
	    int index2=((k+1)%xtiles)*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB
	    index=(z&32)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(((l+1)%ytiles)*size+i);
	    PAINTER_COPY_RGB
	  }
	  for(;j<size-i/2;j++) {
	    int index=(z&1)?(i+size*j):(j+size*i);
	    int index2=k*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB
	    index=(z&2)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(l*size+i);
	    PAINTER_COPY_RGB 
	  }
	}
	for(;i*3<2*size;i++)
	  for(j=i;j<size-i/2;j++) {
	    int index=(z&1)?(i+size*j):(j+size*i);
	    int index2=k*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB
	    index=(z&2)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(l*size+i);
	    PAINTER_COPY_RGB 
	  }
	for(;i<size;i++) {
	  for(j=2*(size-i);j<size-i/2;j++) {
	    int index=(z&4)?(i+size*j):(j+size*i);
	    int index2=k*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB
	    index=(z&8)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(l*size+i);
	    PAINTER_COPY_RGB 
	  }
	  for(;j<=i;j++) {
	    int index=(z&16)?(i+size*j):(j+size*i);
	    int index2=k*size+i+width*(l*size+j);
	    PAINTER_COPY_RGB
	    index=(z&32)?(i+size*j):(j+size*i);
	    index2=k*size+j+width*(l*size+i);
	    PAINTER_COPY_RGB
	  }
	}
      }
   break;
  case SYM_P3M1:
    for(k=0;k<xtiles;k++)
      for(l=0;l<ytiles;l++) {
	int z=rand();
	for(i=0;i<size;i++)
	  for(j=0;i+j<=size;j++) {
	    int index=(z&1)?(i+size*j):((size1-j)+size*(size1-i));
	    int index2=i+k*size+width*(j+l*size);
	    PAINTER_COPY_RGB
	    index=(z&2)?(i+size*j):((size1-j)+size*(size1-i));
	    index2=size1-j+k*size+width*(size1-i+l*size);
	    PAINTER_COPY_RGB
	  }
      }
  }
}
