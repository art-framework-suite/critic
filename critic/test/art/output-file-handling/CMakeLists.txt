# Produce outputs
cet_test(SingleOutputProducerEmptyEvent_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c singleOutputProducerEmptyEvent_w.fcl
  DATAFILES fcl/singleOutputProducerEmptyEvent_w.fcl)

cet_test(MultiOutputProducerEmptyEvent_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c multiOutputProducerEmptyEvent_w.fcl
  DATAFILES fcl/multiOutputProducerEmptyEvent_w.fcl)

cet_test(MultiOutputProducerRootInput_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c multiOutputProducerRootInput_w.fcl -s ../SingleOutputProducerEmptyEvent_w.d/out.root
  DATAFILES fcl/multiOutputProducerRootInput_w.fcl
  TEST_PROPERTIES DEPENDS SingleOutputProducerEmptyEvent_w)

# The following tests exercise the state machine in how it handles
# output-file switches, based on the outputs produced above.  The
# expected output for Event-, SubRun- and Run-level switches should be
# the same for both the 'EmptyEvent' and 'RootInput' sources.
#
# A test for output-file switching upon 'InputFile' boundaries is
# relevant only for RootInput and is presented after these.

foreach(source IN ITEMS EmptyEvent RootInput)

  ## =============================================================
  ## Event-level output-file rollover
  SET(OutputFilePath "${CMAKE_CURRENT_BINARY_DIR}/MultiOutputProducer${source}_w.d/")

  cet_test(MultiOutput${source}_FileIndexCheck_Event_t HANDBUILT
    TEST_EXEC file_info_dumper
    TEST_ARGS
    --range-of-validity
    --file-index
    ${OutputFilePath}/out_r1_sr0_e1.root
    ${OutputFilePath}/out_r1_sr0_e2.root
    ${OutputFilePath}/out_r1_sr0_e3.root
    ${OutputFilePath}/out_r1_sr0_e4.root
    ${OutputFilePath}/out_r1_sr0_e5.root
    ${OutputFilePath}/out_r1_sr1_e1.root
    ${OutputFilePath}/out_r1_sr1_e2.root
    ${OutputFilePath}/out_r1_sr1_e3.root
    ${OutputFilePath}/out_r1_sr1_e4.root
    ${OutputFilePath}/out_r1_sr1_e5.root
    ${OutputFilePath}/out_r2_sr0_e1.root
    ${OutputFilePath}/out_r2_sr0_e2.root
    ${OutputFilePath}/out_r2_sr0_e3.root
    ${OutputFilePath}/out_r2_sr0_e4.root
    REF "${CMAKE_CURRENT_SOURCE_DIR}/MultiOutput_FileIndexCheck_Event_t-ref.txt"
    TEST_PROPERTIES DEPENDS MultiOutputProducer${source}_w
    )

  ## =============================================================
  ## SubRun-level output-file rollover

  cet_test(MultiOutput${source}_FileIndexCheck_SubRun_t HANDBUILT
    TEST_EXEC file_info_dumper
    TEST_ARGS
    --range-of-validity
    --file-index
    ${OutputFilePath}/out_r1_sr0_1.root
    ${OutputFilePath}/out_r1_sr1_1.root
    ${OutputFilePath}/out_r2_sr0_1.root
    REF "${CMAKE_CURRENT_SOURCE_DIR}/MultiOutput_FileIndexCheck_SubRun_t-ref.txt"
    TEST_PROPERTIES DEPENDS MultiOutputProducer${source}_w
    )

  ## =============================================================
  ## Run-level output-file rollover

  cet_test(MultiOutput${source}_FileIndexCheck_Run_t HANDBUILT
    TEST_EXEC file_info_dumper
    TEST_ARGS
    --range-of-validity
    --file-index
    ${OutputFilePath}/out_r1_1.root
    ${OutputFilePath}/out_r2_1.root
    REF "${CMAKE_CURRENT_SOURCE_DIR}/MultiOutput_FileIndexCheck_Run_t-ref.txt"
    TEST_PROPERTIES DEPENDS MultiOutputProducer${source}_w
    )

  ## =============================================================
  ## No rollover at all

  cet_test(MultiOutput${source}_FileIndexCheck_t HANDBUILT
    TEST_EXEC file_info_dumper
    TEST_ARGS
    --range-of-validity
    --file-index
    ${OutputFilePath}/out.root
    REF "${CMAKE_CURRENT_SOURCE_DIR}/MultiOutput_FileIndexCheck_t-ref.txt"
    TEST_PROPERTIES DEPENDS MultiOutputProducer${source}_w
    )

