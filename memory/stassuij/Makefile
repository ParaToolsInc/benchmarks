# Makefile for GFMCmk stassuij

FF = bgxlf_r
F90 = bgxlf90_r
LD  = bgxlf90_r

FFLAGS = -O3 -qhot=level=1 -qsmp=omp:noauto -qdpc=e -qlist -qsource -qreport=hotlist,smplist 
FNOOPTFLAGS = $(FCOMMON) -O2 -qsmp=omp

LDFLAGS = $(FNOOPTFLAGS)

LIBS = -L$(HOME)/lib -ltimebase

OBJS = Main.o stassuij.o stassterms.o 

all: stassuij

Main.o: Main.F90
	$(F90) -qsuffix=cpp=F90 -d $(FNOOPTFLAGS) -c -o Main.o Main.F90 

stassuij.o: stassuij.f
	$(FF) $(FFLAGS) -qfixed=130 -c -o stassuij.o stassuij.f

stassterms.o: stassterms.f90
	$(FF) -O2 -c -o stassterms.o stassterms.f90

%.o: %.f90
	$(F90) $(FFLAGS) -c $<

%.o: %.F90
	$(F90) $(FFLAGS) -c $< 

%.o: %.f
	$(F90) $(FFLAGS) $(F77FIXED) -o $@ -c $< 

%.o: %.c
	$(CC) -c -o $@ $< 

stassuij: $(OBJS) 
	$(LD) $(LDFLAGS) -o stassuij $(OBJS) $(LIBS)

clean: 
	rm -f temp* *.d *.o *.mod stassuij *.lst FMain.f


