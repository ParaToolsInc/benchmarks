export PATH=/opt/apps/gcc/4.9.1/bin:$PATH
export LD_LIBRARY_PATH=/opt/apps/gcc/4.9.1/lib:/opt/apps/gcc/4.9.1/lib64:$LD_LIBRARY_PATH
export PATH=/work/02463/srinathv/maverick/tau2/x86_64/bin:$PATH
export TAU_MAKEFILE=/work/02463/srinathv/maverick/tau2/x86_64/lib/Makefile.tau-intel15-t1024-icpc-papi-pthread-profile-trace
export TAU_PROFILE=1
export TAU_TRACE=0
export TAU_METRICS=TIME:PAPI_L1_DCM

./psum_tbb_rankBuffer 16
