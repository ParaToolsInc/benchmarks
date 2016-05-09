# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <cmath>
# include <omp.h>

using namespace std;
# define NX 161
# define NY 161

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
