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
  size2=size+2;
  if(sz!=size) {
    if(array) fftw_free(array);
    array=(double *)fftw_malloc(sizeof(double)*sz*size2);
    if(fftplan) fftw_destroy_plan(fftplan);
    fftw_plan_dft_c2r_2d(sz,sz,(fftw_complex *)array,array,FFTW_MEASURE);
  }
  painter::paint(sz,sym);
  dq=M_PI/halfsize;
  fill(red);
  fill(green);
  fill(blue);
}

void paintstripes::fill(vector<unsigned char> &arr)
{
  int i,j;
  for(i=0;i<=halfsize;i++)
    for(j=0;j<size;j++)
      randomcauchy(array+2*i+j*size2,(this->*norm)(i,j));
  fftw_execute(fftplan);
  enumerate(*this,&paintstripes::symmetrize);
  double norm(0.);
  for(i=0;i<size;i++)
    for(j=0;j<size;j++) {
      double d=array[i+size2*j];
      norm+=d*d;
    }
  norm=64*sqrt(norm)/size;
  for(i=0;i<size;i++)
    for(j=0;j<size;j++)
      arr[i+size*j]=colorchop(128.+array[i+size2*j]/norm);
}
