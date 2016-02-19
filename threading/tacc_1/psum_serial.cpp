#include <iostream>
#include <chrono>
#include <inttypes.h>


#define NFRAMES 500
#define BUFSIZE 1920 * 1080 * 3
#define NUMBUF 16

// The following is a little code to reaad the TSC - it provides a very high resultion way
// to compare timing.  The reported results should be divided by the CPU frequency
// to get wall time.

typedef uint64_t tic;

__inline__ tic tsc() {
  tic lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((tic)hi << 32 | lo);
}

int main(int a, char **b) {
  using namespace std::chrono;
  int *bufs[NUMBUF];


  for (int i = 0; i < NUMBUF; i++) {
    bufs[i] = new int[BUFSIZE];
    for (int j = 0; j < BUFSIZE; j++) bufs[i][j] = i;
  }

  {
   // tic t0 = tsc();
   high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for (int i = 0; i < NFRAMES; i++)
      for (int j = 1; j < NUMBUF; j++) {
        int *a = bufs[0];
        int *b = bufs[j];
        for (int k = 0; k < BUFSIZE; k++) *a++ += *b++;
      }

 //   tic ttics = tsc() - t0;
   high_resolution_clock::time_point t2 = high_resolution_clock::now();
   duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    //std::cout << "non-parallel " << ttics / (2.8 * 1000000000.0) << "\n";
    std::cout << "non-parallel " << time_span.count() << " seconds.\n";
  }

}
