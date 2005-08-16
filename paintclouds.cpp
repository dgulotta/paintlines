#include "paintclouds.h"

void paintclouds::paint(int sz, symgroup sym)
{
  painter::paint(sy,sym);
  switch(sym) {
  case SYM_P2:
    red[0]=((int)red1+red2+red3)/3;
    green[0]=((int)green1+green2+green3)/3;
    blue[0]=((int)blue1+blue2+blue3)/3;
    red[halfsize]=red1;
    green[halfsize]=green1;
    blue[halfsize]=blue1;
    red[halfsize*size]=red2;
    green[halfsize*size]=green2;
    blue[halfsize*size]=blue2;
    red[halfsize*(size+1)]=red3;
    green[halfsize*(size+1)]=green3;
    blue[halfsize*(size+1)]=blue3;
    paint_border(0,0,0,halfsize);
    paint_border(0,0,halfsize,0);
    paint_border(size,0,halfsize,halfsize);
    for(i=1;i<halfsize;i++) {
      red[size-i]=red[i];
      green[size-i]=green[i];
      blue[size-i]=blue[i];
      red[(size-i)*size]=red[i*size];
      green[(size-i)*size]=green[i*size];
      blue[(size-i)*size]=blue[i*size];
    }
    paint_triangle(0,0,0,size,size,0);
    break;
  case SYM_P4:
    break;
  }
}

void paintclouds::paint_border(int x1, int y1, int x2, int y2)
{
  int mx=(x1+x2)/2;
  int my=(y1+y2)/2;
  if(!((mx==x1||mx==x2)&&(my==y1||my==y2))) {
    mi(red,mx,my)=colorchop(double(mi(red,x1,y1)+mi(red,x2,y2))/2.+
			      /*random*/);
    mi(green,mx,my)=colorchop(double(mi(green,x1,y1)+mi(green,x2,y2))/2.+
			      /*random*/);
    mi(blue,mx,my)=colorchop(double(mi(blue,x1,y1)+mi(blue,x2,y2))/2.+
			      /*random*/);
    paint_border(x1,x2,mx,my);
    paint_border(mx,my,x2,y2);
  }
}

void paintclouds::paint_triangle(int x1, int y1, int x2, int y2, int x3,
				 int y3)
{
  int area=x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2;
  // points must be in counterclockwise order or the area will be negative
  if(area>1) {
     int m1x=(x2+x3)/2, m1y=(y2+y3)/2, m2x=(x1+x3)/2, m2y=(y1+y3)/2;
     int m3x=(x1+x2)/2, m3y=(y1+y2)/2;
     paint_border(m1x,m1y,m2x,m2y);
     paint_border(m2x,m2y,m3x,m3y);
     paint_border(m3x,m3y,m1x,m1y);
     paint_triangle(x1,y1,m3x,m3y,m2x,m2y);
     paint_triangle(m3x,m3y,x2,y2,m1x,m1y);
     paint_triangle(m2x,m2x,m1x,m1y,x3,y3);
  }
}