endforeach()

## =============================================================
## Output switch on InputFile boundary

SET(InputFilePath "${CMAKE_CURRENT_BINARY_DIR}/MultiOutputProducerEmptyEvent_w.d")

cet_test(MultiInput_SwitchOutputOnNewInput_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS
  -c multiInput_SwitchOutputOnNewInput_w.fcl
  -s ${InputFilePath}/out_r1_1.root
  -s ${InputFilePath}/out_r2_1.root
  DATAFILES fcl/multiInput_SwitchOutputOnNewInput_w.fcl
  TEST_PROPERTIES DEPENDS MultiOutputProducerEmptyEvent_w)

SET(InputFilePath "${CMAKE_CURRENT_BINARY_DIR}/MultiInput_SwitchOutputOnNewInput_w.d")

cet_test(MultiOutput_FileIndexCheck_InputFile_t HANDBUILT
  TEST_EXEC file_info_dumper
  TEST_ARGS
  --range-of-validity
  --file-index
  ${InputFilePath}/out_1.root
  ${InputFilePath}/out_2.root
  REF "${CMAKE_CURRENT_SOURCE_DIR}/MultiOutput_FileIndexCheck_InputFile_t-ref.txt"
  TEST_PROPERTIES DEPENDS MultiInput_SwitchOutputOnNewInput_w)

## =============================================================
## Output switch on InputFile boundary - 2 inputs per output

SET(InputFilePath "${CMAKE_CURRENT_BINARY_DIR}/MultiOutputProducerEmptyEvent_w.d")

cet_test(MultiInput_SwitchOutputOnNewInput_w2 HANDBUILT
  TEST_EXEC art
  TEST_ARGS
  -c multiInput_SwitchOutputOnNewInput_w2.fcl
  -s ${InputFilePath}/out_r1_sr0_e1.root
  -s ${InputFilePath}/out_r1_sr0_e2.root
  -s ${InputFilePath}/out_r1_sr0_e3.root
  -s ${InputFilePath}/out_r1_sr0_e4.root
  -s ${InputFilePath}/out_r1_sr0_e5.root
  -s ${InputFilePath}/out_r1_sr1_e1.root
  -s ${InputFilePath}/out_r1_sr1_e2.root
  -s ${InputFilePath}/out_r1_sr1_e3.root
  -s ${InputFilePath}/out_r1_sr1_e4.root
  -s ${InputFilePath}/out_r1_sr1_e5.root
  -s ${InputFilePath}/out_r2_sr0_e1.root
  -s ${InputFilePath}/out_r2_sr0_e2.root
  -s ${InputFilePath}/out_r2_sr0_e3.root
  -s ${InputFilePath}/out_r2_sr0_e4.root
  DATAFILES fcl/multiInput_SwitchOutputOnNewInput_w2.fcl
  TEST_PROPERTIES DEPENDS MultiOutputProducerEmptyEvent_w)

SET(InputFilePath "${CMAKE_CURRENT_BINARY_DIR}/MultiInput_SwitchOutputOnNewInput_w2.d")

cet_test(MultiOutput_FileIndexCheck_InputFile_t2 HANDBUILT
  TEST_EXEC file_info_dumper
  TEST_ARGS
  --range-of-validity
  --file-index
  ${InputFilePath}/out_1.root
  ${InputFilePath}/out_2.root
  ${InputFilePath}/out_3.root
  ${InputFilePath}/out_4.root
  ${InputFilePath}/out_5.root
  ${InputFilePath}/out_6.root
  ${InputFilePath}/out_7.root
  REF "${CMAKE_CURRENT_SOURCE_DIR}/MultiOutput_FileIndexCheck_InputFile_t2-ref.txt"
  TEST_PROPERTIES DEPENDS MultiInput_SwitchOutputOnNewInput_w2)

## =============================================================
## Check that the FileIndex looks reasonable for an output from a job
## that concatenated all event-level files looks reasonable.

SET(InputFilePath "${CMAKE_CURRENT_BINARY_DIR}/MultiOutputProducerRootInput_w.d")

