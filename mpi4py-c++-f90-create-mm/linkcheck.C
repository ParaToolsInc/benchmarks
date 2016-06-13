//
// File:        main.C
// Description: Main program to test SAMRAI-rotor integration
//
#include <cstdlib>
#include <iostream>
#define MPICH_SKIP_MPICXX
#include <mpi.h>

#ifndef included_pycintfc
#include "pycintfc.C"
#endif

int main( int argc, char *argv[] ) {

   // initialize MPI
   MPI_Init(&argc, &argv);

   /*
    * Initialize SAMARC - read input file and build initial grid system
    */
   // process command line
   string input_file;
   int nsteps;
   if (argc < 3) {
      cout << "USAGE:  " << argv[0] << " <input file> <nsteps>"
             << endl;
      return (-1);
   } else {
      input_file = argv[1];
      nsteps = atoi(argv[2]);
   }


   int step = 0;
   double time = 0.0;
   double dt = 0.1;

   int err = samarcInit(input_file.c_str());

   // output plot file
   samarcWritePlotData(time,step);

   // output grid system
   cout << "global ngrids = " << ngrids << endl;
   cout << "local  ngrids = " << nlocal << endl;   
   printGrids(ngrids,obgrids);
   printLocalData(nlocal,obdata);

   /*
    * Advance n timesteps
    */
   for (int i = 0; i < nsteps; i++) {
      cout << "\nstep: " << step << "\ttime: " << time << endl;
      err = samarcStep(time,dt);
      step += 1;
      time += dt;
      samarcWritePlotData(time,step);
   }
   
   
   /*
    * Regrid
    */
   cout << "\n\nREGRID\ttime: " << time << "\n\n" << endl;
   err = samarcRegrid(time);

   // output grid system
   cout << "global ngrids = " << ngrids << endl;
   cout << "local  ngrids = " << nlocal << endl;   
   printGrids(ngrids,obgrids);
   printLocalData(nlocal,obdata);

   
   /*
    * Advance n more timesteps
    */
   for (int i = 0; i < nsteps; i++) {
      cout << "\nstep: " << step << "\ttime: " << time << endl;
      err = samarcStep(time,dt);
      step += 1;
      time += dt;
      samarcWritePlotData(time,step);
   }

   /*
    * Clean up
    */
   err = samarcFinish();
   
   // finalize MPI
   MPI_Finalize();
 
   return(0); 
}


