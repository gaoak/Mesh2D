#!/bin/bash
    rm *.geo *.msh *.dat *.xml
    c++ -std=c++11 -DOUTPUTEXP   *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g wake
    echo "gmsh FarField.geo"
    echo "Mesh.RecombinationAlgorithm = 1;" >> FarField1.geo
    echo "Mesh.RecombinationAlgorithm = 1;" >> FarField2.geo
    gmsh FarField1.geo  -2 -algo del2d  -clscale 10 -smooth 5  > logmesh.dat
    gmsh FarField2.geo  -2 -algo del2d  -clscale 10 -smooth 5  >> logmesh.dat
    ./mesh-g wake merge FarField1.msh FarField2.msh
    rm *plt *vtu
    FieldConvert outerRegion.xml outerRegion.plt
