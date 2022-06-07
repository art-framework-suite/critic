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

# build Persistency libraries
add_subdirectory(CriticTestObjects)
add_subdirectory(art)
add_subdirectory(gallery)
