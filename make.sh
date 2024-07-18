#!/bin/bash
if test -d build; then
  echo "running"
  cd build
else
  echo "running"
  mkdir build
  cd build
  cmake ..
fi
make
./meshOut
echo "gmsh FarField0.geo"
echo "Mesh.RecombinationAlgorithm = 1;" >> FarField0.geo
gmsh FarField0.geo  -2 -algo del2d  -clscale 10 -smooth 5  > logmesh.dat
./meshOut merge FarField0.msh
FieldConvert outerRegion.xml outerRegion.plt -f