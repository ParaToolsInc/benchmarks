rm -rf CMakeCache.txt CmakeFiles

cmake \
  -DCMAKE_CXX_COMPILER:FILEPATH=g++ \
  -DCMAKE_C_COMPILER:FILEPATH=gcc \
  -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -g -std=c++11 " \
  /Users/srinathv/Repos/GitHub/benchmarks/threading/tacc_1

