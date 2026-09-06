#!/bin/bash
set -e

c++ -std=c++11 -DOUTPUTEXP *.cpp CAD2D/*.cpp -c -Og -g
c++ -std=c++11 *.o -Og -o mesh-g -g
./mesh-g

if command -v FieldConvert >/dev/null 2>&1; then
    if ! FieldConvert hartmann.xml hartmann.plt; then
        echo "warning: FieldConvert failed; hartmann.xml was generated successfully" >&2
    fi
fi
