#!/bin/bash

cd cbfm_build/
cmake ..
make
./mom ../examples/pec_plate.mom --cbfm