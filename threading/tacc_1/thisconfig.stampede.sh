rm -rf CMakeCache.txt CmakeFiles

cmake \
  -DCMAKE_CXX_COMPILER:FILEPATH=icpc \
  -DCMAKE_C_COMPILER:FILEPATH=icc \
  -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -g -std=c++11 " \
/work/02463/srinathv/benchmarks/threading/tacc_1
