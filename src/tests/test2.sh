#!/bin/bash
set -e
cd ..
make -f Makefile_test2
cd tests
./test2.exe