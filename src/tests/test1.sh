#!/bin/bash
set -e
cd ..
make -f Makefile_test1
cd tests
./test1.exe