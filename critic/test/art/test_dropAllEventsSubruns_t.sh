#!/bin/bash

. cet_test_functions.sh

export ART_EXEC=$1
verify_drop=$2
verify_drop_name=$(basename $verify_drop)

rm -f out*.root

run_art dropAllEvents_t.fcl --rethrow-all
run_art dropAllEventsSubruns_t1.fcl --rethrow-all
run_art dropAllEventsSubruns_t2.fcl --rethrow-all

check_files "out_dropAllEvents.root"
check_files "out_dropAllEventsSubruns1.root"
check_files "out_dropAllEventsSubruns2.root"

echo "Running $verify_drop_name ..."
$verify_drop || exit 1
echo "Finished $verify_drop_name ..."

run_art dropAllEvents_r_t.fcl --rethrow-all
run_art dropAllEventsSubruns_r_t1.fcl --rethrow-all
run_art dropAllEventsSubruns_r_t2.fcl --rethrow-all

check_files "out_dropAllEvents_r.root"
check_files "out_dropAllEventsSubruns1_r.root"
check_files "out_dropAllEventsSubruns2_r.root"

echo "Running $verify_drop_name r ..."
$verify_drop r || exit 1
echo "Finished $verify_drop_name r ..."
