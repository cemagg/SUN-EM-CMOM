#!/bin/bash

cd build/
cmake ..
make
./mom ../examples/strip_dipole_array/strip_dipole_array.mom --dgfm
