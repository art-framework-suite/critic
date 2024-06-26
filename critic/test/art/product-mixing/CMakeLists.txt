cet_build_plugin(MixAnalyzer art::module NO_INSTALL USE_BOOST_UNIT)

function(build_mix_filter ART_MFT)
  configure_file(MixFilterTestImpl_module.cc.in ${ART_MFT}_module.cc)
  cet_build_plugin(${ART_MFT} art::module NO_INSTALL USE_BOOST_UNIT
    REG_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${ART_MFT}_module.cc
    LIBRARIES PRIVATE
    art::Framework_IO_ProductMix
    art_root_io::art_root_io
    range-v3::range-v3
    )
  set(DEFINITIONS ${ARGN})
  if (DEFINITIONS)
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${ART_MFT}_module.cc
      PROPERTIES COMPILE_DEFINITIONS ${DEFINITIONS})
  endif()
endfunction()

build_mix_filter(MixFilterTest)
build_mix_filter(MixFilterTestETS "ART_TEST_EVENTS_TO_SKIP_CONST=0")
build_mix_filter(MixFilterTestETSc "ART_TEST_EVENTS_TO_SKIP_CONST=1")
build_mix_filter(MixFilterTestNoStartEvent "ART_TEST_NO_STARTEVENT")

cet_build_plugin(MixProducer art::module NO_INSTALL)

# Write the data file required for the other mixing tests.
cet_test(ProductMix_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS -c "ProductMix_w.fcl"
  DATAFILES fcl/ProductMix_w.fcl)

# Make trimmed file with all events in the same run
cet_test(ProductMix_trimmed_w HANDBUILT
  TEST_EXEC art
  TEST_ARGS
    -c /dev/null
    -o mix_trimmed.root
    -s ../ProductMix_w.d/mix.root
    -n 100
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Test wrap-around feature in MT environment
cet_test(ProductMix_wraparound_r HANDBUILT
  TEST_EXEC art
  PARG_J -j 1 2 4 8
  TEST_ARGS -c ProductMix_wraparound_r.fcl
  DATAFILES fcl/ProductMix_wraparound_r.fcl
  TEST_PROPERTIES DEPENDS ProductMix_trimmed_w)

# Mix the events from ProductMix_w, analyzing the results and
# writing an output file.
cet_test(ProductMix_r1 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1.fcl"
  DATAFILES fcl/ProductMix_r1.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Mix the events from ProductMix_w, testing that things behave properly
# with primary event #2 having no secondaries to mix. No analysis is done
# nor output written.
cet_test(ProductMix_r1a HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1a.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1a.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Mix the events from ProductMix_w, testing that things behave properly
# with multiple secondary input files (actually, the same file specified
# twice).
cet_test(ProductMix_r1b HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1b.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1b.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Mix the events from ProductMix_w, testing that things behave properly
# with secondary input file wrapping and no eventsToSkip().
cet_test(ProductMix_r1c1 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1c.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1c.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w
  PASS_REGULAR_EXPRESSION "TrigReport Events total = 400 passed = 400 failed = 0")

# Mix the events from ProductMix_w, testing that things behave properly
# with secondary input file wrapping, respondToXXX() functions and
# eventsToSkip().
cet_test(ProductMix_r1c2 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1c2.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1c.fcl
  fcl/ProductMix_r1c2.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Mix the events from ProductMix_w, testing that things behave properly
# with secondary input file wrapping and eventsToSkip() const.
cet_test(ProductMix_r1c3 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1c3.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1c.fcl
  fcl/ProductMix_r1c3.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Mix the events from ProductMix_w, testing that things behave
# properly with secondary input file wrapping where the number of
# secondaries exceeds the number in the mixing file.
cet_test(ProductMix_r1c4 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1c4.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1c.fcl
  fcl/ProductMix_r1c4.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w
  PASS_REGULAR_EXPRESSION "The number of requested secondaries.*exceeds the number of events"
)

# Mix the events from ProductMix_w, testing that things behave properly
# with no startEvent function.
cet_test(ProductMix_r1d1 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1d1.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1d1.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Mix the events from ProductMix_w, testing that the secondary event
# selection is as specified. Order is:
#   SEQUENTIAL
#   RANDOM_REPLACE
#   RANDOM_LIM_REPLACE (require no dupes within a primary)
#   RANDOM_LIM_REPLACE (require dupes across a job).
#   RANDOM_NO_REPLACE
#   RANDOM_OFFSET (sequential after first event is randomly selected)
foreach(test RANGE 1 6)
  cet_test(ProductMix_r1e${test} HANDBUILT
    TEST_EXEC art_ut
    TEST_ARGS -- -c "ProductMix_r1e${test}.fcl"
    DATAFILES
    fcl/ProductMix_r1.fcl
    fcl/ProductMix_r1e.fcl
    fcl/ProductMix_r1e${test}.fcl
    TEST_PROPERTIES DEPENDS ProductMix_w)
endforeach()

# Mix the events from ProductMix_w, obtaining the filenames from a
# detail-object-provided function rather than from fileNames_.
cet_test(ProductMix_r1f1 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1f1.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1c.fcl
  fcl/ProductMix_r1f1.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Mix the events from ProductMix_w, testing that things behave properly
# with the compactMissingProducts option selected.
cet_test(ProductMix_r1g HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1g.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1g.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

set_tests_properties(
  ProductMix_r1c2
  ProductMix_r1c3
  PROPERTIES
  PASS_REGULAR_EXPRESSION "TrigReport Events total = 400 passed = 400 failed = 0")

# Mix the events from ProductMix_w, testing that we correctly detect an
# inappropriate attempt to dereference a Ptr.
cet_test(ProductMix_r1d HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r1d.fcl"
  DATAFILES
  fcl/ProductMix_r1.fcl
  fcl/ProductMix_r1d.fcl
  TEST_PROPERTIES DEPENDS ProductMix_w)

# Read the output from ProductMix_r1a running the analysis to ensure that
# everything works as read from an output file written post-mixing.
cet_test(ProductMix_r2 HANDBUILT
  TEST_EXEC art_ut
  TEST_ARGS -- -c "ProductMix_r2.fcl"
  DATAFILES fcl/ProductMix_r2.fcl
  TEST_PROPERTIES DEPENDS ProductMix_r1)
