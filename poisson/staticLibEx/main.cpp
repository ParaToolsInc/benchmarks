# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <cmath>
# include <omp.h>

#ifdef __USE_MPI
#include <mpi.h>
#endif

using namespace std;

# define NX 161
# define NY 161

double r8mat_rms ( int m, int n, double a[NX][NY] );
void rhs ( int nx, int ny, double f[NX][NY] );
void sweep ( int nx, int ny, double dx, double dy, double f[NX][NY],
  int itold, int itnew, double u[NX][NY], double unew[NX][NY] );
void timestamp ( );
double u_exact ( double x, double y );
double uxxyy_exact ( double x, double y );

//****************************************************************************80

int main ( int argc, char *argv[] )

//****************************************************************************80
//
//  Purpose:
//
//    MAIN is the main program for POISSON_OPENMP.
//
//  Discussion:
//
//    POISSON_OPENMP is a program for solving the Poisson problem.
//
//    This program uses OpenMP for parallel execution.
//
//    The Poisson equation
//
//      - DEL^2 U(x,y) = F(x,y)
//
//    is solved on the unit square [0,1] x [0,1] using a grid of NX by
//    NX evenly spaced points.  The first and last points in each direction
//    are boundary points.
//
//    The boundary conditions and F are set so that the exact solution is
//
//      U(x,y) = sin ( pi * x * y )
//
//    so that
//
//      - DEL^2 U(x,y) = pi^2 * ( x^2 + y^2 ) * sin ( pi * x * y )
//
//    The Jacobi iteration is repeatedly applied until convergence is detected.
//
//    For convenience in writing the discretized equations, we assume that NX = NY.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 December 2011
//
//  Author:
//
//    John Burkardt
//
{
  bool converged;
  double diff;
  double dx;
  double dy;
  double error;
  double f[NX][NY];
  int i;
  int id;
  int itnew;
  int itold;
  int j;
  int jt;
  int jt_max = 20;
  int nx = NX;
  int ny = NY;
  double tolerance = 0.000001;
  double u[NX][NY];
  double u_norm;
  double udiff[NX][NY];
  double uexact[NX][NY];
  double unew[NX][NY];
  double unew_norm;
  double wtime;
  double x;
  double y;


  #ifdef __USE_MPI
  MPI_Init(&argc, &argv);
  #endif


  dx = 1.0 / ( double ) ( nx - 1 );
  dy = 1.0 / ( double ) ( ny - 1 );
//
//  Print a message.
//
  timestamp ( );
  cout << "\n";
  cout << "POISSON_OPENMP:\n";
  cout << "  C++ version\n";
  cout << "  A program for solving the Poisson equation.\n";
  cout << "\n";
  cout << "  Use OpenMP for parallel execution.\n";
  cout << "  The number of processors is " << omp_get_num_procs ( ) << "\n";
# pragma omp parallel
{
  id = omp_get_thread_num ( );
  if ( id == 0 )
  {
    cout << "  The maximum number of threads is " << omp_get_num_threads ( ) << "\n";
  }
}
  cout << "\n";
  cout << "  -DEL^2 U = F(X,Y)\n";
  cout << "\n";
  cout << "  on the rectangle 0 <= X <= 1, 0 <= Y <= 1.\n";
  cout << "\n";
  cout << "  F(X,Y) = pi^2 * ( x^2 + y^2 ) * sin ( pi * x * y )\n";
  cout << "\n";
  cout << "  The number of interior X grid points is " << nx << "\n";
  cout << "  The number of interior Y grid points is " << ny << "\n";
  cout << "  The X grid spacing is " << dx << "\n";
  cout << "  The Y grid spacing is " << dy << "\n";
//
//  Set the right hand side array F.
//
  rhs ( nx, ny, f );
//
//  Set the initial solution estimate UNEW.
//  We are "allowed" to pick up the boundary conditions exactly.
//
  for ( j = 0; j < ny; j++ )
  {
    for ( i = 0; i < nx; i++ )
    {
      if ( i == 0 || i == nx - 1 || j == 0 || j == ny - 1 )
      {
        unew[i][j] = f[i][j];
      }
      else
      {
        unew[i][j] = 0.0;
      }
    }
  }
  unew_norm = r8mat_rms ( nx, ny, unew );
//
//  Set up the exact solution UEXACT.
//
  for ( j = 0; j < ny; j++ )
  {
    y = ( double ) ( j ) / ( double ) ( ny - 1 );
    for ( i = 0; i < nx; i++ )
    {
      x = ( double ) ( i ) / ( double ) ( nx - 1 );
      uexact[i][j] = u_exact ( x, y );
    }
  }
  u_norm = r8mat_rms ( nx, ny, uexact );
  cout << "  RMS of exact solution = " << u_norm << "\n";
//
//  Do the iteration.
//
  converged = false;

  cout << "\n";
  cout << "  Step    ||Unew||     ||Unew-U||     ||Unew-Exact||\n";
  cout << "\n";

  for ( j = 0; j < ny; j++ )
  {
    for ( i = 0; i < nx; i++ )
    {
      udiff[i][j] = unew[i][j] - uexact[i][j];
    }
  }
  error = r8mat_rms ( nx, ny, udiff );
  cout << "  " << setw(4) << 0
       << "  " << setw(14) << unew_norm
       << "  " << "              "
       << "  " << setw(14) << error << "\n";

  wtime = omp_get_wtime ( );

  itnew = 0;

  for ( ; ; )
  {
    itold = itnew;
    itnew = itold + 500;
//
//  SWEEP carries out 500 Jacobi steps in parallel before we come
//  back to check for convergence.
//
    sweep ( nx, ny, dx, dy, f, itold, itnew, u, unew );
//
//  Check for convergence.
//
    u_norm = unew_norm;
    unew_norm = r8mat_rms ( nx, ny, unew );

    for ( j = 0; j < ny; j++ )
    {
      for ( i = 0; i < nx; i++ )
      {
        udiff[i][j] = unew[i][j] - u[i][j];
      }
    }
    diff = r8mat_rms ( nx, ny, udiff );

    for ( j = 0; j < ny; j++ )
    {
      for ( i = 0; i < nx; i++ )
      {
        udiff[i][j] = unew[i][j] - uexact[i][j];
      }
    }
    error = r8mat_rms ( nx, ny, udiff );

    cout << "  " << setw(4)  << itnew
         << "  " << setw(14) << unew_norm
         << "  " << setw(14) << diff
         << "  " << setw(14) << error << "\n";

    if ( diff <= tolerance )
    {
      converged = true;
      break;
    }

  }

  if ( converged )
  {
    cout << "  The iteration has converged.\n";
  }
  else
  {
    cout << "  The iteration has NOT converged.\n";
  }

  wtime = omp_get_wtime ( ) - wtime;
  cout << "\n";
  cout << "  Elapsed seconds = " << wtime << "\n";
//
//  Terminate.
//
#ifdef __USE_MPI
 MPI_Finalize();
#endif
  cout << "\n";
  cout << "POISSON_OPENMP:\n";
  cout << "  Normal end of execution.\n";
  cout << "\n";
  timestamp ( );

  return 0;
}

# undef NX
# undef NY
