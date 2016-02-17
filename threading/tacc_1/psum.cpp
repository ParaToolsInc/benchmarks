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

int main(int a, char **b) {
  int *bufs[NUMBUF];
  int NCORES[] = { 1, 2, 4, 8, 16, 20, 30, 40, 50 };

  for (int i = 0; i < NUMBUF; i++) {
    bufs[i] = new int[BUFSIZE];
    for (int j = 0; j < BUFSIZE; j++) bufs[i][j] = i;
  }

  {
    tic t0 = tsc();

    for (int i = 0; i < NFRAMES; i++)
      for (int j = 1; j < NUMBUF; j++) {
        int *a = bufs[0];
        int *b = bufs[j];
        for (int k = 0; k < BUFSIZE; k++) *a++ += *b++;
      }

    tic ttics = tsc() - t0;
    std::cout << "non-parallel " << ttics / (2.8 * 1000000000.0) << "\n";
  }

  for (int jj = 0; jj < sizeof(NCORES) / sizeof(int); jj++) {
    int nc = NCORES[jj];

    tic t0 = tsc();

    for (int i = 0; i < NFRAMES; i++) {
      int nchunks = nc;
      int chunk_size = BUFSIZE / nchunks;

      std::vector<std::pair<int, int> > chunks(nchunks);
      std::vector<std::future<void> > futures;

      for (int ii = 0; ii < nchunks - 1; ii++)
        chunks.push_back(std::make_pair(ii * chunk_size, ii * chunk_size + chunk_size));
      chunks.push_back(std::make_pair((nchunks - 1) * chunk_size, BUFSIZE));

      for (auto &limit : chunks)
        futures.push_back(std::async(std::launch::async, [&]() {
          for (int j = 1; j < NUMBUF; j++) {
            int *a = bufs[0] + limit.first;
            int *b = bufs[j] + limit.first;
            for (int k = 0; k < (limit.second - limit.first); k++) *a++ += *b++;
          }
        }));

      for (std::future<void> &f : futures) f.wait();
    }

    tic ttics = tsc() - t0;
    std::cout << "std::async " << nc << " " << ttics / (2.8 * 1000000000.0) << "\n";
  }

  for (int jj = 0; jj < sizeof(NCORES) / sizeof(int); jj++) {
    omp_set_num_threads(NCORES[jj]);

    tic t0 = tsc();
    int *a = bufs[0];
    for (int i = 0; i < NFRAMES; i++)
      for (int j = 1; j < NUMBUF; j++) {
        int *b = bufs[j];
#pragma omp parallel for
        for (int k = 0; k < BUFSIZE; k++) a[k] += b[k];
      }

    tic ttics = tsc() - t0;
    std::cout << "openmp " << NCORES[jj] << ": " << ttics / (2.8 * 1000000000.0) << "\n";
  }

  for (int jj = 0; jj < sizeof(NCORES) / sizeof(int); jj++) {
    //  omp_set_num_threads(NCORES[jj]);
    tbb::task_scheduler_init init(NCORES[jj]);
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
    std::cout << "tbb single buffer " << NCORES[jj] << ": " << ttics / (2.8 * 1000000000.0) << "\n";
  }

  for (int jj = 0; jj < sizeof(NCORES) / sizeof(int); jj++) {
    tbb::task_scheduler_init init(NCORES[jj]);
    tic t0 = tsc();
    int *a = bufs[0];
    for (int i = 0; i < NFRAMES; i++)
      tbb::parallel_for(tbb::blocked_range<size_t>(0, BUFSIZE), [&](const tbb::blocked_range<size_t> &range) {
        for (int k = range.begin(); k != range.end(); ++k) {
          for (int j = 1; j < NUMBUF; j++) {
            int *b = bufs[j];
            a[k] += b[k];
          }
        }
      });

    tic ttics = tsc() - t0;
    std::cout << "tbb rank buffer " << NCORES[jj] << ": " << ttics / (2.8 * 1000000000.0) << "\n";
  }
}
