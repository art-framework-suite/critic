art_dictionary(NO_INSTALL DICTIONARY_LIBRARIES PRIVATE art_test::TestObjects)

#================================================================
# Data container (e.g. Run, Event) shape testing
#================================================================

cet_test(DifferentHistories_ProcessA_w HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c produce.fcl --process-name ProcA -n 1 -o out.root
  DATAFILES fcl/produce.fcl)

cet_test(DifferentHistories_ProcessB_w1 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c empty.fcl --process-name EMPTY -n 1 -e 1:0:2 -o out.root
  DATAFILES fcl/empty.fcl)

cet_test(DifferentHistories_ProcessB_w2 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS --
  -c produce.fcl
  --process-name ProcB
  -o out.root
  -s ../DifferentHistories_ProcessB_w1.d/out.root
  DATAFILES fcl/produce.fcl
  REQUIRED_FILES ../DifferentHistories_ProcessB_w1.d/out.root
  TEST_PROPERTIES DEPENDS DifferentHistories_ProcessB_w1)

# Check that inconsistently produced files can be read in the same
# job, specifically de-referencing art::Ptrs in events and subruns.
cet_test(DifferentHistories_r1 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS --
  -c analyze.fcl
  -s ../DifferentHistories_ProcessA_w.d/out.root
  -s ../DifferentHistories_ProcessB_w2.d/out.root
  REQUIRED_FILES
  ../DifferentHistories_ProcessA_w.d/out.root
  ../DifferentHistories_ProcessB_w2.d/out.root
  DATAFILES fcl/analyze.fcl)

# Check that inconsistently produced files can not only be read in the
# same job (DifferentHistories_r1), but that the products therein can
# be persisted to an output file.
cet_test(DifferentHistories_r2 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS --
  -c analyze.fcl
  -s ../DifferentHistories_ProcessA_w.d/out.root
  -s ../DifferentHistories_ProcessB_w2.d/out.root
  -o out.root
  REQUIRED_FILES
  ../DifferentHistories_ProcessA_w.d/out.root
  ../DifferentHistories_ProcessB_w2.d/out.root
  DATAFILES fcl/analyze.fcl)

# Verify that the correct presence bits are set in the output of
# DifferentHistories_r2.  To wit, any event products that were
# produced with ProcA will have no products produced via ProcB, and
# vice versa.
cet_test(DifferentHistories_r3 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS --
  -c analyze_concatenated_histories.fcl
  -s ../DifferentHistories_r2.d/out.root
  DATAFILES fcl/analyze_concatenated_histories.fcl
  REQUIRED_FILES ../DifferentHistories_r2.d/out.root
  TEST_PROPERTIES DEPENDS DifferentHistories_r2)

set_tests_properties(DifferentHistories_r1 DifferentHistories_r2 PROPERTIES
  DEPENDS "DifferentHistories_ProcessA_w;DifferentHistories_ProcessB_w2")

#============================================================================
# More consistency checks, just for Event-level products, also testing
# dropping on input.
#
#  FIXME: Should be more explicit about what kinds of shapes we're
#         testing here.

cet_build_plugin(ESPrimaryProducer art::module NO_INSTALL)
cet_build_plugin(ESSecondaryProducer art::module NO_INSTALL)

# First processes: create files with two collections.
cet_test(ES_w01 HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c ES_w01.fcl -n 1 -o ES_w01.root
  DATAFILES fcl/ES_w01.fcl)

cet_test(ES_w02 HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c ES_w02.fcl -n 1 -o ES_w02.root
  DATAFILES fcl/ES_w02.fcl)

# Second process: derivative products.
cet_test(ES_ws01 HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c ES_ws01.fcl -o ES_ws01.root ../ES_w01.d/ES_w01.root
  DATAFILES fcl/ES_ws01.fcl
  TEST_PROPERTIES DEPENDS ES_w01)

cet_test(ES_ws02 HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c ES_ws02.fcl -o ES_ws02.root ../ES_w02.d/ES_w02.root
  DATAFILES fcl/ES_ws02.fcl
  TEST_PROPERTIES DEPENDS ES_w02)

# Reading processes
cet_test(ES_r01 HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c ES_r01.fcl ../ES_ws01.d/ES_ws01.root ../ES_ws02.d/ES_ws02.root
  DATAFILES fcl/ES_r01.fcl)

cet_test(ES_r02 HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c ES_r02.fcl ../ES_ws01.d/ES_ws01.root ../ES_ws02.d/ES_ws02.root
  DATAFILES fcl/ES_r02.fcl)

set_tests_properties(ES_r01 ES_r01 PROPERTIES DEPENDS "ES_ws01;ES_ws02")
