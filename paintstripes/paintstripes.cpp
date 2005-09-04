#include "paintstripes.h"
#include <algorithm>

using std::copy;

void randomcauchy(double *d,double var)
{
  double z=(double)rand()/RAND_MAX;
  double r=var*sqrt(1./(z*z)-1.);
  z=(2.*M_PI*rand())/RAND_MAX;
  *d=r*cos(z);
  *(d+1)=r*sin(z);
}

void paintstripes::paint(int sz, symgroup sym)
{
  if(sz!=size) {
    if(array) fftw_free(array);
    array=fftw_malloc(sizeof(fftw_complex)*sz*(sz/2+1));
    if(fftplan) fftw_destroy_plan(fftplan);
    fftw_plan_dft_c2r_2d(sz,sz,(fftw_complex *)array,array,FFTW_MEASURE);
  }
  painter::paint(sz,sym);
  rowskip=halfsize+1;
  dq=M_PI/halfsize;
  fill(red);
  fill(green);
  fill(blue);
}

void paintstripes::fill(vector<unsigned char> &arr)
{
  int i,j;
  int size2=size+2;
  for(i=0;i<rowskip;i++)
    for(j=0;j<size;j++)
      randomcauchy(array+2*i+j*size2,(this->*norm)(i,j));
  fftw_execute_plan(fftplan);
  enumerate(*this,&paintstripes::symmetrize);
  for(i=0;i<size;i++) {
    copy(array+i*rowskip,array+i*rowskip+size,arr.begin()+i*size);
  }
}
