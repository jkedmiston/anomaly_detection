#!/bin/bash
cd src
make -f Makefile release
python process_batch.py
python process_stream.py
./flag_purchases.exe
python process_flags.py