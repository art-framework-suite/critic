cet_enable_asserts()
# Make sure tests have correct environment settings.
include(CetTest)
cet_test_env("FHICL_FILE_PATH=.")

set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(CLHEP COMPONENTS Random REQUIRED)
find_package(ROOT COMPONENTS Core RIO Tree REQUIRED)
find_package(Threads REQUIRED)
find_package(art REQUIRED EXPORT)
find_package(art_root_io REQUIRED)
find_package(canvas REQUIRED EXPORT)
find_package(cetlib REQUIRED)
find_package(fhiclcpp REQUIRED)
find_package(gallery REQUIRED)
find_package(messagefacility REQUIRED)

include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# Put everything in a different export set.
cet_register_export_set(SET_NAME Testing NAMESPACE critic_test SET_DEFAULT)

# Build Persistency libraries
add_subdirectory(CriticTestObjects)

if (BUILD_TESTING)
  cet_transitive_paths(LIBRARY_DIR BINARY IN_TREE)
  cet_test_env_prepend(CET_PLUGIN_PATH ${TRANSITIVE_PATHS_WITH_LIBRARY_DIR})
  cet_test_env_prepend(LD_LIBRARY_PATH ${TRANSITIVE_PATHS_WITH_LIBRARY_DIR})

  cet_transitive_paths(SOURCE_DIR IN_TREE)
  cet_test_env_prepend(ROOT_INCLUDE_PATH ${TRANSITIVE_PATHS_WITH_SOURCE_DIR})

  add_subdirectory(art)
  add_subdirectory(gallery)
endif()
