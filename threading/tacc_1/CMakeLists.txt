cmake_minimum_required (VERSION 3.0)
project (PSUM)


  set (TBB_INCDIRS /opt/apps/intel/15/composer_xe_2015.2.164/tbb/include )
  set (TBB_LIBDIRS -L/opt/apps/intel/15/composer_xe_2015.2.164/tbb/lib/intel64/gcc4.1
	-ltbb -ltbbmalloc -ltbbmalloc_proxy )
add_executable(psum_serial psum_serial.cpp)
add_executable(psum_stdVec psum_stdVec.cpp)
add_executable(psum_openmp psum_openmp.cpp)
add_executable(psum_tbb_singleBuffer psum_tbb_singleBuffer.cpp)
add_executable(psum_tbb_rankBuffer psum_tbb_rankBuffer.cpp)
add_executable(psum psum.cpp)

target_link_libraries(psum_stdVec -pthread)

target_include_directories (psum_tbb_singleBuffer PUBLIC ${TBB_INCDIRS})
target_include_directories (psum_tbb_rankBuffer PUBLIC ${TBB_INCDIRS})

target_link_libraries(psum ${TBB_LIBDIRS} -openmp -pthread)
set_target_properties( psum PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -openmp -O3 -std=c++11")

target_link_libraries(psum_openmp -openmp )
set_target_properties( psum_openmp PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -openmp -O3 -std=c++11")

target_link_libraries(psum_tbb_singleBuffer  ${TBB_LIBDIRS} )
set_target_properties( psum_tbb_singleBuffer PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")

target_link_libraries(psum_tbb_rankBuffer  ${TBB_LIBDIRS} )
set_target_properties( psum_tbb_rankBuffer PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")



