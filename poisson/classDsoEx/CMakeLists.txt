cmake_minimum_required (VERSION 2.8)
project (poison_omp)


add_library( solver SHARED jacobi.cpp vectorOps.cpp )
add_executable(poisson main.cpp)
target_link_libraries(poisson LINK_PUBLIC solver)
