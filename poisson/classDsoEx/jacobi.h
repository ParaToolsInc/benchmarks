# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <cmath>
# include <omp.h>

# define NX 161
# define NY 161

using namespace std;


class myJacobi
{
public:
 void sweep ( int nx, int ny, double dx, double dy, double f[NX][NY],
    int itold, int itnew, double u[NX][NY], double unew[NX][NY] );
};
