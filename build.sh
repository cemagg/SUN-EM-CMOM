#!/bin/bash

cd build/
cmake ..
make
./mom ../examples/example-12/pec_plate.mom
