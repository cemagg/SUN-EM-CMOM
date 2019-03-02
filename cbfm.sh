#!/bin/bash

cd cbfm_build/
cmake ..
make
./mom ../examples/simple_plate_array/simple_plate_array.mom --cbfm