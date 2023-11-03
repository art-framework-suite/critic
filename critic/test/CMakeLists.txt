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
  cet_test_env_prepend(CET_PLUGIN_PATH
    ${PROJECT_BINARY_DIR}/${critic_LIBRARY_DIR}
    $<TARGET_FILE_DIR:art_root_io::RootOutput_module>
    $<TARGET_FILE_DIR:art::Framework_Services_System_FileCatalogMetadata_service>
    $<TARGET_FILE_DIR:messagefacility::plugins_cerr_mfPlugin>)

  cet_test_env_prepend(LD_LIBRARY_PATH
    ${PROJECT_BINARY_DIR}/${critic_LIBRARY_DIR}
    $<TARGET_FILE_DIR:canvas_root_io::Dictionaries_canvas_Provenance_dict>)

  cet_test_env_prepend(ROOT_INCLUDE_PATH
    ${CMAKE_SOURCE_DIR}/critic
    ${CMAKE_SOURCE_DIR}/art
    ${CMAKE_SOURCE_DIR}/canvas
    ${CMAKE_SOURCE_DIR}/fhicl-cpp
    ${CMAKE_SOURCE_DIR}/cetlib
    ${CMAKE_SOURCE_DIR}/cetlib-except)

  add_subdirectory(art)
  add_subdirectory(gallery)
endif()
