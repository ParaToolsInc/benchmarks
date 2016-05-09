cmake_minimum_required (VERSION 2.8)
project (poison_omp)


add_library( vectorOps SHARED vectorOps.cpp )
add_library( jacobi SHARED jacobi.cpp )
add_library( timeSoln STATIC timeSoln.cpp )
add_executable(poisson main.cpp)
target_link_libraries(poisson LINK_PUBLIC vectorOps jacobi)
