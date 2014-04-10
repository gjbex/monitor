CC=icc
CFLAGS=-O2
F90=ifort
FFLAGS=-O2
PAR=-openmp
LDFLAGS=-lm
MKLROOT=/apps/leuven/intel/cluster_studio/2011.0.013/mkl/
MKL_SEQ= -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread

all: dgemm fib exit-code forker pthreads

forker: forker.pl dgemm
	cp forker.pl forker
	chmod u+x forker

exit-code: exit-code.c
	$(CC) $(CFLAGS) -o exit-code exit-code.c

fib: fib-openmp.c
	$(CC) $(CFLAGS) $(PAR) -o fib fib-openmp.c

dgemm: dgemm.f90
	$(F90) $(FFLAGS) -r8 -o dgemm dgemm.f90 $(LDFLAGS) $(MKL_SEQ)

pthreads: pthreads.c
	$(CC) $(CFLAGS) -o pthreads pthreads.c -lpthread

clean:
	rm -f *.o *.mod dgemm fib exit-code forker pthreads

