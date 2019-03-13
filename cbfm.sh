#!/bin/bash

cd cbfm_build/
cmake ..
make
./mom ../examples/cbfm_debug/cbfm_debug.mom --cbfm
