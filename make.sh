#!/usr/bin/env bash
set -euo pipefail

c++ -std=c++11 -DOUTPUTEXP -O2 \
    mesh.cpp \
    CAD2D/MeshRegion.cpp \
    CAD2D/MeshRegions.cpp \
    CAD2D/tinyxml2.cpp \
    CAD2D/util.cpp \
    -o mesh-g

./mesh-g "$@"
