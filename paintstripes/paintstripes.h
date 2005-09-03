#ifndef _PAINTSTRIPES_H
#define _PAINTSTRIPES_H

#include "../painter.h"
#include <fftw3.h>

class paintstripes : public painter
{
 public:
  painter() : p(NULL), array(NULL) {}
  ~painter() {
    if(fftplan) fftw_destroy_plan(fftplan);
    if(array) fttw_free(array);
  }
  void paint(int sz, symgroup sym);
 private:
  fftw_plan fftplan;
  double *array;
}

#endif