cet_test(MultiInputConcatenate_t HANDBUILT
  TEST_EXEC art
  TEST_ARGS
  -c empty.fcl
  -o out.root
  -s ${InputFilePath}/out_r1_sr0_e1.root
  -s ${InputFilePath}/out_r1_sr0_e2.root
  -s ${InputFilePath}/out_r1_sr0_e3.root
  -s ${InputFilePath}/out_r1_sr0_e4.root
  -s ${InputFilePath}/out_r1_sr0_e5.root
  -s ${InputFilePath}/out_r1_sr1_e1.root
  -s ${InputFilePath}/out_r1_sr1_e2.root
  -s ${InputFilePath}/out_r1_sr1_e3.root
  -s ${InputFilePath}/out_r1_sr1_e4.root
  -s ${InputFilePath}/out_r1_sr1_e5.root
  -s ${InputFilePath}/out_r2_sr0_e1.root
  -s ${InputFilePath}/out_r2_sr0_e2.root
  -s ${InputFilePath}/out_r2_sr0_e3.root
  -s ${InputFilePath}/out_r2_sr0_e4.root
  DATAFILES fcl/empty.fcl
  TEST_PROPERTIES DEPENDS MultiOutputProducerRootInput_w)

cet_test(MultiInputConcatenate_FileIndexCheck_t HANDBUILT
  TEST_EXEC file_info_dumper
  TEST_ARGS --range-of-validity --file-index ../MultiInputConcatenate_t.d/out.root
  REF "${CMAKE_CURRENT_SOURCE_DIR}/MultiInputConcatenate_FileIndexCheck_t-ref.txt"
  TEST_PROPERTIES DEPENDS MultiInputConcatenate_t)

## =============================================================
## Test that output files are correctly split whenever a maximum
## number of events is specified, but the granularity is 'SubRun'.

cet_test(DelaySwitchToSubRunBoundary_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c delaySwitchToSubRunBoundary_w.fcl
  DATAFILES fcl/delaySwitchToSubRunBoundary_w.fcl)

cet_test(DelaySwitchToSubRunBoundary_FileIndexCheck_t HANDBUILT
  TEST_EXEC file_info_dumper
  TEST_ARGS
  --range-of-validity
  --file-index
  "../DelaySwitchToSubRunBoundary_w.d/out_1.root"
  "../DelaySwitchToSubRunBoundary_w.d/out_2.root"
  REF "${CMAKE_CURRENT_SOURCE_DIR}/DelaySwitchToSubRunBoundary_FileIndexCheck_t-ref.txt"
  TEST_PROPERTIES DEPENDS DelaySwitchToSubRunBoundary_w)

## =============================================================
## Test that output files are correctly split whenever two
## file-switching conditions are specified.

cet_test(SwitchOnTwoConditions_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c switchOnTwoConditions_w.fcl
  DATAFILES fcl/switchOnTwoConditions_w.fcl)

cet_test(SwitchOnTwoConditions_FileIndexCheck_t HANDBUILT
  TEST_EXEC file_info_dumper
  TEST_ARGS
  --range-of-validity
  --file-index
  "../SwitchOnTwoConditions_w.d/out_r1_1.root"
  "../SwitchOnTwoConditions_w.d/out_r1_2.root"
  "../SwitchOnTwoConditions_w.d/out_r2_1.root"
  "../SwitchOnTwoConditions_w.d/out_r2_2.root"
  REF "${CMAKE_CURRENT_SOURCE_DIR}/SwitchOnTwoConditions-ref.txt"
  TEST_PROPERTIES DEPENDS SwitchOnTwoConditions_w)

## =============================================================
## Test output-file switching based on file size.

cet_build_plugin(BigProductProducer art::module NO_INSTALL)

cet_test(SwitchOnFileSizeMax_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c switchOnFileSizeMax_w.fcl
  DATAFILES fcl/switchOnFileSizeMax_w.fcl)

cet_test(SwitchOnFileSizeMax_r
  SOURCE switch_on_file_size_max_r.cxx
  LIBRARIES PRIVATE ROOT::RIO ROOT::Tree
  TEST_ARGS
  "../SwitchOnFileSizeMax_w.d/out_1.root"
  "../SwitchOnFileSizeMax_w.d/out_2.root"
  "../SwitchOnFileSizeMax_w.d/out_3.root"
  TEST_PROPERTIES DEPENDS SwitchOnFileSizeMax_w)
