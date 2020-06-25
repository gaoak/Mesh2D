#!/bin/bash
for aoa in  15
do
    c++ -std=c++11 -DOUTPUTEXP -DFOILAOA=${aoa} -DFOILM=0 -DFOILP=0 -DFOILT=12  *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g wake
    ${GMSHBIN}gmsh FarField.geo  -2 -algo del2d  -clscale 2  -smooth 2 > logmesh.dat
    ./mesh-g wake merge FarField.msh
    rm test.plt test.vtu
    $NEKBIN/FieldConvert test.xml test.vtu
    $NEKBIN/FieldConvert test.xml test.plt
    cp test.xml airfoil_${aoa}.xml
    cp test.vtu airfoil_${aoa}.vtu
done
