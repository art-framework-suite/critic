
set(gallery_tests_libraries
  critic_test_CriticTestObjects
  gallery
  canvas
  fhiclcpp
  cetlib
  cetlib_except
  ${ROOT_RIO_LIBRARY}
  ${ROOT_Core_LIBRARY}
  )

########################################################################
#
# This test suite generates a considerable number of files, in many
# cases using one file as the basis for the generation of the next.
# The dependencies are described below, showing which input file is used
# to generate a specific output file. The notation [ test_name ] is used
# to indicate that rather than going into the creation of a file, the
# specified inputs are used to run a test named "test_name".
#
# empty.root is an art/ROOT file with no events.
# [ make_empty_file_t ] -> test_gallery_empty.root
#
# test_gallery1.root -> test_gallery2.root
#
# test_gallery2.root -> test_gallery3.root
# test_gallery2.root -> test_gallery4.root
# test_gallery2.root -> test_gallery6.root
#
# test_gallery3.root + test_gallery4.root -> test_gallery5.root
# test_gallery6.root -> test_gallery7.root
# test_gallery7.root -> test_gallery8.root
#
# test_gallery5.root + test_gallery7.root + test_gallery8.root -> [ gallery_t ]
# test_gallery5.root + test_gallery7.root -> [ gallery_t.C ]
# test_gallery5.root + test_gallery7.root -> [ gallery_t.py ]
#
# out1.root -> [ findOne_1_t ]
# out1.root -> [ findOne_2_t ]
# out1.root -> [ findOne_3_t ]
#
#------------------------------------------------------------------------
#
# Make a hierarchy of input files in order to test gallery in C++,
# interpreted C++ and python.

cet_test(gallery_makeEmptyFile HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeEmptyFile.fcl
  DATAFILES
  fcl/test_gallery_makeEmptyFile.fcl
)

cet_test(gallery_makeInput1 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput1.fcl
  DATAFILES
  fcl/test_gallery_makeInput1.fcl
  )

# Uses output from gallery_makeInput1 and gallery_makeEmptyFile
cet_test(gallery_makeInput2 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c test_gallery_makeInput2.fcl ../gallery_makeInput1.d/test_gallery1.root
  DATAFILES
  fcl/test_gallery_makeInput2.fcl
  REQUIRED_FILES ../gallery_makeInput1.d/test_gallery1.root ../gallery_makeEmptyFile.d/test_gallery_empty.root
  )
set_tests_properties(gallery_makeInput2 PROPERTIES DEPENDS "gallery_makeEmptyFile;gallery_makeInput1")

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
cet_make_exec(gallery_t
  SOURCE gallery_t.cc
  LIBRARIES ${gallery_tests_libraries})
cet_test(gallery_t HANDBUILT
  TEST_EXEC cet-bootstrap
  TEST_ARGS gallery_t
  REQUIRED_FILES ../gallery_makeInput5.d/test_gallery5.root
  ../gallery_makeInput7.d/test_gallery7.root
  ../gallery_makeInput8.d/test_gallery8.root
  )
# Enable plugins to access symbols exported by the exec (CMake policy CMP0065).
set_property(TARGET gallery_t PROPERTY ENABLE_EXPORTS TRUE)
set_property(TEST gallery_t PROPERTY
  DEPENDS "gallery_makeInput5;gallery_makeInput7;gallery_makeInput8")

# Use gallery to read a single file that is empty.
cet_make_exec(gallery_empty_file_t
  SOURCE gallery_empty_file_t.cc
  LIBRARIES ${gallery_tests_libraries})
cet_test(gallery_empty_file_t HANDBUILT
  TEST_EXEC cet-bootstrap
  TEST_ARGS gallery_empty_file_t
  REQUIRED_FILES ../gallery_makeEmptyFile.d/test_gallery_empty.root
  TEST_PROPERTIES DEPENDS gallery_makeEmptyFile
  TEST_ARGS ../gallery_makeEmptyFile.d/test_gallery_empty.root
  )

# Test gallery random access i/o.
cet_make_exec(gallery_random_io_t
  SOURCE gallery_random_io_t.cc
  LIBRARIES ${gallery_tests_libraries})
cet_test(gallery_random_io_t HANDBUILT
  TEST_EXEC cet-bootstrap
  TEST_ARGS gallery_random_io_t
  REQUIRED_FILES ../gallery_makeInput3.d/test_gallery3.root
  TEST_PROPERTIES DEPENDS gallery_makeInput3
  TEST_ARGS ../gallery_makeInput3.d/test_gallery3.root
  )

# Use gallery in interpreted C++ to read output from gallery_makeInput5
# and gallery_makeInput7.
cet_test(gallery_t.C HANDBUILT
  TEST_EXEC cet-bootstrap
  TEST_ARGS root -b -q -l gallery_t.C
  DATAFILES gallery_t.C
  REQUIRED_FILES ../gallery_makeInput5.d/test_gallery5.root
  ../gallery_makeInput7.d/test_gallery7.root
  )

set_tests_properties(gallery_t.C PROPERTIES
  DEPENDS "gallery_makeInput5;gallery_makeInput7")

# Use gallery in python to read output from gallery_makeInput5 and
# gallery_makeInput7.
cet_test(gallery_t.py HANDBUILT
  TEST_EXEC cet-bootstrap
  TEST_ARGS python gallery_t.py
  DATAFILES gallery_t.py
  REQUIRED_FILES ../gallery_makeInput5.d/test_gallery5.root
  ../gallery_makeInput7.d/test_gallery7.root
  )
set_tests_properties(gallery_t.py PROPERTIES
  DEPENDS "gallery_makeInput5;gallery_makeInput7")

cet_test(CriticAssns_w HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- --rethrow-all -c "CriticAssns_w.fcl"
  DATAFILES
  fcl/CriticAssns_w.fcl
)

foreach (num 1 2 3)
  cet_make_exec(findOne_${num}_t USE_BOOST_UNIT
    SOURCE findOne_${num}_t.cc
    LIBRARIES ${gallery_tests_libraries}
    )
  cet_test(findOne_${num}_t HANDBUILT
    TEST_EXEC cet-bootstrap
    TEST_ARGS findOne_${num}_t
    TEST_PROPERTIES DEPENDS CriticAssns_w
    REQUIRED_FILES ../CriticAssns_w.d/out1.root
    )
endforeach()

simple_plugin(GalleryTestProducer     "module"  NO_INSTALL art_Persistency_Provenance cetlib_except)
simple_plugin(GalleryPtrTestProducer  "module"  NO_INSTALL critic_test_CriticTestObjects art_Persistency_Provenance cetlib_except)
simple_plugin(GalleryAssnsProducer    "module"  NO_INSTALL critic_test_CriticTestObjects art_Persistency_Provenance cetlib_except)
simple_plugin(CriticFailEverything    "module"  NO_INSTALL)
