#!/bin/bash
for aoa in  15
do
    c++ -std=c++11 *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g
done
