# - Try to find TBB
# Once done this will define
#  TBB_FOUND - System has TBB
#  TBB_INCLUDE_DIRS - The TBB include directories
#  TBB_LIBRARY_DIRS - The pion library directories
#  TBB_LIBRARIES - The libraries needed to use TBB

# find_package (PkgConfig)
# pkg_check_modules (PC_LIBTBB QUIET libpion)
set (TBB_DEFINITIONS ${PC_LIBTBB_CFLAGS_OTHER})
#set (EXT_LIBLOC_TBB ${CMAKE_SOURCE_DIR}/ext_library/linux/x86_64/pion-net-4.0.5)

find_path (TBB_INCLUDE_DIRS
  NAMES tbb/tbb_config.h
  HINTS $ENV{TBBROOT}/include $ENV{INCLUDE}
  )


find_library (TBB_LIBRARY
  NAMES tbb tbbmalloc tbbmalloc_proxy
  HINTS $ENV{TBBROOT}/lib/intel64/gcc4.4 $ENV{LIBRARY_PATH} $ENV{LD_LIBRARY_PATH} 
)


set (TBB_LIBRARIES ${TBB_LIBRARY})
foreach(_tbb_my_lib ${TBB_LIBRARY})
  get_filename_component(_tbb_my_lib_path "${_tbb_my_lib}" PATH)
  list(APPEND TBB_LIBRARY_DIRS ${_tbb_my_lib_path})
endforeach()

include (FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set TBB_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args (TBB DEFAULT_MSG
  TBB_INCLUDE_DIRS
  TBB_LIBRARY_DIRS 
  TBB_LIBRARIES
  TBB_LIBRARY)

mark_as_advanced (TBB_LIBRARY)

