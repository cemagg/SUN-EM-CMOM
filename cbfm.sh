#!/bin/bash

cd cbfm_build/
cmake ..
make
./mom ../examples/two_plate_array/two_plate_array.mom --cbfm