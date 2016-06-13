#Makefile for driver directory of SAMARC

CXX           = mpiicpc
CC            = mpiicpc
MPIF90        = 

#### Defns ##########
CXXFLAGS      = -fPIC -fpermissive -g
#CPPFLAGS      = -I/opt/mpi/x86_64/intel/9.1/openmpi-1.2/include

CFLAGS        = -fPIC -g

F90           = ifort
FFLAGS        = -fPIC -g 

PY_INCL       = -I/usr/include/python2.6 -I/usr/lib64/python2.6/site-packages/numpy/core/include

#### Codes ##########
CXX_OBJS  = mm.o

F90_OBJS  = fortmthds.o

PY_OBJS   = mm_wrap.o

#### Rules ##########
all:	$(CXX_OBJS) $(F90_OBJS) $(PY_OBJS)
	$(CXX) -fPIC -shared $(PY_OBJS) $(CXX_OBJS) $(F90_OBJS) -o _mm.so -L/opt/intel/2011.0.013/composerxe-2011.0.084/compiler/lib/intel64 -lifcore
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CXX_OBJS) $(F90_OBJS) \
#	linkcheck.o $(LDLIBS) -o linkcheck 


mm_wrap.o: mm_wrap.c
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c mm_wrap.c $(PY_INCL) 

fortmthds.o: fortmthds.f
	$(F90) $(FFLAGS) -c fortmthds.f

mm_wrap.c: swig/mm.i
	cd swig; $(MAKE) all


#### clean ##########
cleanc:
	$(RM) *.o 

clean:
	$(RM) mm.py mm_wrap.c _mm.so
	$(RM) *.o *.pyc *~
	cd swig; $(MAKE) $@
