
# define NX 161
# define NY 161

# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <cmath>
# include <omp.h>

using namespace std;
double r8mat_rms ( int m, int n, double a[NX][NY] );
double u_exact ( double x, double y );
double uxxyy_exact ( double x, double y );

class myVector {
  public:

void rhs ( int nx, int ny, double f[NX][NY] );

};
