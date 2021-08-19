cet_build_plugin(AssnsIterProducer1 art::module NO_INSTALL)
cet_build_plugin(AssnsIterProducer2 art::module NO_INSTALL)
cet_build_plugin(ConstAssnsIterAnalyzer art::module NO_INSTALL)

cet_test(assns_const_iter HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config assns_const_iter.fcl
  DATAFILES assns_const_iter.fcl)
