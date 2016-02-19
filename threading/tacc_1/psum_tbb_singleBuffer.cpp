#include <algorithm>
#include <future>
#include <iostream>
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


    tbb::task_scheduler_init init(NCORES);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

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

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "tbb single buffer " << NCORES << ": " << time_span.count()  << " seconds. \n";


}
