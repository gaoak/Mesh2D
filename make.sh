#!/bin/bash
    c++ -std=c++11 -DOUTPUTEXP  *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g
    if [ "0" = "$?" ]; then
        rm square.plt
        FieldConvert square.xml square.plt
    fi
