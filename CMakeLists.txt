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


# use cmake 3.11 or later
cmake_minimum_required (VERSION 3.11...3.14 FATAL_ERROR)

project(critic VERSION 1.03.38 LANGUAGES CXX)

# cetmodules contains our cmake modules
find_package(cetmodules 1.01.01 REQUIRED)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  EXTRA_CXX_FLAGS -Wnon-virtual-dtor -Wdelete-non-virtual-dtor)

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_package(hep_concurrency REQUIRED)
find_package(cetlib_except REQUIRED)
find_package(cetlib REQUIRED)
find_package(fhiclcpp REQUIRED)
find_package(canvas REQUIRED)
find_package(art REQUIRED)
find_package(art_root_io REQUIRED)
find_package(gallery REQUIRED)
find_package(canvas_root_io REQUIRED)
find_package(Boost REQUIRED COMPONENTS)
find_package(ROOT REQUIRED COMPONENTS)
find_package(CLHEP REQUIRED COMPONENTS)

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(BuildPlugins)

add_subdirectory(critic)

# ups - table and config files
add_subdirectory(cmakeConfig)
