#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <chrono>


#define NFRAMES 500
#define BUFSIZE 1920 * 1080 * 3
#define NUMBUF 16

// The following is a little code to reaad the TSC - it provides a very high resultion way
// to compare timing.  The reported results should be divided by the CPU frequency
// to get wall time.



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

    int nc = NCORES;

   high_resolution_clock::time_point t1 = high_resolution_clock::now();
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
   high_resolution_clock::time_point t2 = high_resolution_clock::now();
   duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "std::async " << nc << " " <<  time_span.count() << " seconds. \n";


}

