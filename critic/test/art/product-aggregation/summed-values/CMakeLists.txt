cet_build_plugin(SummedValueAcrossRuns art::module NO_INSTALL)

cet_test(summed_value_across_runs_t HANDBUILT
  TEST_EXEC art
  TEST_ARGS --config summed_value_across_runs.fcl
  DATAFILES fcl/summed_value_across_runs.fcl)
