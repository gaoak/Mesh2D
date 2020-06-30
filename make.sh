#!/bin/bash
for aoa in  15
do
    c++ -std=c++11 -DOUTPUTEXP -DFOILAOA=${aoa} -DFOILM=0 -DFOILP=0 -DFOILT=12  *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g 
    ${GMSHBIN}gmsh FarField.geo  -2 -algo del2d  -clscale 20  -smooth 2 > logmesh.dat
    echo "Mesh.RecombinationAlgorithm = 0;" >> airfoil.geo
    ${GMSHBIN}gmsh airfoil.geo  -2 -algo meshadapt -clscale 20  > logmesh.dat
    ./mesh-g  merge FarField.msh airfoil.msh
    rm *plt
    $NEKBIN/FieldConvert outerRegion.xml outerRegion.plt
    $NEKBIN/FieldConvert inFoil.xml inFoil.plt
done
