# Makefile for GFMCmk stassuij

FF = ifort
F90 = ifort
LD  = ifort

FFLAGS = -O3 -xHost -ipo
FNOOPTFLAGS = $(FCOMMON) -O2

LDFLAGS = $(FNOOPTFLAGS)

LIBS = -L$(HOME)/lib -ltimebase

OBJS = Main.o stassuij.o stassterms.o

all: stassuij

Main.o: Main.F90
	$(F90) $(FNOOPTFLAGS) -c -o Main.o Main.F90

stassuij.o: stassuij.f
	$(FF) $(FFLAGS) -c -o stassuij.o stassuij.f

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


