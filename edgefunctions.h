#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H

#include <cmath>
#include <iostream>
#include <vector>

#include "CAD2D/CompositEdge.h"
#include "CAD2D/LineEdge.h"
#include "CAD2D/util.h"
#include "params.h"

// Corner and y-partition points:
//   6 ----- 7       yMax
//   4 ----- 5       yUpperLayerStart
//   2 ----- 3       yLowerLayerEnd
//   0 ----- 1       yMin
double pts[8][2];

int NyLower = 0;
int NyCore = 0;
int NyUpper = 0;

LineEdge bottomEdge;
LineEdge topEdge;
LineEdge leftLowerEdge;
LineEdge leftCoreEdge;
LineEdge leftUpperEdge;
LineEdge rightLowerEdge;
LineEdge rightCoreEdge;
LineEdge rightUpperEdge;

CompositEdge leftEdge;
CompositEdge rightEdge;

std::vector<double> edgeBottom(double s) { return bottomEdge.Evaluate(s); }
std::vector<double> edgeTop(double s) { return topEdge.Evaluate(s); }

std::vector<double> edgeLeftLower(double s) {
    return leftLowerEdge.Evaluate(s);
}
std::vector<double> edgeLeftCore(double s) {
    return leftCoreEdge.Evaluate(s);
}
std::vector<double> edgeLeftUpper(double s) {
    return leftUpperEdge.Evaluate(s);
}
std::vector<double> edgeLeft(double s) { return leftEdge.Evaluate(s); }

std::vector<double> edgeRightLower(double s) {
    return rightLowerEdge.Evaluate(s);
}
std::vector<double> edgeRightCore(double s) {
    return rightCoreEdge.Evaluate(s);
}
std::vector<double> edgeRightUpper(double s) {
    return rightUpperEdge.Evaluate(s);
}
std::vector<double> edgeRight(double s) { return rightEdge.Evaluate(s); }

bool validateParameters() {
    bool valid = true;

    if (xMax <= xMin) {
        std::cerr << "error: xMax must be greater than xMin\n";
        valid = false;
    }
    if (Nx < 1) {
        std::cerr << "error: Nx must be at least 1\n";
        valid = false;
    }
    if (!(yMin < yLowerLayerEnd &&
          yLowerLayerEnd < yUpperLayerStart &&
          yUpperLayerStart < yMax)) {
        std::cerr << "error: y coordinates must satisfy "
                  << "yMin < yLowerLayerEnd < yUpperLayerStart < yMax\n";
        valid = false;
    }
    if (yFirstLayerHeight <= 0.0 || yLayerGrowth < 1.0 ||
        yMaximumLayerHeight <= 0.0 || yCoreMeshSize <= 0.0) {
        std::cerr << "error: y mesh sizes must be positive and "
                  << "yLayerGrowth must be at least 1\n";
        valid = false;
    }

    return valid;
}

int initialiseEdges() {
    pts[0][0] = xMin;
    pts[0][1] = yMin;
    pts[1][0] = xMax;
    pts[1][1] = yMin;

    pts[2][0] = xMin;
    pts[2][1] = yLowerLayerEnd;
    pts[3][0] = xMax;
    pts[3][1] = yLowerLayerEnd;

    pts[4][0] = xMin;
    pts[4][1] = yUpperLayerStart;
    pts[5][0] = xMax;
    pts[5][1] = yUpperLayerStart;

    pts[6][0] = xMin;
    pts[6][1] = yMax;
    pts[7][0] = xMax;
    pts[7][1] = yMax;

    NyLower = findNlayers(yFirstLayerHeight, yLayerGrowth,
                          yLowerLayerEnd - yMin,
                          yMaximumLayerHeight);
    NyCore = static_cast<int>(
        std::ceil((yUpperLayerStart - yLowerLayerEnd) / yCoreMeshSize));
    NyUpper = findNlayers(yFirstLayerHeight, yLayerGrowth,
                          yMax - yUpperLayerStart,
                          yMaximumLayerHeight);

    bottomEdge = LineEdge(pts[0], pts[1], Nx, UNIFORM, 0.0, 0.0);
    topEdge = LineEdge(pts[6], pts[7], Nx, UNIFORM, 0.0, 0.0);

    leftLowerEdge = LineEdge(pts[0], pts[2], NyLower, BOUNDARYLAYER0,
                             yFirstLayerHeight, yLayerGrowth, NyLower - 1,
                             0.0, 0.0, 0);
    leftCoreEdge = LineEdge(pts[2], pts[4], NyCore, UNIFORM, 0.0, 0.0);
    leftUpperEdge = LineEdge(pts[4], pts[6], NyUpper, BOUNDARYLAYER1,
                             0.0, 0.0, 0, yFirstLayerHeight,
                             yLayerGrowth, NyUpper - 1);

    rightLowerEdge = LineEdge(pts[1], pts[3], NyLower, BOUNDARYLAYER0,
                              yFirstLayerHeight, yLayerGrowth, NyLower - 1,
                              0.0, 0.0, 0);
    rightCoreEdge = LineEdge(pts[3], pts[5], NyCore, UNIFORM, 0.0, 0.0);
    rightUpperEdge = LineEdge(pts[5], pts[7], NyUpper, BOUNDARYLAYER1,
                              0.0, 0.0, 0, yFirstLayerHeight,
                              yLayerGrowth, NyUpper - 1);

    leftEdge.addEdge(leftLowerEdge,
                     reinterpret_cast<void *>(edgeLeftLower));
    leftEdge.addEdge(leftCoreEdge,
                     reinterpret_cast<void *>(edgeLeftCore));
    leftEdge.addEdge(leftUpperEdge,
                     reinterpret_cast<void *>(edgeLeftUpper));

    rightEdge.addEdge(rightLowerEdge,
                      reinterpret_cast<void *>(edgeRightLower));
    rightEdge.addEdge(rightCoreEdge,
                      reinterpret_cast<void *>(edgeRightCore));
    rightEdge.addEdge(rightUpperEdge,
                      reinterpret_cast<void *>(edgeRightUpper));

    return 0;
}

#endif
