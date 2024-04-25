#!/bin/bash

. cet_test_functions.sh

export ART_EXEC=$1

rm -f out.root cerr.log warnings.log

run_art test_compressed_simple_01.fcl --rethrow-all

check_files "out.root"

run_art test_compressed_simple_01r.fcl --rethrow-all
