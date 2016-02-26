rm -rf CMakeCache.txt CMakeFiles/
export PATH=/opt/apps/gcc/4.9.1/bin:$PATH
export LD_LIBRARY_PATH=/opt/apps/gcc/4.9.1/lib:opt/apps/gcc/4.9.1/lib64:$LD_LIBRARY_PATH
export PATH=/work/02463/srinathv/maverick/tau2/x86_64/bin:$PATH
export TAU_MAKEFILE=/work/02463/srinathv/maverick/tau2/x86_64/lib/Makefile.tau-intel15-t1024-icpc-papi-pthread-profile-trace
export TAU_OPTIONS="-optRevert -optNoCompInst"
cmake \
  -DCMAKE_CXX_COMPILER:FILEPATH=tau_cxx.sh \
  -DCMAKE_C_COMPILER:FILEPATH=tau_cc.sh \
  -DCMAKE_CXX_FLAGS="-DTAU_ENABLED ${CMAKE_CXX_FLAGS} -g -std=c++11 -DTAU_STDCXXLIB" \
/work/02463/srinathv/maverick/benchmarks-paratools/threading/tacc_1


export TAU_OPTIONS="-optNoRevert -optLinkOnly"
make psum_tbb_rankBuffer VERBOSE=1 2>&1 | tee build.out

