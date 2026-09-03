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
./meshOut farbox
echo "gmsh FarField0.geo"
echo "Mesh.RecombinationAlgorithm = 1;" >> FarField0.geo
gmsh FarField0.geo -2 -algo del2d -clscale 10 -smooth 5 -format msh2 \
  -o FarField0.msh > logmesh.dat
echo "gmsh FarField1.geo"
echo "Mesh.RecombinationAlgorithm = 1;" >> FarField1.geo
gmsh FarField1.geo -2 -algo del2d -clscale 10 -smooth 5 -format msh2 \
  -o FarField1.msh > logmesh.dat
echo "gmsh FarField2.geo"
echo "Mesh.RecombinationAlgorithm = 1;" >> FarField2.geo
gmsh FarField2.geo -2 -algo del2d -clscale 10 -smooth 5 -format msh2 \
  -o FarField2.msh > logmesh.dat
./meshOut merge FarField0.msh FarField1.msh FarField2.msh farbox
FieldConvert outerRegion.xml outerRegion.plt -f
