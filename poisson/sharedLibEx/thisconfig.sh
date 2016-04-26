rm -rf CMakeFiles CMakeCache.txt

cmake . \
-DCMAKE_C_COMPILER=icc \
-DCMAKE_CXX_COMPILER=icpc \
-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -openmp"
