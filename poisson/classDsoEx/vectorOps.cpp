
#include "vectorOps.h"

//****************************************************************************80

void myVector::rhs ( int nx, int ny, double f[NX][NY] )

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



# undef NX
# undef NY
