#!/bin/bash
rm ./mesh-g
c++ -std=c++11 -DOUTPUTEXP   *.cpp CAD2D/*.cpp -c -O0 -g
c++ -std=c++11 *.o -O0 -o mesh-g -g
./mesh-g
echo "Mesh.RecombinationAlgorithm = 1;" >> Centre.geo
echo "Mesh.RecombinationAlgorithm = 1;" >> UpperFar.geo
gmsh Centre.geo  -2 -algo del2d  -clscale 0.5 -smooth 5  > logmesh.dat
gmsh UpperFar.geo  -2 -algo del2d  -clscale 10 -smooth 5  >> logmesh.dat
./mesh-g  merge
rm *plt *vtu
FieldConvert outerRegion.xml outerRegion.plt