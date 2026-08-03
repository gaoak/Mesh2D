#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H

#include "CAD2D/CompositEdge.h"
#include "CAD2D/LineEdge.h"
#include "params.h"

#include <algorithm>
#include <cmath>
#include <vector>

inline std::vector<double> circlePoint(double centerX, double centerY,
                                       double radius, double theta) {
  return {centerX + radius * std::cos(theta),
          centerY + radius * std::sin(theta)};
}

inline std::vector<double> innerBoundary(double s) {
  const double theta = std::acos(-1.0) * (s + 1.0);
  return circlePoint(innerCenterX, innerCenterY, innerRadius, theta);
}

inline std::vector<double> outerBoundary(double s) {
  const double theta = std::acos(-1.0) * (s + 1.0);
  return circlePoint(outerCenterX, outerCenterY, outerRadius, theta);
}

inline std::vector<double> innerCoreBoundary(double s) {
  const double theta = std::acos(-1.0) * (s + 1.0);
  return circlePoint(innerCenterX, innerCenterY,
                     innerRadius + innerBoundaryLayerThickness, theta);
}

inline std::vector<double> outerCoreBoundary(double s) {
  const double theta = std::acos(-1.0) * (s + 1.0);
  return circlePoint(outerCenterX, outerCenterY,
                     outerRadius - outerBoundaryLayerThickness, theta);
}

inline double geometricThickness(double firstLayer, double growth, int layers,
                                 double maximumLayer) {
  double thickness = 0.0;
  double layer = firstLayer;
  for (int i = 0; i < layers; ++i) {
    thickness += std::min(layer, maximumLayer);
    layer *= growth;
  }
  return thickness;
}

// Derive the layer count from first-layer size, growth and total thickness,
// following the approach used by the square branch.
inline int findBoundaryLayerCells(double firstLayer, double growth,
                                  double totalThickness) {
  int cells = 0;
  double accumulated = 0.0;
  double layer = firstLayer;
  while (accumulated < totalThickness) {
    accumulated += std::min(layer, coreMeshSize);
    layer *= growth;
    ++cells;
  }
  return cells;
}

inline int innerBoundaryLayerCells() {
  return findBoundaryLayerCells(innerFirstLayer, innerLayerGrowth,
                                innerBoundaryLayerThickness);
}

inline int outerBoundaryLayerCells() {
  return findBoundaryLayerCells(outerFirstLayer, outerLayerGrowth,
                                outerBoundaryLayerThickness);
}

// Fit the growth ratio so that the derived number of layers ends exactly at
// the requested total thickness while retaining the first-layer thickness.
inline double resolvedGrowth(double firstLayer, double requestedGrowth,
                             double totalThickness, int cells) {
  if (cells <= 1 || requestedGrowth <= 1.0) {
    return requestedGrowth;
  }
  double low = 1.0;
  double high = requestedGrowth;
  for (int i = 0; i < 80; ++i) {
    const double middle = 0.5 * (low + high);
    if (geometricThickness(firstLayer, middle, cells, coreMeshSize) <
        totalThickness) {
      low = middle;
    } else {
      high = middle;
    }
  }
  return 0.5 * (low + high);
}

inline double boundaryLayerDistance(int layer, double firstLayer,
                                    double requestedGrowth,
                                    double totalThickness, int cells) {
  if (layer <= 0) {
    return 0.0;
  }
  if (layer >= cells) {
    return totalThickness;
  }
  const double growth = resolvedGrowth(firstLayer, requestedGrowth,
                                       totalThickness, cells);
  const double full =
      geometricThickness(firstLayer, growth, cells, coreMeshSize);
  const double partial =
      geometricThickness(firstLayer, growth, layer, coreMeshSize);
  return totalThickness * partial / full;
}

inline std::vector<double> circleSegment(double centerX, double centerY,
                                         double radius, double theta0,
                                         double theta1, double s) {
  const double theta = 0.5 * (1.0 - s) * theta0 +
                       0.5 * (1.0 + s) * theta1;
  return circlePoint(centerX, centerY, radius, theta);
}

// Inner-wall segments run clockwise, so eBoundaryLayer1's left normal points
// out of the inner cylinder and into the fluid domain.
inline std::vector<double> innerWallEdge0(double s) {
  return circleSegment(innerCenterX, innerCenterY, innerRadius, 0.0,
                       -0.5 * std::acos(-1.0), s);
}
inline std::vector<double> innerWallEdge1(double s) {
  return circleSegment(innerCenterX, innerCenterY, innerRadius,
                       -0.5 * std::acos(-1.0), -std::acos(-1.0), s);
}
inline std::vector<double> innerWallEdge2(double s) {
  return circleSegment(innerCenterX, innerCenterY, innerRadius,
                       -std::acos(-1.0), -1.5 * std::acos(-1.0), s);
}
inline std::vector<double> innerWallEdge3(double s) {
  return circleSegment(innerCenterX, innerCenterY, innerRadius,
                       -1.5 * std::acos(-1.0), -2.0 * std::acos(-1.0), s);
}

