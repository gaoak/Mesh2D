#!/usr/bin/env bash
set -euo pipefail

c++ -std=c++11 -DOUTPUTEXP -O2 \
    mesh.cpp \
    CAD2D/CompositEdge.cpp \
    CAD2D/LineEdge.cpp \
    CAD2D/MeshRegion.cpp \
    CAD2D/MeshRegions.cpp \
    CAD2D/RectRegion.cpp \
    CAD2D/tinyxml2.cpp \
    CAD2D/util.cpp \
    -o mesh-g

output_file="${1:-ring.xml}"
gmsh_bin="${GMSH_BIN:-gmsh}"

if ! command -v "$gmsh_bin" >/dev/null 2>&1; then
    echo "Gmsh executable not found: $gmsh_bin" >&2
    exit 1
fi

./mesh-g geometry core.geo
"$gmsh_bin" core.geo -2 -algo del2d -format msh2 -o core.msh \
    > logmesh.dat 2>&1
./mesh-g merge core.msh "$output_file"
