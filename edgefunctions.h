#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "CAD2D/cylinder.h"
#include "params.h"

/////////////////////////////// do not modify
// the boundary edge should be continuous for real number s
int findNlayers(double h, double q, double R, double m) {
  int n = 0;
  double len = 0;
  double delta = h;
  for (n = 1; n <= 1000000; ++n) {
    if (delta >= m)
      delta = m;
    len += delta;
    if (len >= R)
      return n;
    delta *= q;
  }
  return n;
}
int nLayers = findNlayers(hFirstLayer, progress, rBoundaryLayer, maxLayerh);

double pts[NUMPTS][2];
double squarePts[NUMPTS][2];
double virtualpts[NUMPTS][2];

LineEdge CradiusEdge(pts[0], pts[1], nLayers, UNIFORM, 0., 0.);
void setRadiusLayers(int n) { nLayers = n; }
std::vector<double> radiusEdge(double s) {
  int n = round(0.5 * (1. + s) * nLayers);
  static vector<vector<double>> reses;
  if (reses.size() < nLayers + 1) {
    reses.clear();
    vector<double> p0(2, 0.);
    reses.push_back(p0);
    double delta = hFirstLayer;
    for (int n = 1; n <= nLayers; ++n) {
      vector<double> p1(2, 0.);
      if (delta >= maxLayerh)
        delta = maxLayerh;
      p1[0] = reses[reses.size() - 1][0] + delta;
      delta *= progress;
      reses.push_back(p1);
    }
  }
  return reses[n];
}

int nLayersInFoil = findNlayers(hFirstLayerInFoil, progressInFoil,
                                rBoundaryLayerInFoil, maxLayerhInFoil);

std::vector<double> radiusEdgeInFoil(double s) {
  int n = round(0.5 * (1. + s) * nLayersInFoil);
  static vector<vector<double>> reses;
  if (reses.size() < nLayersInFoil + 1) {
    vector<double> p0(2, 0.);
    reses.push_back(p0);
    double delta = hFirstLayerInFoil;
    for (int n = 1; n <= nLayersInFoil; ++n) {
      vector<double> p1(2, 0.);
      if (delta >= maxLayerhInFoil)
        delta = maxLayerhInFoil;
      p1[0] = reses[reses.size() - 1][0] + delta;
      delta *= progressInFoil;
      reses.push_back(p1);
    }
  }
  return reses[n];
}

static void transform(std::vector<double> &p, double AoA) {
  double x = p[0], y = p[1];
  p[0] = x * cos(AoA) + y * sin(AoA);
  p[1] = -x * sin(AoA) + y * cos(AoA);
}

int InitPts() {

  pts[8][0] = xBoxRight;
  pts[8][1] = yBoxDown;

  pts[9][0] = xBoxRight;
  pts[9][1] = yBoxUp;

  pts[10][0] = xBoxLeft;
  pts[10][1] = yBoxUp;

  pts[11][0] = xBoxLeft;
  pts[11][1] = yBoxDown;

  return 0;
}

CompositEdge CinnerEdge;
std::vector<double> innerEdge(double s) { return CinnerEdge.Evaluate(s); }

// airfoil surfaces
int nLayers2 = findNlayers(hFirstLayer, progress, rBoundaryLayer2, maxLayerh);
int nLayers3 = findNlayers(hFirstLayer, progress, rBoundaryLayer3, maxLayerh);
int nLayers4 = findNlayers(hFirstLayer, progress, rBoundaryLayer4, maxLayerh);
int nLayers5 = findNlayers(hFirstLayer, progress, rBoundaryLayer5, maxLayerh);

Cylinder cyl2(0.5 * chordLen, theta6, theta32);
Cylinder cyl3(0.5 * chordLen, theta33, theta4);
Cylinder cyl4(0.5 * chordLen, theta4, theta5);
Cylinder cyl5(0.5 * chordLen, theta5, theta6);
std::vector<double> edge2(double s) { return cyl2.GetPoint(s); }
std::vector<double> edge3(double s) { return cyl3.GetPoint(s); }
std::vector<double> edge4(double s) { return cyl4.GetPoint(s); }
std::vector<double> edge5(double s) { return cyl5.GetPoint(s); }

// straight edges in the outter box
CompositEdge CoutterEdge;
std::vector<double> outterEdge(double s) { return CoutterEdge.Evaluate(s); }
LineEdge Cedge9(pts[8], pts[9], nBoxRight, UNIFORM, 0., 0.);
LineEdge Cedge10(pts[9], pts[10], nBoxUp, UNIFORM, 0., 0.);
LineEdge Cedge11(pts[10], pts[11], nBoxLeft, UNIFORM, 0., 0.);
LineEdge Cedge12(pts[11], pts[8], nBoxDown, UNIFORM, 0., 0.);
std::vector<double> edge9(double s) { return Cedge9.Evaluate(s); }
std::vector<double> edge10(double s) { return Cedge10.Evaluate(s); }
std::vector<double> edge11(double s) { return Cedge11.Evaluate(s); }
std::vector<double> edge12(double s) { return Cedge12.Evaluate(s); }

// straight edges in the near-field square
LineEdge Cedge19(pts[16], pts[17], nFarWakex, QUDREFINE0,
                 (farWakeUp - farWakeDown) / nFarWakey, 0.);
LineEdge Cedge20(pts[17], pts[18], nFarWakey, UNIFORM, 0., 0.);
LineEdge Cedge21(pts[18], pts[19], nFarWakex, QUDREFINE1, 0.,
                 (farWakeUp - farWakeDown) / nFarWakey);
LineEdge Cedge22(pts[19], pts[16], nFarWakey, UNIFORM, 0., 0.);
std::vector<double> edge19(double s) { return Cedge19.Evaluate(s); }
std::vector<double> edge20(double s) { return Cedge20.Evaluate(s); }
std::vector<double> edge21(double s) { return Cedge21.Evaluate(s); }
std::vector<double> edge22(double s) { return Cedge22.Evaluate(s); }

// straight edges in the square region
LineEdge CedgeA(squarePts[0], squarePts[1], nSquarex, QUDREFINE0,
                (SquareUp - SquareDown) / nSquarey, 0.);
LineEdge CedgeB(squarePts[1], squarePts[2], nSquarey, UNIFORM, 0., 0.);
LineEdge CedgeC(squarePts[2], squarePts[3], nSquarex, QUDREFINE1, 0.,
                (SquareUp - SquareDown) / nSquarey);
LineEdge CedgeD(squarePts[3], squarePts[0], nSquarey, UNIFORM, 0., 0.);
std::vector<double> edgeA(double s) { return CedgeA.Evaluate(s); }
std::vector<double> edgeB(double s) { return CedgeB.Evaluate(s); }
std::vector<double> edgeC(double s) { return CedgeC.Evaluate(s); }
std::vector<double> edgeD(double s) { return CedgeD.Evaluate(s); }

std::vector<double> boundingbox;
bool toremove(std::vector<double> p) {
  double eps = boundingbox[boundingbox.size() - 1];
  for (int i = 0; i < 2; ++i) {
    if (p[i] < boundingbox[2 * i] - eps ||
        p[i] > boundingbox[2 * i + 1] + eps) {
      return false;
    }
  }
  return true;
}
#endif
