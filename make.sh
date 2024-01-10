#!/bin/bash
    c++ -std=c++11 -DOUTPUTEXP  *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g
    echo "gmsh FarField0.geo"
    echo "Mesh.RecombinationAlgorithm = 1;" >> FarField0.geo
    gmsh FarField0.geo  -2 -algo del2d  -clscale 10 -smooth 5  > logmesh.dat
    echo "gmsh FarField1.geo"
    echo "Mesh.RecombinationAlgorithm = 1;" >> FarField1.geo
    gmsh FarField1.geo  -2 -algo del2d  -clscale 10 -smooth 5  > logmesh.dat
    ./mesh-g merge FarField0.msh FarField1.msh
    FieldConvert outerRegion.xml outerRegion.plt -f