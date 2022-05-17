#!/bin/bash
    c++ -std=c++11 -DOUTPUTEXP   *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g wake
    echo "gmsh FarField.geo"
    echo "Mesh.RecombinationAlgorithm = 1;" >> FarField.geo
    ${GMSHBIN}/gmsh FarField.geo  -2 -algo front2d  -clscale 10 -smooth 5  > logmesh.dat
    ./mesh-g wake merge FarField.msh
    rm *plt *vtu
    $NEKBIN/FieldConvert outerRegion.xml outerRegion.plt
