#!/bin/bash
#SBATCH -J uuid1 # job name
#SBATCH -o uuid1.o%j       # output and error file name (%j expands to jobID)
#SBATCH -n 1              # total number of mpi tasks requested
#SBATCH -p gpu # queue (partition) -- normal, development, etc.
##SBATCH -p gpu # queue (partition) -- normal, development, etc.
#SBATCH -t 01:00:00        # run time (hh:mm:ss) - 1.5 hours
#SBATCH --mail-user=srinathv@paratools.com
#SBATCH --mail-type=begin  # email me when the job starts
#SBATCH --mail-type=end    # email me when the job finishes
#SBATCH -A GraviT



#ibrun -n 1 -o 0 ./runTest_1.py
ibrun -n 1 -o 0 ./psum 2>&1 > maverick_psum.txt
