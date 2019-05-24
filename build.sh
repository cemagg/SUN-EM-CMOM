#!/bin/bash

cd build/
cmake ..
make
time ./mom ../examples/dgfm_big/dgfm_big.mom --dgfm
