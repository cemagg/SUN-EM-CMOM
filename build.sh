#!/bin/bash

cd build/
cmake ..
make
./mom ../examples/dgfm_small/dgfm_small.mom --dgfm
