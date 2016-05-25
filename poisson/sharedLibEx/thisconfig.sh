rm -rf CMakeFiles CMakeCache.txt

cmake . \
-DCMAKE_C_COMPILER=cc \
-DCMAKE_CXX_COMPILER=CC \
-DCMAKE_CXX_FLAGS="-D__USE_MPI ${CMAKE_CXX_FLAGS} -openmp -g -O3"
