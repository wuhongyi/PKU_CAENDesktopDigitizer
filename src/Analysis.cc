// Analysis.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 4月 15 13:10:33 2017 (+0800)
// Last-Updated: 日 4月 16 17:03:16 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "Analysis.hh"

#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI 3.141592653
#endif

int FFT(unsigned short *wave, double *fft, int ns, int WindowType)
{
  int m,n,ip,le,le1,nm1,k,l,j,i,nv2;
  double u1,u2,u3,arg,c,s,t1,t2,t3,t4;
  double *x, *y;

  /* ns should be a power of 2. If it is not, find the maximum m
     such that n = 2^m < ns and get n samples instead of ns.*/
  i = 1;
  while((int)std::pow(2,i) < ns)
    i++;
  if(ns == (int)(std::pow(2,i))) {
    m = i;
    n = ns;
  }
  else {
    m = i - 1;
    n = (int)std::pow(2,m);
  }

  /* allocate the memory buffers for the real and imaginary parts of the fft */
  x = (double *)malloc(n * sizeof(double));
  y = (double *)malloc(n * sizeof(double));

  /* apply the windowing to the input vector */
  for(i=0; i<n; i++) {
    y[i] = 0.0; /* imaginary part of the input vector (always 0) */
    switch (WindowType) {
    case HANNING_FFT_WINDOW  :  
      x[i] = wave[i] * (0.5 - 0.5 * std::cos(2*M_PI * i/n));
      break;
    case HAMMING_FFT_WINDOW  :  
      x[i] = wave[i] * (0.54 - 0.46 * std::cos(2*M_PI * i/n)); 
      break;
    case BLACKMAN_FFT_WINDOW  :  
      x[i] = wave[i] * (2.4 * (0.42323 - 0.49755*cos(2*M_PI*i/n) + 0.07922*std::cos(4*M_PI*i/n)));
      break;
    case RECT_FFT_WINDOW  :  
      x[i] = wave[i];
      break;
    default :  
      x[i] = wave[i] * (2.4*(0.42323-0.49755*cos(2*M_PI*(i)/n)+0.07922*cos(4*M_PI*(i)/n)));
      break;
    }
  }

  /* now the vectors x and y represents the input waveform expressed as imaginary numbers
     after the appplication of the windowing. */

  /* calculate the FFT */
  for(l=0; l<m; l++) {
    le=(int)std::pow(2,m-l);
    le1=le/2;
    u1=1.0;
    u2=0.0;
    arg=M_PI/le1;
    c=std::cos(arg);
    s=-std::sin(arg);

    for (j=0; j<le1; j++) {
      for (i=j; i<n; i=i+le) {
	ip=i+le1;
	t1=x[i]+x[ip];
	t2=y[i]+y[ip];
	t3=x[i]-x[ip];
	t4=y[i]-y[ip];
	x[ip]=t3*u1-t4*u2;
	y[ip]=t4*u1+t3*u2;
	x[i]=t1;
	y[i]=t2;
      }
      u3=u1*c-u2*s;
      u2=u2*c+u1*s;
      u1=u3;
    }
  }

  nv2=n/2;
  nm1=n-1;
  j=0;
  i=0;
  while (i<nm1) {
    if(i>j)
      goto rif;
    t1=x[j];
    t2=y[j];
    x[j]=x[i];
    y[j]=y[i];
    x[i]=t1;
    y[i]=t2;
  rif:
    k=nv2;
  rife:
    if (k>j)
      goto rifer;
    j=j-k;
    k=k/2;
    goto rife;
  rifer:
    j=j+k;
    i++;
  }

  /* get the amplitude of the FFT (modulo) */
  y[0]=y[0]/n;
  x[0]=x[0]/n;
  fft[0]=std::sqrt(x[0]*x[0] + y[0]*y[0]);
  for(i=1;i<n/2;i++) {
    y[i]=2*y[i]/n;
    x[i]=2*x[i]/n;
    fft[i]=std::sqrt(x[i]*x[i] + y[i]*y[i]);
  }

  /* Add the baseline, normalize and transform in dB */
  for(i=0; i<n/2; i++) 
    fft[i] = 20 * std::log10( fft[i]/NORM_FACTOR + FFT_BASELINE);

  /* free the buffers and return the number of points (only half FFT) */
  free(x);
  free(y);
  return (n/2);
}



int Pixie16complexFFT(double *data, unsigned int length)
{
  double tempR;
  double tempI;
  double theta;
  double wr;
  double wi;
  double wpr;
  double wpi;
  double wtemp;
	
  unsigned int nComplex;
  unsigned int m;
  unsigned int mmax;
  unsigned int iStep;
  unsigned int i, j;
	
  nComplex = 2 * length;
	
  // Do the bit reversal re-ordering
  j = 0;
  for (i = 0; i < nComplex; i += 2) 
    {
      if (j > i) 
	{
	  tempR = data[j];
	  tempI = data[j+1];
	  data[j]   = data[i];
	  data[j+1] = data[i+1];
	  data[i]   = tempR;
	  data[i+1] = tempI;
	}
		
      m = nComplex / 2;
      while ((m >= 2) && (j >= m)) 
	{
	  j = j - m;
	  m = m / 2;
	}
		
      j = j + m;
    }
	
  // Do the recursive FFT
  mmax = 2;
  while (nComplex > mmax)
    {
      iStep = 2 * mmax;
      theta = PI2 / mmax;
      wpr = std::sin(0.5 * theta);
      wpr = -2. * wpr * wpr;
      wpi = std::sin(theta);
      wr = 1.;
      wi = 0.;
      for (m = 0; m < mmax; m += 2)
	{
	  for (i = m; i < nComplex; i += iStep)
	    {
	      j = i + mmax;
	      tempR = wr * data[j]   - wi * data[j+1];
	      tempI = wr * data[j+1] + wi * data[j];
	      data[j]   = data[i]   - tempR;
	      data[j+1] = data[i+1] - tempI;
	      data[i]   = data[i]   + tempR;
	      data[i+1] = data[i+1] + tempI;
	    }
	  wtemp = wr;
	  wr = wr * wpr - wi * wpi + wr;
	  wi = wi * wpr + wtemp * wpi + wi;
	}
      mmax = iStep;
    }
	
  return 0;
	
}


// 
// Analysis.cc ends here
