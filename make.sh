#!/bin/bash
rm ./mesh-g
c++ -std=c++11 -DOUTPUTEXP   *.cpp CAD2D/*.cpp -c -O0 -g
c++ -std=c++11 *.o -O0 -o mesh-g -g
./mesh-g
echo "gmsh Centre.geo"
echo "Mesh.RecombinationAlgorithm = 1;" >> Centre.geo
gmsh Centre.geo  -2 -algo del2d  -clscale 10 -smooth 5  > logmesh.dat
./mesh-g  merge
#    rm *plt *vtu
#    FieldConvert outerRegion.xml outerRegion.plt
#    FieldConvert outerRegion_Otip.xml outerRegion_Otip.plt
#    FieldConvert inFoil.xml inFoil.plt
