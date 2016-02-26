#include <algorithm>
#include <future>
#include <iostream>
#include <omp.h>
#include <thread>
#include <chrono>

#define NFRAMES 500
#define BUFSIZE 1920 * 1080 * 3
#define NUMBUF 16

// The following is a little code to reaad the TSC - it provides a very high resultion way
// to compare timing.  The reported results should be divided by the CPU frequency
// to get wall time.

#ifdef TAU_ENABLED
#include <TAU.h>
#endif

int main(int argc, char* argv[]) {
  using namespace std::chrono;

  int *bufs[NUMBUF];
  int NCORES = 1 ;

  if (argv[1]!=NULL) {
    NCORES = atoi(argv[1]);
  }

  for (int i = 0; i < NUMBUF; i++) {
    bufs[i] = new int[BUFSIZE];
    for (int j = 0; j < BUFSIZE; j++) bufs[i][j] = i;
  }




    omp_set_num_threads(NCORES);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    int *a = bufs[0];
    for (int i = 0; i < NFRAMES; i++)
      for (int j = 1; j < NUMBUF; j++) {
        int *b = bufs[j];
#ifdef TAU_ENABLED
   TAU_START("inner for loop");
#endif
#pragma omp parallel for
        for (int k = 0; k < BUFSIZE; k++) a[k] += b[k];
#ifdef TAU_ENABLED
   TAU_STOP("inner for loop");
#endif
      }
     high_resolution_clock::time_point t2 = high_resolution_clock::now();
     duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
     std::cout << "openmp " << NCORES << ": " << time_span.count() << " seconds.\n";

  }
