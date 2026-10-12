#!/bin/bash
set -e
# Usage: bash makein.sh; all mesh settings are in params.h.
if [ "$#" -ne 0 ]; then
  echo "Usage: bash makein.sh (set mesh parameters in params.h)" >&2
  exit 1
fi
cd "$(dirname "$0")"
mkdir -p build
cd build
cmake ..
make meshIn
./meshIn
gmsh FarField0.geo -2 -algo meshadapt -format msh2 -o FarField0.msh > logmesh.dat 2>&1
./meshIn merge FarField0.msh
if command -v FieldConvert >/dev/null; then
  FieldConvert inFoil.xml inFoil.plt -f
else
  echo "Generated build/inFoil.xml; FieldConvert is not on PATH, skipped PLT export."
fi
