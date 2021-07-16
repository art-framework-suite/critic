cet_enable_asserts()
# Make sure tests have correct environment settings.
include(CetTest)
cet_test_env("FHICL_FILE_PATH=.")

set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(CLHEP PRIVATE COMPONENTS Random REQUIRED)
find_package(ROOT PRIVATE COMPONENTS Core RIO Tree REQUIRED)
find_package(Threads PRIVATE REQUIRED)
find_package(art REQUIRED PUBLIC)
find_package(art_root_io PRIVATE REQUIRED)
find_package(canvas REQUIRED PUBLIC)
find_package(cetlib PRIVATE REQUIRED)
find_package(fhiclcpp PRIVATE REQUIRED)
find_package(gallery PRIVATE REQUIRED)
find_package(messagefacility PRIVATE REQUIRED)

include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# Put everything in a different export set.
cet_register_export_set(SET_NAME Testing NAMESPACE critic_test SET_DEFAULT)

# build Persistency libraries
add_subdirectory(CriticTestObjects)
add_subdirectory(art)
add_subdirectory(gallery)
