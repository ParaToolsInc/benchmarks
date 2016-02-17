cmake_minimum_required (VERSION 3.0)
project (PSUM)


add_executable(psum_serial psum_serial.cpp)
add_executable(psum_stdVec psum_stdVec.cpp)
#set_target_properties( psum_stdVec PROPERTIES COMPILE_FLAGS "-Wc++11-extensions")

