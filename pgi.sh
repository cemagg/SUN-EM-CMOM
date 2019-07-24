#!/bin/bash

cd acc_build
cmake -DPGGG=ON -DCMAKE_C_COMPILER="/opt/pgi/linux86-64/19.4/bin/pgcc" -DCMAKE_CXX_COMPILER="/opt/pgi/linux86-64/19.4/bin/pgc++" ..
make
