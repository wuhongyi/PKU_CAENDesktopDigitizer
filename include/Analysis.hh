// Analysis.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 4月 15 13:10:20 2017 (+0800)
// Last-Updated: 六 4月 15 13:16:26 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

// source from www.caen.it


#define FFT_BASELINE  0.0000001  // Baseline for low clipping (-140dB)
#define NORM_FACTOR   4096       // Normalize the amplitide (12bit ADC)

// Types of windowing
#define HANNING_FFT_WINDOW    0
#define HAMMING_FFT_WINDOW    1
#define BLACKMAN_FFT_WINDOW   2
#define RECT_FFT_WINDOW       3



// Description:
// -----------------------------------------------------------------------------
// This is a function that calculates the FFT for a vector of 'ns' samples.
// The number ns must be a power of 2. In case it isn't, the closest power of 2
// smaller than ns will be considered and exceeding samples ignored.
// The calculation of the FFT is based on imaginary numbers (x = real part,
// y = imaginary part). However, the input vector is represented as real numbers
// (unsigned short) and the function returns a vector of real numbers (double)
// that are the amplitude (i.e. sqrt(x^2 + y^2) ) of the FFT points. 
// The amplitude is also normalized respect to the maximum amplitude (for 
// example 4096 for 12 bit samples) and expressed in dB. A contant baseline (for
// example 0.0000001 which is -140dB) is also added to the value in order to 
// low clip the FFT points. Since the FFT has two symmetrical lobes, only half
// points are returned.

// Input Parameters: 
// --------------------------------------------------------------------------
// wave: pointer to the input vector (waveform samples)
// fft: pointer to the output vector (fft amplitude in dB, half lobe)
// ns: number of samples of the input vector wave
// WindowType: Type of windowing for the FFT

// Return:
// --------------------------------------------------------------------------
// Number of pointf of the output vector fft
int FFT(unsigned short *wave, double *fft, int ns, int WindowType);


#endif /* _ANALYSIS_H_ */
// 
// Analysis.hh ends here
