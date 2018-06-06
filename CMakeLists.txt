# ======================================================================
#  critic main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/critic/ups/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================

# Required to keep CMake happy.
cmake_minimum_required(VERSION ${CMAKE_VERSION} FATAL_ERROR)
# Actual version requirement.
cmake_policy(VERSION 3.4)

# Project information.
project(critic VERSION 1.03.37 LANGUAGES CXX)

# cetmodules contains our cmake modules
find_package(cetmodules REQUIRED)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  EXTRA_CXX_FLAGS -Wnon-virtual-dtor -Wdelete-non-virtual-dtor)

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_package(canvas REQUIRED)
find_package(art REQUIRED)
find_package(gallery REQUIRED)
find_package(ROOT REQUIRED COMPONENTS Core RIO)

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(BuildPlugins)

add_subdirectory(critic)

# ups - table and config files
add_subdirectory(ups)
