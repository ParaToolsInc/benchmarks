cmake_minimum_required (VERSION 3.0)
project (PSUM)


  set (TBB_INCDIRS -I/opt/apps/intel/15/composer_xe_2015.2.164/tbb/include )
  set (TBB_LIBDIRS -L/opt/apps/intel/15/composer_xe_2015.2.164/tbb/lib/intel64/gcc4.1 )
add_executable(psum_serial psum_serial.cpp)
add_executable(psum_stdVec psum_stdVec.cpp)
#set_target_properties( psum_stdVec PROPERTIES COMPILE_FLAGS "-Wc++11-extensions")
add_executable(psum psum.cpp)
set_target_properties( psum PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -openmp -O3 -std=c++11 ${TBB_INCDIRS} ${TBB_LIBDIRS} -lpthread -ltbb -ltbbmalloc -ltbbmalloc_proxy")

