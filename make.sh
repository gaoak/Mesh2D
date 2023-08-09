#!/bin/bash
rm ./mesh-g *.msh *.geo *.xml *.dat *.plt 
c++ -std=c++11 -DOUTPUTEXP   *.cpp CAD2D/*.cpp -c -O0 -g
c++ -std=c++11 *.o -O0 -o mesh-g -g
./mesh-g
echo "Mesh.RecombinationAlgorithm = 1;" >> BottomGap.geo
gmsh BottomGap.geo  -2 -algo front2d  -clscale 5 -smooth 5  > logmesh.dat
./mesh-g  merge
rm *plt *vtu
FieldConvert outerRegion.xml outerRegion.plt