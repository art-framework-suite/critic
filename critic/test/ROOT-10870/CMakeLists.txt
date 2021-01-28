cmake_policy(PUSH)
cmake_minimum_required(VERSION 3.14..3.18 FATAL_ERROR)

find_package(cetmodules 2.01.00 REQUIRED)
project(r10870 LANGUAGES CXX C)

include(CetCMakeEnv)
cet_cmake_env()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Modules) ### MIGRATE-NO-ACTION

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  EXTRA_CXX_FLAGS -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -DBOOST_TEST_NO_OLD_TOOLS)

cet_report_compiler_flags()

cet_find_package(ROOT 6.20.00 REQUIRED HINTS $ENV{ROOTSYS})

include(ArtDictionary)
include(ArtMake)

cet_enable_asserts()
include(CetTest)

art_make_library(LIBRARY_NAME ${PROJECT_NAME}
  SOURCE DataObj.cc art_Exception.cc cet_exception.cc
  NO_INSTALL)
art_dictionary(${PROJECT_NAME}
  NO_DEFAULT_LIBRARIES NO_INSTALL
  DICT_NAME_VAR dictname
  DICTIONARY_LIBRARIES ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}_dict
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

add_custom_target(${PROJECT_NAME}-reproducer_tar
  COMMAND ${CMAKE_COMMAND} -E rm -rf "${PROJECT_NAME}-reproducer"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}"
  "${PROJECT_NAME}-reproducer"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "$ENV{CETMODULES_DIR}"
  "${PROJECT_NAME}-reproducer/cetmodules/$ENV{CETMODULES_VERSION}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "$ENV{CETPKGSUPPORT_DIR}"
  "${PROJECT_NAME}-reproducer/cetpkgsupport/$ENV{CETPKGSUPPORT_VERSION}"
  COMMAND ${CMAKE_COMMAND} -E tar jcf "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-reproducer.tar.bz2" "${PROJECT_NAME}-reproducer"
  COMMENT "Generating self-contained archive ${PROJECT_NAME}-reproducer.tar.bz2"
  )

cmake_policy(POP)