// Outer-wall segments run counter-clockwise, so the left normal points inward.
inline std::vector<double> outerWallEdge0(double s) {
  return circleSegment(outerCenterX, outerCenterY, outerRadius, 0.0,
                       0.5 * std::acos(-1.0), s);
}
inline std::vector<double> outerWallEdge1(double s) {
  return circleSegment(outerCenterX, outerCenterY, outerRadius,
                       0.5 * std::acos(-1.0), std::acos(-1.0), s);
}
inline std::vector<double> outerWallEdge2(double s) {
  return circleSegment(outerCenterX, outerCenterY, outerRadius,
                       std::acos(-1.0), 1.5 * std::acos(-1.0), s);
}
inline std::vector<double> outerWallEdge3(double s) {
  return circleSegment(outerCenterX, outerCenterY, outerRadius,
                       1.5 * std::acos(-1.0), 2.0 * std::acos(-1.0), s);
}

// CompositEdge uses the LineEdge objects for segment cell counts and the
// function pointers for geometry evaluation.
double innerThetaPoints[5][2] = {
    {0.0, 0.0},
    {-0.5 * std::acos(-1.0), 0.0},
    {-std::acos(-1.0), 0.0},
    {-1.5 * std::acos(-1.0), 0.0},
    {-2.0 * std::acos(-1.0), 0.0}};
double outerThetaPoints[5][2] = {
    {0.0, 0.0},
    {0.5 * std::acos(-1.0), 0.0},
    {std::acos(-1.0), 0.0},
    {1.5 * std::acos(-1.0), 0.0},
    {2.0 * std::acos(-1.0), 0.0}};

LineEdge CinnerWallEdge0(innerThetaPoints[0], innerThetaPoints[1],
                         innerCircumferentialCells / 4, UNIFORM, 0.0, 0.0);
LineEdge CinnerWallEdge1(innerThetaPoints[1], innerThetaPoints[2],
                         innerCircumferentialCells / 4, UNIFORM, 0.0, 0.0);
LineEdge CinnerWallEdge2(innerThetaPoints[2], innerThetaPoints[3],
                         innerCircumferentialCells / 4, UNIFORM, 0.0, 0.0);
LineEdge CinnerWallEdge3(innerThetaPoints[3], innerThetaPoints[4],
                         innerCircumferentialCells / 4, UNIFORM, 0.0, 0.0);
LineEdge CouterWallEdge0(outerThetaPoints[0], outerThetaPoints[1],
                         outerCircumferentialCells / 4, UNIFORM, 0.0, 0.0);
LineEdge CouterWallEdge1(outerThetaPoints[1], outerThetaPoints[2],
                         outerCircumferentialCells / 4, UNIFORM, 0.0, 0.0);
LineEdge CouterWallEdge2(outerThetaPoints[2], outerThetaPoints[3],
                         outerCircumferentialCells / 4, UNIFORM, 0.0, 0.0);
LineEdge CouterWallEdge3(outerThetaPoints[3], outerThetaPoints[4],
                         outerCircumferentialCells / 4, UNIFORM, 0.0, 0.0);

CompositEdge CinnerWall;
CompositEdge CouterWall;

inline void initialiseCompositeEdges() {
  static bool initialised = false;
  if (initialised) {
    return;
  }
  CinnerWall.addEdge(CinnerWallEdge0,
                     reinterpret_cast<void *>(innerWallEdge0));
  CinnerWall.addEdge(CinnerWallEdge1,
                     reinterpret_cast<void *>(innerWallEdge1));
  CinnerWall.addEdge(CinnerWallEdge2,
                     reinterpret_cast<void *>(innerWallEdge2));
  CinnerWall.addEdge(CinnerWallEdge3,
                     reinterpret_cast<void *>(innerWallEdge3));
  CouterWall.addEdge(CouterWallEdge0,
                     reinterpret_cast<void *>(outerWallEdge0));
  CouterWall.addEdge(CouterWallEdge1,
                     reinterpret_cast<void *>(outerWallEdge1));
  CouterWall.addEdge(CouterWallEdge2,
                     reinterpret_cast<void *>(outerWallEdge2));
  CouterWall.addEdge(CouterWallEdge3,
                     reinterpret_cast<void *>(outerWallEdge3));
  initialised = true;
}

inline std::vector<double> innerWallEdge(double s) {
  return CinnerWall.Evaluate(s);
}

inline std::vector<double> outerWallEdge(double s) {
  return CouterWall.Evaluate(s);
}

inline std::vector<double> innerNormalEdge(double s) {
  const int layer = static_cast<int>(
      std::lround(0.5 * (1.0 + s) * innerBoundaryLayerCells()));
  return {boundaryLayerDistance(layer, innerFirstLayer, innerLayerGrowth,
                                innerBoundaryLayerThickness,
                                innerBoundaryLayerCells()),
          0.0};
}

inline std::vector<double> outerNormalEdge(double s) {
  const int layer = static_cast<int>(
      std::lround(0.5 * (1.0 + s) * outerBoundaryLayerCells()));
  return {boundaryLayerDistance(layer, outerFirstLayer, outerLayerGrowth,
                                outerBoundaryLayerThickness,
                                outerBoundaryLayerCells()),
          0.0};
}

#endif
