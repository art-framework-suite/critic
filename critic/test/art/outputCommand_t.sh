#!/bin/bash

. cet_test_functions.sh

export ART_EXEC=$1

rm -f out.root cerr.log warnings.log

run_art outputCommand_w.fcl --rethrow-all

check_files "out.root"

run_art outputCommand_r.fcl --rethrow-all
