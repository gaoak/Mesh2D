#!/bin/bash
for aoa in  15
do
    c++ -std=c++11 -DOUTPUTEXP -DFOILAOA=${aoa} -DFOILM=0 -DFOILP=0 -DFOILT=12  *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g wake
    echo "gmsh FarField.geo"
    echo "Mesh.RecombinationAlgorithm = 1;" >> FarField.geo
    ${GMSHBIN}gmsh FarField.geo  -2 -algo del2d  -clscale 80   > logmesh.dat
    echo "Mesh.RecombinationAlgorithm = 0;" >> back_airfoil.geo
    echo "gmsh back_airfoil.geo"
    ${GMSHBIN}gmsh back_airfoil.geo  -2 -algo meshadapt -clscale 10  >> logmesh.dat
    echo "gmsh front_airfoil.geo"
    ${GMSHBIN}gmsh front_airfoil.geo  -2 -algo front2d -clscale 10  >> logmesh.dat
    ./mesh-g wake merge FarField.msh front_airfoil.msh back_airfoil.msh
    rm *plt *vtu
    $NEKBIN/FieldConvert outerRegion.xml outerRegion.plt
    $NEKBIN/FieldConvert inFoil.xml inFoil.plt
done
