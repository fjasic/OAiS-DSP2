#include <math.h>
#include <stdlib.h>
#include "obrada.h"
#include "sr_fft.h"

double time_buffer[FFT_SIZE];
double fft_buffer[FFT_SIZE];
double in_delay[FFT_SIZE/2];
double out_delay[FFT_SIZE/2];

extern double window[FFT_SIZE];

void obrada(double *in, double *out, int N)
{
  int i;
  int x;
  for (i = 0; i < N; i++) {
	  out[i] = in[i];
  }
  for (i = 0; i < N; i++) {
	  x = N + i;
	  time_buffer[i] = in_delay[i];
	  time_buffer[x] = in[i];

  }	
  fft(time_buffer, fft_buffer, 10);
  ifft(fft_buffer, time_buffer, 10);

  for (i = 0; i < 2*N; i++) {
	  time_buffer[i] = time_buffer[i] * window[i];
  }

  for (i = 0; i < N; i++) {
	  out[i] = time_buffer[i] + out_delay[i];
  }

  for (i = 0; i < N; i++) {
	  x = i + N;
	  out_delay[i] = time_buffer[x];
  }

  for (i = 0; i < N; i++) {
	  in_delay[i] = in[i];
  }

  }


