#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "obrada.h"
#include "mdct.h"

double time_buffer[MDCT_SIZE];
double mdct_buffer[MDCT_SIZE/2];
double in_delay[MDCT_SIZE/2];
double out_delay[MDCT_SIZE/2];
extern double window[MDCT_SIZE];

void obrada(double *in, double *out, int N)
{
  int i;
  int x;
  int b=6;
  for (i=0; i<N; i++)
    out[i] = in[i];

  for (i = 0; i < N; i++) {
      x = N + i;
      time_buffer[i] = in_delay[i];
      time_buffer[x] = in[i];

  }
  mdct(time_buffer, mdct_buffer);
  if (mdct_buffer[i] < 0) {
      mdct_buffer[i] = -sqrt(-mdct_buffer[i]);
  }
  else {
      mdct_buffer[i] = sqrt(mdct_buffer[i]);
  }
  mdct_buffer[i]=round(mdct_buffer[i] / pow(2, b - 1));
  mdct_buffer[i]=mdct_buffer[i] * pow(2,12);

  mdct_buffer[i]=mdct_buffer[i] *pow(2,12);
  mdct_buffer[i]=round(mdct_buffer[i] / pow(2, b - 1));

  if (mdct_buffer[i] < 0) {
      mdct_buffer[i] = -mdct_buffer[i]*mdct_buffer[i];
  }
  else {
      mdct_buffer[i] = mdct_buffer[i]*mdct_buffer[i];
  }
  imdct(mdct_buffer, time_buffer);


  for (i = 0; i < N; i++) {
      out[i] = time_buffer[i]/2 + out_delay[i]/2;
  }

  for (i = 0; i < N; i++) {
      x = i + N;
      out_delay[i] = time_buffer[x];
  }

  for (i = 0; i < N; i++) {
      in_delay[i] = in[i];
  }

}

void statistika()
{
}
