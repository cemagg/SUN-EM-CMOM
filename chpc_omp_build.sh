#!/bin/bash

cd build
cmake -DCHPC=ON -DCOMP=ON -DCMAKE_C_COMPILER="/apps/compilers/pgi/linux86-64/19.4/bin/pgcc" -DCMAKE_CXX_COMPILER="/apps/compilers/pgi/linux86-64/19.4/bin/pgc++" ..
make