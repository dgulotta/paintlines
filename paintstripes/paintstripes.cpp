#include "paintstripes.h"
#include <algorithm>

using std::copy;

void paintstripes::paint(int sz, symgroup sym)
{
  if(sz!=size) {
    if(array) fftw_free(array);
    array=fftw_malloc(sizeof(fftw_complex)*sz*(sz/2+1));
    if(fftplan) fftw_destroy_plan(fftplan);
    fftw_plan_dft_c2r_2d(sz,sz,(fftw_complex *)array,array,FFTW_MEASURE);
  }
  painter::paint(sz,sym);
  fill(red);
  fill(green);
  fill(blue);
}

void paintstripes::fill(vector<unsigned char> &arr)
{
   int i,j, rowskip=(size|1)+1;
  // put random values into array
  fftw_execute_plan(fftplan);
  // symmetrize
  for(i=0;i<size;i++) {
    copy(array+i*rowskip,array+i*rowskip+size,arr.begin()+i*size);
  }
}
