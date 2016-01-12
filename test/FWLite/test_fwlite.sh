#!/bin/bash

# Tests the interoperability of fwlite and the
# full art framework by using the full art framework
# to generate some output files and then using
# fwlite to read them.

function die { echo Failure $1: status $2 ; exit $2 ; }

. cet_test_functions.sh

# Add some products containing known values and
# write an output file.
run_art test_fwlite_makeInput1.fcl --rethrow-all
check_files test_fwlite1.root

# Read file produced in previous process and add
# some products with known values.
run_art test_fwlite_makeInput2.fcl --rethrow-all
check_files test_fwlite2.root

# Read file produced in previous process and add
# some products with known values.
run_art test_fwlite_makeInput3.fcl --rethrow-all
check_files test_fwlite3.root

# Same as 3 (reads the output file of the second process)
# except parameters have different values for module m7
# to force ProcessHistoryID to be different.
run_art test_fwlite_makeInput4.fcl --rethrow-all
check_files test_fwlite4.root

# Merge the files produced by 3, and 4.
run_art test_fwlite_makeInput5.fcl --rethrow-all
check_files test_fwlite5.root

# Now produce a separate file with file 2 as input
# but with differences so that art cannot merge it,
# but fwlite will be able to read it in the same
# process. Add an extra process with products.
# Also remove a product in the last step.
# This will yield differing ProductRegistries,
# BranchIDLists, and ProductHistories.

run_art test_fwlite_makeInput6.fcl --rethrow-all
check_files test_fwlite6.root

run_art test_fwlite_makeInput7.fcl --rethrow-all
check_files test_fwlite7.root

# Make a file with no events to test that the event
# iterating code can handle empty files

run_art test_fwlite_makeInput8.fcl --rethrow-all
check_files test_fwlite8.root

# Read the file using the new fwlite code
# Test by getting values from products and
# checking that they contain the expected values.
# Plus some other tests of the fwlite code
# mixed in.
echo Invoking fwlite_t
time fwlite_t || die "fwlite_t " $?

# Test using ROOT script
echo Invoking root -b -q -l fwlite_t.C
time root -b -q -l fwlite_t.C || die "fwlite_t.C " $?

# Test using ROOT script compiled with ACLiC
# Currently this does not work, but this is
# not important. We do not need it.
#export ROOT_INCLUDE_PATH=${ART_INC}:${CETLIB_INC}:${FHICLCPP_INC}
#echo Invoking root -b -q -l fwlite_t_aclic.C++
#time root -b -q -l fwlite_t_aclic.C++ || die "fwlite_t.C++ " $?

# Test using pyROOT
echo Invoking python fwlite_t.py
time python fwlite_t.py || die "fwlite_t.py " $?
