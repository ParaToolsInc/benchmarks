#Makefile for driver directory of SAMARC

# Set to 1 to build with TAU
TAU=0

ifeq ($(TAU),1)
CXX           = tau_cxx.sh
CC            = tau_cc.sh
MPIF90        = tau_f90.sh
else
CXX           = mpiicpc
CC            = mpiicc
MPIF90        = mpiifort
endif

#### Defns ##########
CXXFLAGS      = -fPIC -fpermissive -g
#CPPFLAGS      = -I/opt/mpi/x86_64/intel/9.1/openmpi-1.2/include

CFLAGS        = -fPIC -g

F90           = ifort
FFLAGS        = -fPIC -g -O0

PY_INCL       = -I/usr/include/python2.6 -I/usr/lib64/python2.6/site-packages/numpy/core/include

#### Codes ##########
CXX_OBJS  = SAMINT.o

F90_OBJS  = fortmthds.o

PY_OBJS   = pycintfc.o samint_wrap.o

#### Rules ##########
all:	$(CXX_OBJS) $(F90_OBJS) $(PY_OBJS) linkcheck.o
	$(CXX) -fPIC -shared $(PY_OBJS) $(CXX_OBJS) $(F90_OBJS) -o _samint.so -L/opt/intel/2011.0.013/composerxe-2011.0.084/compiler/lib/intel64 -lifcore
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CXX_OBJS) $(F90_OBJS) \
#	linkcheck.o $(LDLIBS) -o linkcheck 

pycintfc.o: pycintfc.C
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c pycintfc.C $(PY_INCL) 

samint_wrap.o: samint_wrap.c
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c samint_wrap.c $(PY_INCL) 

samint_wrap.c: swig/samint.i
	cd swig; $(MAKE) all

linkcheck.o: linkcheck.C
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c linkcheck.C 

fortmthds.o: fortmthds.f
	$(F90) $(FFLAGS) -c fortmthds.f


#### clean ##########
cleanc:
	$(RM) *.o 

clean:
	$(RM) samint.py samint_wrap.c _samint.so
	$(RM) *.o *.pyc *~
	cd swig; $(MAKE) $@
