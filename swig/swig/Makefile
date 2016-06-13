#Makefile for driver directory of SAMARC

SWIG_LIB=/packages/swig/3.0.6//share/swig/2.0.9

CP    = /bin/cp

CXX_OBJS  = mm_wrap.o
 
F77_OBJS  = 

all:    mm.i
	swig -python -I../  -I$(SWIG_LIB) -I$(SWIG_LIB)/python mm.i
	$(CP) mm.py mm_wrap.c ../

clean:
	$(RM) *.o *.c *.py *~
