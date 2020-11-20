#!/bin/bash
    c++ -std=c++11 -DOUTPUTEXP   *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g wake
    echo "gmsh FarField.geo"
    echo "Mesh.RecombinationAlgorithm = 1;" >> FarField.geo
    ${GMSHBIN}gmsh FarField.geo  -2 -algo del2d  -clscale 10 -smooth 5  > logmesh.dat
    echo "Mesh.RecombinationAlgorithm = 0;" >> airfoil.geo
    echo "gmsh airfoil.geo"
    ${GMSHBIN}gmsh airfoil.geo  -2 -algo meshadapt -clscale 10  >> logmesh.dat
    ./mesh-g wake merge FarField.msh airfoil.msh
    rm *plt *vtu
    $NEKBIN/FieldConvert outerRegion.xml outerRegion.plt
    $NEKBIN/FieldConvert outerRegion_Otip.xml outerRegion_Otip.plt
    $NEKBIN/FieldConvert inFoil.xml inFoil.plt
