# Required to keep CMake happy.
cmake_minimum_required(VERSION 3.18.2 FATAL_ERROR)

# Find cetmodules macros and functions.

# Project information, including version and required languages.
find_package(cetmodules 2.01.00 REQUIRED)
project(critic VERSION 2.04.03 LANGUAGES CXX C)

####################################
# Set any project configuration variables here.
set(${PROJECT_NAME}_OLD_STYLE_CONFIG_VARS TRUE)

# Initialize the cetmodules build environment.
include(CetCMakeEnv)
cet_cmake_env()

# Compiler flag settings.
cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic)

# Compiler flag settings.
cet_report_compiler_flags(REPORT_THRESHOLD VERBOSE)

####################################
# Dependencies.

find_ups_product(art) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(art_root_io) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(gallery) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(canvas) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(messagefacility) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(fhiclcpp) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(cetlib) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(cetlib_except) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(clhep) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(canvas_root_io) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(hep_concurrency) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(range) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_product(sqlite) ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking
find_ups_root()
find_ups_boost(BOOST_TARGETS)

# SQLite
cet_find_library(SQLITE3 NAMES sqlite3_ups PATHS ENV SQLITE_LIB NO_DEFAULT_PATH) ### MIGRATE-ACTION-RECOMMENDED: use of ENV ... may be UPS-dependent ### MIGRATE-ACTION-RECOMMENDED: use cet_find_package() with target semantics for linking

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(BuildPlugins)

add_subdirectory(critic)

####################################
# Generate CMake (and UPS, if appropriate) configuration files and
# packaging.
cet_cmake_config(COMPATIBILITY SameMajorVersion)
