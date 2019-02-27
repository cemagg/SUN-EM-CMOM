#!/bin/bash

cd build/
cmake ..
make
./mom ../examples/two_plate_array/two_plate_array_coarse.mom --cbfm