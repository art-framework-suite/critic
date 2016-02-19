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

simple_plugin(GalleryTestProducer     "module"  NO_INSTALL art_Persistency_Provenance)
simple_plugin(GalleryPtrTestProducer  "module"  NO_INSTALL art_Persistency_Provenance test_CriticTestObjects)
simple_plugin(CriticFailEverything    "module"  NO_INSTALL)
