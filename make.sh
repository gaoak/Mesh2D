#!/bin/bash
    c++ -std=c++11 -DOUTPUTEXP  *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g
    echo "gmsh FarField.geo"
    echo "Mesh.RecombinationAlgorithm = 1;" >> FarField.geo
    gmsh FarField.geo  -2 -algo del2d  -clscale 10 -smooth 5  > logmesh.dat
    ./mesh-g merge FarField.msh
    FieldConvert outerRegion.xml outerRegion.plt -f