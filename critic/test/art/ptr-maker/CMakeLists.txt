cet_build_plugin(PtrMakerProducer art::module NO_INSTALL)
cet_build_plugin(PtrMakerProducer2 art::module NO_INSTALL)
cet_build_plugin(PtrMakerAnalyzer art::module NO_INSTALL)

cet_test(PtrMaker_t1 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config PtrMaker_t1.fcl
  DATAFILES PtrMaker_t1.fcl)

cet_test(PtrMaker_t2 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config PtrMaker_t2.fcl
  DATAFILES PtrMaker_t2.fcl)

cet_test(PtrMaker_t3 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config PtrMaker_t3.fcl
  DATAFILES PtrMaker_t3.fcl
  TEST_PROPERTIES DEPENDS PtrMaker_t1)

cet_test(PtrMaker_t4 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config PtrMaker_t4.fcl
  DATAFILES PtrMaker_t4.fcl
  TEST_PROPERTIES DEPENDS PtrMaker_t2)
