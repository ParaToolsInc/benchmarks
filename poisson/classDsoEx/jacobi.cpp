# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <cmath>
# include <omp.h>

using namespace std;
# define NX 161
# define NY 161

double u_exact ( double x, double y );
double uxxyy_exact ( double x, double y );



//****************************************************************************80

void sweep ( int nx, int ny, double dx, double dy, double f[NX][NY],
  int itold, int itnew, double u[NX][NY], double unew[NX][NY] )

//****************************************************************************80
//
//  Purpose:
//
//   SWEEP carries out one step of the Jacobi iteration.
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
//  Parameters:
//
//    Input, int NX, NY, the X and Y grid dimensions.
//
//    Input, double DX, DY, the spacing between grid points.
//
//    Input, double F[NX][NY], the right hand side data.
//
//    Input, int ITOLD, the iteration index on input.
//
//    Input, int ITNEW, the desired iteration index
//    on output.
//
//    Input, double U[NX][NY], the solution estimate on
//    iteration ITNEW-1.
//
//    Input/output, double UNEW[NX][NY], on input, the solution
//    estimate on iteration ITOLD.  On output, the solution estimate on
//    iteration ITNEW.
//
{
  int i;
  int it;
  int j;

# pragma omp parallel shared ( dx, dy, f, itnew, itold, nx, ny, u, unew ) private ( i, it, j )

  for ( it = itold + 1; it <= itnew; it++ )
  {
//
//  Save the current estimate.
//
# pragma omp for
    for ( j = 0; j < ny; j++ )
    {
      for ( i = 0; i < nx; i++ )
      {
        u[i][j] = unew[i][j];
      }
    }
//
//  Compute a new estimate.
//
# pragma omp for
    for ( j = 0; j < ny; j++ )
    {
      for ( i = 0; i < nx; i++ )
      {
        if ( i == 0 || j == 0 || i == nx - 1 || j == ny - 1 )
        {
          unew[i][j] = f[i][j];
        }
        else
        {
          unew[i][j] = 0.25 * (
            u[i-1][j] + u[i][j+1] + u[i][j-1] + u[i+1][j] + f[i][j] * dx * dy );
        }
      }
    }

  }
  return;
}
//****************************************************************************80


# undef NX
# undef NY
