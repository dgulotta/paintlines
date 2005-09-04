#ifndef _PAINTSTRIPES_H
#define _PAINTSTRIPES_H

#include "../painter.h"
#include <fftw3.h>

void randomcauchy(double *d);

class paintstripes : virtual public painter
{
 public:
  painter() : p(NULL), array(NULL) {}
  ~painter() {
    if(fftplan) fftw_destroy_plan(fftplan);
    if(array) fttw_free(array);
  }
  void paint(int sz, symgroup sym);
 private:
  void symmetrize(int x, int y) {
    sum=0.;
    painter::symmetrize(*this,&paintstripes::accumulate,x,y);
    painter::symmetrize(*this,&paintstripes::set,x,y);
  }
  void accumulate(int x, int y) {
    sum+=array[x+y*rowskip];
  }
  void set(int x, int y) {
    array[x+y*rowskip]=sum;
  }
  double norm_square(int x, int y) {
    if(x||y)
      return 1./(2.-cos(dq*x)-cos(dq*y));
    else return 0.;
  }
  double norm_hexagonal(int x, int y) {
    if(x||y)
      return 1./(3.-cos(dq*x)-cos(dq*y)-cos(dq*(x+y)));
    else return 0;
  }
  double (paintstripes::*norm)(int,int);
  int rowskip;
  fftw_plan fftplan;
  double *array;
  double sum;
  double dq;
}

#endif
