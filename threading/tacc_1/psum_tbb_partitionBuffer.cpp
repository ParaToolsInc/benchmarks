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

#ifdef TAU_ENABLED
#include <TAU.h>
#endif

int main(int argc, char* argv[]) {
  using namespace std::chrono;

#ifdef TAU_ENABLED
  TAU_PROFILE_SET_NODE(0);
#endif
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
    static tbb::simple_partitioner sp;
    size_t psize = (BUFSIZE / (NCORES*4));
    int *a = bufs[0];
#ifdef TAU_ENABLED
  TAU_START("Outter for loop");
#endif
    for (int i = 0; i < NFRAMES; i++)
      tbb::parallel_for(tbb::blocked_range<size_t>(0, BUFSIZE,psize), [&](const tbb::blocked_range<size_t> &range) {
        for (int k = range.begin(); k != range.end(); ++k) {
          for (int j = 1; j < NUMBUF; j++) {
            int *b = bufs[j];
            a[k] += b[k];
          }
        }
      },sp);

#ifdef TAU_ENABLED
  TAU_STOP("Outter for loop");
#endif
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "tbb rank buffer " << NCORES<< ": " << time_span.count() << " seconds.\n";

}
