# Client maintainer: chuck.atkins@kitware.com
set(ENV{CC}  clang)
set(ENV{CXX} clang++)
set(ENV{FC}  gfortran)
set(ENV{CFLAGS}   "-Werror -Wall")
set(ENV{CXXFLAGS} "-Werror -Wall")
set(ENV{FFLAGS}   "-Werror -Wall")

set(dashboard_cache "
ADIOS2_USE_Fortran:BOOL=ON
ADIOS2_USE_MPI:BOOL=OFF
ADIOS2_USE_Python:BOOL=ON
")

set(ENV{MACOSX_DEPLOYMENT_TARGET} "10.14")
set(CTEST_CMAKE_GENERATOR "Ninja")
list(APPEND CTEST_UPDATE_NOTES_FILES "${CMAKE_CURRENT_LIST_FILE}")
include(${CMAKE_CURRENT_LIST_DIR}/ci-common.cmake)
