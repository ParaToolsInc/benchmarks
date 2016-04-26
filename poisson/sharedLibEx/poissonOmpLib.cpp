# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <cmath>
# include <omp.h>

using namespace std;




//****************************************************************************80

double r8mat_rms ( int nx, int ny, double a[NX][NY] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_RMS returns the RMS norm of a vector stored as a matrix.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NX, NY, the number of rows and columns in A.
//
//    Input, double A[NX][NY], the vector.
//
//    Output, double R8MAT_RMS, the root mean square of the entries of A.
//
{
  int i;
  int j;
  double v;

  v = 0.0;

  for ( j = 0; j < ny; j++ )
  {
    for ( i = 0; i < nx; i++ )
    {
      v = v + a[i][j] * a[i][j];
    }
  }
  v = sqrt ( v / ( double ) ( nx * ny )  );

  return v;
}
//****************************************************************************80

void rhs ( int nx, int ny, double f[NX][NY] )

//****************************************************************************80
//
//  Purpose:
//
//    RHS initializes the right hand side "vector".
//
//  Discussion:
//
//    It is convenient for us to set up RHS as a 2D array.  However, each
//    entry of RHS is really the right hand side of a linear system of the
//    form
//
//      A * U = F
//
//    In cases where U(I,J) is a boundary value, then the equation is simply
//
//      U(I,J) = F(i,j)
//
//    and F(I,J) holds the boundary data.
//
//    Otherwise, the equation has the form
//
//      (1/DX^2) * ( U(I+1,J)+U(I-1,J)+U(I,J-1)+U(I,J+1)-4*U(I,J) ) = F(I,J)
//
//    where DX is the spacing and F(I,J) is the value at X(I), Y(J) of
//
//      pi^2 * ( x^2 + y^2 ) * sin ( pi * x * y )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NX, NY, the X and Y grid dimensions.
//
//    Output, double F[NX][NY], the initialized right hand side data.
//
{
  double fnorm;
  int i;
  int j;
  double x;
  double y;
//
//  The "boundary" entries of F store the boundary values of the solution.
//  The "interior" entries of F store the right hand sides of the Poisson equation.
//
  for ( j = 0; j < ny; j++ )
  {
    y = ( double ) ( j ) / ( double ) ( ny - 1 );
    for ( i = 0; i < nx; i++ )
    {
      x = ( double ) ( i ) / ( double ) ( nx - 1 );
      if ( i == 0 || i == nx - 1 || j == 0 || j == ny - 1 )
      {
        f[i][j] = u_exact ( x, y );
      }
      else
      {
        f[i][j] = - uxxyy_exact ( x, y );
      }
    }
  }

  fnorm = r8mat_rms ( nx, ny, f );

  cout << "  RMS of F = " << fnorm << "\n";

  return;
}
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

void timestamp ( )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 July 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
//****************************************************************************80

double u_exact ( double x, double y )

//****************************************************************************80
//
//  Purpose:
//
//    U_EXACT evaluates the exact solution.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, Y, the coordinates of a point.
//
//    Output, double U_EXACT, the value of the exact solution
//    at (X,Y).
//
{
  double pi = 3.141592653589793;
  double value;

  value = sin ( pi * x * y );

  return value;
}
//****************************************************************************80

double uxxyy_exact ( double x, double y )

//****************************************************************************80
//
//  Purpose:
//
//    UXXYY_EXACT evaluates ( d/dx d/dx + d/dy d/dy ) of the exact solution.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, Y, the coordinates of a point.
//
//    Output, double UXXYY_EXACT, the value of
//    ( d/dx d/dx + d/dy d/dy ) of the exact solution at (X,Y).
//
{
  double pi = 3.141592653589793;
  double value;

  value = - pi * pi * ( x * x + y * y ) * sin ( pi * x * y );

  return value;
}

