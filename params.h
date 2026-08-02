#ifndef PARAMS_H
#define PARAMS_H

// Geometry.
const double centerX = 0.0;
const double centerY = 0.0;
const double innerRadius = 1.0;
const double outerRadius = 2.0;

// Structured quadrilateral mesh resolution.
const int circumferentialCells = 128;
const int radialCells = 16;

// Ratio between successive radial cell widths. Use 1.0 for uniform spacing;
// values greater than 1.0 cluster cells next to the inner circle.
const double radialGrowth = 1.08;

// Number of high-order points written for every curved boundary edge.
const int curvedPoints = 7;
const double meshTolerance = 1.0e-10;

#endif
