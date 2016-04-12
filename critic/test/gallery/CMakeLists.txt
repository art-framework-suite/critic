set(gallery_tests
  gallery_t
)

set( gallery_tests_libraries
        gallery
        canvas_Persistency_Common
        canvas_Persistency_Provenance
        canvas_Utilities
        ${ROOT_RIO}
)

foreach(gallery_test ${gallery_tests})
  cet_test(${gallery_test} NO_AUTO
    LIBRARIES ${gallery_tests_libraries}
  )
endforeach()

cet_test(test_gallery.sh PREBUILT
  DATAFILES
  fcl/test_gallery_makeInput1.fcl
  fcl/test_gallery_makeInput2.fcl
  fcl/test_gallery_makeInput3.fcl
  fcl/test_gallery_makeInput4.fcl
  fcl/test_gallery_makeInput5.fcl
  fcl/test_gallery_makeInput6.fcl
  fcl/test_gallery_makeInput7.fcl
  fcl/test_gallery_makeInput8.fcl
  fcl/messageDefaults.fcl
  gallery_t.C
  gallery_t.py
)

cet_test(CriticAssns_w HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- --rethrow-all -c "CriticAssns_w.fcl"
  DATAFILES
  fcl/CriticAssns_w.fcl
)

cet_test(findOne_1_t USE_BOOST_UNIT
  LIBRARIES ${gallery_tests_libraries}
  TEST_PROPERTIES DEPENDS CriticAssns_w
  REQUIRED_FILES ../CriticAssns_w.d/out1.root
)

cet_test(findOne_2_t USE_BOOST_UNIT
  LIBRARIES ${gallery_tests_libraries}
  TEST_PROPERTIES DEPENDS CriticAssns_w
  REQUIRED_FILES ../CriticAssns_w.d/out1.root
)

cet_test(findOne_3_t USE_BOOST_UNIT
  LIBRARIES ${gallery_tests_libraries}
  TEST_PROPERTIES DEPENDS CriticAssns_w
  REQUIRED_FILES ../CriticAssns_w.d/out1.root
)

simple_plugin(GalleryTestProducer     "module"  NO_INSTALL art_Persistency_Provenance)
simple_plugin(GalleryPtrTestProducer  "module"  NO_INSTALL critic_test_CriticTestObjects art_Persistency_Provenance)
simple_plugin(GalleryAssnsProducer    "module"  NO_INSTALL critic_test_CriticTestObjects art_Persistency_Provenance)
simple_plugin(CriticFailEverything    "module"  NO_INSTALL)
