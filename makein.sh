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
./meshIn
echo "gmsh FarField0.geo"
echo "Mesh.RecombinationAlgorithm = 1;" >> FarField0.geo
gmsh FarField0.geo  -2 -algo meshadapt  -clscale 10  > logmesh.dat
./meshIn merge FarField0.msh
FieldConvert inFoilRegion.xml inFoilRegion.plt -f