rm -rf CMakeFiles CMakeCache.txt

cmake . \
-DCMAKE_C_COMPILER=cc \
-DCMAKE_CXX_COMPILER=CC \
-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -qopenmp -g -O3 -D__USE_MPI"
