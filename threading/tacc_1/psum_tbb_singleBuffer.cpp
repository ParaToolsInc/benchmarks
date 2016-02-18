#include <algorithm>
#include <future>
#include <iostream>
#include <omp.h>
#include <thread>

#include <tbb/blocked_range.h>
#include <tbb/mutex.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

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

int main(int argc, char* argv[]) {
  int *bufs[NUMBUF];
  int NCORES = 1 ;

  if (argv[1]!=NULL) {
    NCORES = atoi(argv[1]);
  }

  for (int i = 0; i < NUMBUF; i++) {
    bufs[i] = new int[BUFSIZE];
    for (int j = 0; j < BUFSIZE; j++) bufs[i][j] = i;
  }


    tbb::task_scheduler_init init(NCORES);
    tic t0 = tsc();

    int *a = bufs[0];
    for (int i = 0; i < NFRAMES; i++)
      for (int j = 1; j < NUMBUF; j++) {
        int *b = bufs[j];
        tbb::parallel_for(tbb::blocked_range<size_t>(0, BUFSIZE), [&](const tbb::blocked_range<size_t> &range) {
          for (int c = range.begin(); c != range.end(); ++c) {
            a[c] += b[c];
          }
        });
      }

    tic ttics = tsc() - t0;
    std::cout << "tbb single buffer " << NCORES << ": " << ttics / (2.8 * 1000000000.0) << "\n";


}
