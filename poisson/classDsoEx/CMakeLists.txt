cmake_minimum_required (VERSION 2.8)
project (poison_omp)


set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include_directories( ${PROJECT_SOURCE_DIR})

add_library( vectorOps SHARED vectorOps.cpp )
add_library( jacobi SHARED jacobi.cpp )
add_library( timeSoln STATIC timeSoln.cpp )
add_executable(poisson main.cpp)

target_link_libraries(vectorOps timeSoln)
target_link_libraries(jacobi timeSoln)

target_link_libraries(poisson LINK_PUBLIC vectorOps jacobi timeSoln)
