#!/bin/bash

cd build/
cmake ..
make
./mom ../examples/pec_plate.mom