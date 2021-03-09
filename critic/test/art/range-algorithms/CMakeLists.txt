build_plugin(AssnsProducer1   "module" NO_INSTALL)
build_plugin(AssnsProducer2   "module" NO_INSTALL)
build_plugin(AssnsAnalyzerFEG "module" NO_INSTALL LIBRARIES
  PRIVATE canvas::AssnsAlgorithms)

cet_test(for_each_group_1 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config for_each_group_1.fcl
  DATAFILES for_each_group_1.fcl)

cet_test(for_each_group_2 HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config for_each_group_2.fcl
  DATAFILES for_each_group_2.fcl
  TEST_PROPERTIES DEPENDS for_each_group_1)

art_dictionary(NO_INSTALL)
