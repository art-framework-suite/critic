cet_build_plugin(PtrVectorAnalyzer art::module NO_INSTALL)
cet_build_plugin(PtrVectorProducer art::module NO_INSTALL)

cet_test(PtrVector_01a_t HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config ptrvector_01a.fcl
  DATAFILES fcl/ptrvector_01a.fcl)

cet_test(PtrVector_01b_t HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config ptrvector_01b.fcl
  DATAFILES fcl/ptrvector_01b.fcl
  TEST_PROPERTIES DEPENDS PtrVector_01a_t)

cet_test(PtrVector_01c_t HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config ptrvector_01c.fcl
  DATAFILES fcl/ptrvector_01c.fcl)

cet_test(PtrVector_01d_t HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config ptrvector_01d.fcl
  DATAFILES fcl/ptrvector_01d.fcl
  TEST_PROPERTIES DEPENDS PtrVector_01c_t)

cet_test(PtrVector_01e_t HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config ptrvector_01e.fcl
  DATAFILES fcl/ptrvector_01e.fcl
  TEST_PROPERTIES DEPENDS PtrVector_01d_t)
