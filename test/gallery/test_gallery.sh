#!/bin/bash

# Tests the interoperability of gallery and the
# full art framework by using the full art framework
# to generate some output files and then using
# gallery to read them.

function die { echo Failure $1: status $2 ; exit $2 ; }

. cet_test_functions.sh

# Add some products containing known values and
# write an output file.
run_art test_gallery_makeInput1.fcl --rethrow-all
check_files test_gallery1.root

# Read file produced in previous process and add
# some products with known values.
run_art test_gallery_makeInput2.fcl --rethrow-all
check_files test_gallery2.root

# Read file produced in previous process and add
# some products with known values.
run_art test_gallery_makeInput3.fcl --rethrow-all
check_files test_gallery3.root

# Same as 3 (reads the output file of the second process)
# except parameters have different values for module m7
# to force ProcessHistoryID to be different.
run_art test_gallery_makeInput4.fcl --rethrow-all
check_files test_gallery4.root

# Merge the files produced by 3, and 4.
run_art test_gallery_makeInput5.fcl --rethrow-all
check_files test_gallery5.root

# Now produce a separate file with file 2 as input
# but with differences so that art cannot merge it,
# but gallery will be able to read it in the same
# process. Add an extra process with products.
# Also remove a product in the last step.
# This will yield differing ProductRegistries,
# BranchIDLists, and ProductHistories.

run_art test_gallery_makeInput6.fcl --rethrow-all
check_files test_gallery6.root

run_art test_gallery_makeInput7.fcl --rethrow-all
check_files test_gallery7.root

# Make a file with no events to test that the event
# iterating code can handle empty files

run_art test_gallery_makeInput8.fcl --rethrow-all
check_files test_gallery8.root

# Read the file using the new gallery code
# Test by getting values from products and
# checking that they contain the expected values.
# Plus some other tests of the gallery code
# mixed in.
echo Invoking gallery_t
time gallery_t || die "gallery_t " $?

# Test using ROOT script
echo Invoking root -b -q -l gallery_t.C
time root -b -q -l gallery_t.C || die "gallery_t.C " $?

# Test using pyROOT
echo Invoking python gallery_t.py
time python gallery_t.py || die "gallery_t.py " $?