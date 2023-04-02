#!/bin/bash
    c++ -std=c++11 -DOUTPUTEXP   *.cpp CAD2D/*.cpp -c -Og -g
    c++ -std=c++11 *.o -Og -o mesh-g -g
    ./mesh-g
#    echo "gmsh FarField.geo"
#    echo "Mesh.RecombinationAlgorithm = 0;" >> FarField.geo
    ${GMSHBIN}/gmsh FarField.geo  -2  -smooth 5  > logmesh.dat
#    ${GMSHBIN}/gmsh data/domain.geo  -2 -smooth 5 >> logmesh.dat
    ./mesh-g merge FarField.msh  data/domain.msh
    rm *plt *vtu
    $NEKBIN/FieldConvert outerRegion.xml outerRegion.plt -f
