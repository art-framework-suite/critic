# Set the test environment so that pyROOT and interactive ROOT can find all necessary headers
#
if (DEFINED ENV{MRB_BUILDDIR})
  # This first clause should be redundant when cetbuildtools goes to v6.
  cet_test_env(ROOT_INCLUDE_PATH=$ENV{MRB_BUILDDIR}/canvas:$ENV{ROOT_INCLUDE_PATH})
else()
  cet_test_env(ROOT_INCLUDE_PATH=${PROJECT_SOURCE_DIR}:$ENV{ROOT_INCLUDE_PATH})
endif()

set(gallery_tests_libraries
  critic_test_CriticTestObjects
  gallery
  canvas
  fhiclcpp
  cetlib
  cetlib_except
  ${ROOT_RIO}
  ${ROOT_CORE}
  )

########################################################################
# Make a hierarchy of input files in order to test gallery in C++,
# interpreted C++ and python.
cet_test(gallery_makeInput1 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput1.fcl
  DATAFILES
  fcl/test_gallery_makeInput1.fcl
  )

# Uses output from gallery_makeInput1.
cet_test(gallery_makeInput2 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput2.fcl ../gallery_makeInput1.d/test_gallery1.root
  DATAFILES
  fcl/test_gallery_makeInput2.fcl
  REQUIRED_FILES ../gallery_makeInput1.d/test_gallery1.root
  TEST_PROPERTIES DEPENDS gallery_makeInput1
  )

# Uses output from gallery_makeInput2.
cet_test(gallery_makeInput3 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput3.fcl ../gallery_makeInput2.d/test_gallery2.root
  DATAFILES
  fcl/test_gallery_makeInput3.fcl
  REQUIRED_FILES ../gallery_makeInput2.d/test_gallery2.root
  TEST_PROPERTIES DEPENDS gallery_makeInput2
  )

# Uses output from gallery_makeInput2 with a differently-configured module.
cet_test(gallery_makeInput4 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput4.fcl ../gallery_makeInput2.d/test_gallery2.root
  DATAFILES
  fcl/test_gallery_makeInput4.fcl
  REQUIRED_FILES ../gallery_makeInput2.d/test_gallery2.root
  TEST_PROPERTIES DEPENDS gallery_makeInput2
  )

# Uses output from gallery_makeInput3 and gallery_makeInput4.
cet_test(gallery_makeInput5 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput5.fcl
  ../gallery_makeInput3.d/test_gallery3.root
  ../gallery_makeInput4.d/test_gallery4.root
  DATAFILES
  fcl/test_gallery_makeInput5.fcl
  REQUIRED_FILES ../gallery_makeInput3.d/test_gallery3.root
  ../gallery_makeInput4.d/test_gallery4.root
  )

set_tests_properties(gallery_makeInput5 PROPERTIES DEPENDS "gallery_makeInput3;gallery_makeInput4")

# Use gallery_makeInput2 to make an output incompatible with other files from the same ancestor.
cet_test(gallery_makeInput6 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput6.fcl ../gallery_makeInput2.d/test_gallery2.root
  DATAFILES
  fcl/test_gallery_makeInput6.fcl
  REQUIRED_FILES ../gallery_makeInput2.d/test_gallery2.root
  TEST_PROPERTIES DEPENDS gallery_makeInput2
  )

# Uses output from gallery_makeInput6
cet_test(gallery_makeInput7 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput7.fcl ../gallery_makeInput6.d/test_gallery6.root
  DATAFILES
  fcl/test_gallery_makeInput7.fcl
  REQUIRED_FILES ../gallery_makeInput6.d/test_gallery6.root
  TEST_PROPERTIES DEPENDS gallery_makeInput6
  )

# Uses output from gallery_makeInput7
cet_test(gallery_makeInput8 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput8.fcl ../gallery_makeInput7.d/test_gallery7.root
  DATAFILES
  fcl/test_gallery_makeInput8.fcl
  REQUIRED_FILES ../gallery_makeInput7.d/test_gallery7.root
  TEST_PROPERTIES DEPENDS gallery_makeInput7
  )

# Use gallery in compiled C++ to read output from gallery_makeInput5,
# gallery_makeInput7 and gallery_makeInput8.
cet_test(gallery_t
  LIBRARIES ${gallery_tests_libraries}
  REQUIRED_FILES ../gallery_makeInput5.d/test_gallery5.root
  ../gallery_makeInput7.d/test_gallery7.root
  ../gallery_makeInput8.d/test_gallery8.root
  )

set_tests_properties(gallery_t PROPERTIES
  DEPENDS "gallery_makeInput5;gallery_makeInput7;gallery_makeInput8")

# Use gallery in interpreted C++ to read output from gallery_makeInput5
# and gallery_makeInput7.
cet_test(gallery_t.C HANDBUILT
  TEST_EXEC root
  TEST_ARGS -b -q -l gallery_t.C
  DATAFILES gallery_t.C
  REQUIRED_FILES ../gallery_makeInput5.d/test_gallery5.root
 ../gallery_makeInput7.d/test_gallery7.root
  )

set_tests_properties(gallery_t.C PROPERTIES
  DEPENDS "gallery_makeInput5;gallery_makeInput7")

# Use gallery in python to read output from gallery_makeInput5 and
# gallery_makeInput7.
if (NOT CMAKE_SYSTEM_NAME MATCHES "Darwin")
  cet_test(gallery_t.py HANDBUILT
    TEST_EXEC python
    TEST_ARGS gallery_t.py
    DATAFILES gallery_t.py
    REQUIRED_FILES ../gallery_makeInput5.d/test_gallery5.root
    ../gallery_makeInput7.d/test_gallery7.root
    )
set_tests_properties(gallery_t.py PROPERTIES
  DEPENDS "gallery_makeInput5;gallery_makeInput7")
endif()

cet_test(CriticAssns_w HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- --rethrow-all -c "CriticAssns_w.fcl"
  DATAFILES
  fcl/CriticAssns_w.fcl
)

foreach (num 1 2 3)
  cet_test(findOne_${num}_t USE_BOOST_UNIT
    LIBRARIES ${gallery_tests_libraries}
    TEST_PROPERTIES DEPENDS CriticAssns_w
    REQUIRED_FILES ../CriticAssns_w.d/out1.root
    )
endforeach()

simple_plugin(GalleryTestProducer     "module"  NO_INSTALL art_Persistency_Provenance cetlib_except)
simple_plugin(GalleryPtrTestProducer  "module"  NO_INSTALL critic_test_CriticTestObjects art_Persistency_Provenance cetlib_except)
simple_plugin(GalleryAssnsProducer    "module"  NO_INSTALL critic_test_CriticTestObjects art_Persistency_Provenance cetlib_except)
simple_plugin(CriticFailEverything    "module"  NO_INSTALL)
