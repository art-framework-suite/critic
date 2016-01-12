set(fwlite_tests
  fwlite_t
)

set( fwlite_tests_libraries
        canvas_FWLite
        canvas_Persistency_Provenance
        canvas_Utilities
        ${ROOT_RIO}
)

foreach(fwlite_test ${fwlite_tests})
  cet_test(${fwlite_test} NO_AUTO
    LIBRARIES ${fwlite_tests_libraries}
  )
endforeach()

cet_test(test_fwlite.sh PREBUILT
  DATAFILES
  fcl/test_fwlite_makeInput1.fcl
  fcl/test_fwlite_makeInput2.fcl
  fcl/test_fwlite_makeInput3.fcl
  fcl/test_fwlite_makeInput4.fcl
  fcl/test_fwlite_makeInput5.fcl
  fcl/test_fwlite_makeInput6.fcl
  fcl/test_fwlite_makeInput7.fcl
  fcl/test_fwlite_makeInput8.fcl
  fcl/messageDefaults.fcl
  fwlite_t.C
  fwlite_t_aclic.C
  fwlite_t.py
)

simple_plugin(FWLiteTestProducer                 "module"  NO_INSTALL )
simple_plugin(FWLitePtrTestProducer                 "module"  NO_INSTALL )
