#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H

#include "params.h"

#include <cmath>
#include <vector>

// Geometry functions for the annular computational domain. Boundary
// parameter s is in [-1, 1] and traverses one complete circle.
inline std::vector<double> ringPoint(double radius, double theta) {
  return {centerX + radius * std::cos(theta),
          centerY + radius * std::sin(theta)};
}

inline std::vector<double> innerBoundary(double s) {
  const double theta = std::acos(-1.0) * (s + 1.0);
  return ringPoint(innerRadius, theta);
}

inline std::vector<double> outerBoundary(double s) {
  const double theta = std::acos(-1.0) * (s + 1.0);
  return ringPoint(outerRadius, theta);
}

// Radial node distribution. Consecutive layers follow radialGrowth.
inline double ringRadius(int radialNode) {
  const double fraction = static_cast<double>(radialNode) / radialCells;
  if (std::fabs(radialGrowth - 1.0) < 1.0e-12) {
    return innerRadius + (outerRadius - innerRadius) * fraction;
  }

  const double numerator = std::pow(radialGrowth, radialNode) - 1.0;
  const double denominator = std::pow(radialGrowth, radialCells) - 1.0;
  return innerRadius + (outerRadius - innerRadius) * numerator / denominator;
}

// Topology of one annular quadrilateral. Vertices are counter-clockwise:
// inner/start -> outer/start -> outer/end -> inner/end.
inline std::vector<std::vector<double>>
ringElementVertices(int circumferentialCell, int radialCell) {
  const double dTheta =
      2.0 * std::acos(-1.0) / circumferentialCells;
  const double theta0 = circumferentialCell * dTheta;
  const double theta1 = (circumferentialCell + 1) * dTheta;
  const double r0 = ringRadius(radialCell);
  const double r1 = ringRadius(radialCell + 1);

  return {ringPoint(r0, theta0), ringPoint(r1, theta0),
          ringPoint(r1, theta1), ringPoint(r0, theta1)};
}

#endif
