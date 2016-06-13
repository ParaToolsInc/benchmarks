import mm
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.rank
size = comm.size

comm.Barrier()
t_start = MPI.Wtime()
mm.mm()
comm.Barrier()
t_diff = MPI.Wtime() - t_start
print 'runtime on rank %d of %d is %f' %(rank, size, t_diff)

comm.Barrier()
print 'Executing 1 fortran timestep on rank %d of %d.' %(rank, size)
mm.timestep(1)
comm.Barrier()
