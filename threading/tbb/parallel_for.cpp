/* from :http://parallel-for.sourceforge.net/mpi-tbb-cpu-sse.html */

#include <mpi.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <tbb/cache_aligned_allocator.h>
using namespace tbb;
#include <xmmintrin.h>


struct thread1 {
 float ee;
 float *x, *y;
 thread1(float *xx, float *yy) : ee(0), x(xx), y(yy) {}
 thread1(thread1& s, split) { ee = 0; x = s.x; y = s.y; }
 void operator() (const blocked_range<int> & r) {
  // do computation
  float ve[4] = {0, 0, 0, 0};
  float e = ee;
  for (int i=r.begin(); i!=r.end(); i+=4) {
   float half =.5;
   _mm_store_ps(&x[i],
    	_mm_mul_ps(_mm_load1_ps(&half),
      	_mm_add_ps(_mm_loadu_ps(&y[i+1]),
        	_mm_loadu_ps(&y[i-1]))));
   _mm_store_ps(&ve[0],
    	_mm_add_ps(_mm_load_ps(&ve[0]),
      	_mm_mul_ps(_mm_load_ps(&y[i]),
        	_mm_load_ps(&y[i]))));
  }
  e += ve[0] + ve[1] + ve[2] + ve[3];
  ee = e;
 }
 void join(thread1& s) { ee += s.ee; }
};

int main(int argc, char *argv[]) {
  int n = ...;
  MPI_Init(&argc, &argv);
  int numproc, me;
  MPI_Comm_size(MPI_COMM_WORLD, &numproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  int p_left = -1, p_right = -1;
  if (me > 0)
   p_left = me-1;
  if (me < numproc-1)
   p_right = me+1;
  int n_local0 = 1 + (me * (n-1)) / numproc;
  int n_local1 = 1 + ((me+1) * (n-1)) / numproc;
  // allocate only local part + ghost zone of the arrays x,y
  float *x, *y;
  x = cache_aligned_allocator<float>().allocate(n_local1 - n_local0 + 2);
  y = cache_aligned_allocator<float>().allocate(n_local1 - n_local0 + 2);
  x -= (n_local0 - 1);
  y -= (n_local0 - 1);

  ... // fill x, y

  // fill ghost zone
  MPI_Status s;
  if (p_left != -1)
    MPI_Send(&y[n_local0], 1, MPI_FLOAT, p_left,
      1, MPI_COMM_WORLD);
  if (p_right != -1) {
    MPI_Recv(&y[n_local1], 1, MPI_FLOAT, p_right,
      1, MPI_COMM_WORLD, &s);
    MPI_Send(&y[n_local1-1], 1, MPI_FLOAT, p_right,
      2, MPI_COMM_WORLD);
  }
  if (p_left != -1)
    MPI_Recv(&y[n_local0-1], 1, MPI_FLOAT, p_left,
      2, MPI_COMM_WORLD, &s);


  thread1 t(x, y);
  parallel_reduce(blocked_range<int>(n_local0, n_local1, 1024), t);
  float e = t.ee;

  float e_local = e;
  MPI_Allreduce(&e_local, &e, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

  ... // output x, e

  x += (n_local0 - 1);
  y += (n_local0 - 1);
  cache_aligned_allocator<float>().deallocate(x, n_local1 - n_local0 + 2);
  cache_aligned_allocator<float>().deallocate(y, n_local1 - n_local0 + 2);
  MPI_Finalize();
  return 0;
}
