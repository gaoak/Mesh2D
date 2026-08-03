#ifndef PARAMS_H
#define PARAMS_H

// Geometry.
const double innerCenterX = 0.5;
const double innerCenterY = 0.0;
const double innerRadius = 0.2;
const double outerCenterX = 0.0;
const double outerCenterY = 0.0;
const double outerRadius = 1.0;

// Independent circumferential divisions on the two cylinder surfaces.
const int innerCircumferentialCells = 64;
const int outerCircumferentialCells = 64;

// Boundary layers at both circular surfaces. Cell counts are calculated from
// the first-layer thickness, growth ratio and total thickness.
const double innerFirstLayer = 0.01;
const double innerLayerGrowth = 1.30;
const double innerBoundaryLayerThickness = 0.1;
const double outerFirstLayer = 0.01;
const double outerLayerGrowth = 1.30;
const double outerBoundaryLayerThickness = 0.1;

// Target element size in the unstructured core region.
const double coreMeshSize = 0.04;

// Number of high-order points written for every curved boundary edge.
const int curvedPoints = 7;
const double meshTolerance = 1.0e-10;

#endif
