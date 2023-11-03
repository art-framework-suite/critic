#!/bin/bash

art_exec=$1

# Process 1
$art_exec --rethrow-all -c runSubRunNoPut_t.fcl -o out1.root -n 1

# Process 2
# N.B. This process will produce an output file that cannot be read
#      because there will be two instances of the same event.  That is
#      okay for the purposes of this test.
touch empty.fcl
$art_exec --rethrow-all -c empty.fcl -o out.root -s out1.root -s out1.root -n 2 || exit 1

rm out1.root
