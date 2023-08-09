#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"

/////////////////////////////// do not modify
// the boundary edge should be continuous for real number s
double G_pts[NUMPTS][2];
double G_ptsA[NUMPTS][2];

int InitPts() {
  G_pts[0][0] = 0.;
  G_pts[0][1] = 0.;

  G_pts[1][0] = waveLength * numWaves;
  G_pts[1][1] = 0.;

  G_pts[2][0] = G_pts[1][0];

  G_pts[3][0] = G_pts[1][0];
  G_pts[3][1] = centralHeight;

  G_pts[4][0] = G_pts[0][0];
  G_pts[4][1] = centralHeight;

  G_pts[5][0] = G_pts[0][0];

  G_pts[6][0] = G_pts[1][0];
  G_pts[6][1] = upperHeight;

  G_pts[7][0] = G_pts[0][0];
  G_pts[7][1] = upperHeight;

  G_ptsA[0][0] = G_pts[0][0];
  G_ptsA[1][0] = G_pts[1][0];
  return 0;
}

// bottom wavy wall
LineEdge Cedge01(G_pts[0], G_pts[1], bottomNx, UNIFORM, 0., 0.);
std::vector<double> edge01(double s) {
  double x = Cedge01.Evaluate(s)[0];
  std::vector<double> p(3, 0.);
  p[0] = x;
  p[1] = waveAmplitude * cos(x * 2. * M_PI / waveLength);
  return p;
}

double thickFunc01(std::vector<double> p) {
  return 0.0 * (p[0] - 0.5) * (p[0] - 0.5) * 4 + rBoundaryLayerBottom;
}

// oval shap
std::vector<double> edgeinner(double s) {
  std::vector<double> p(3, 0.);
  // p[0] = x;
  // p[1] = 0.2 * cos(x * 2 * M_PI);
  p[0] = 0.5 + 0.1 * cos((0.3 - s) * M_PI);
  p[1] = 0.25 + 0.25 * sin((0.3 - s) * M_PI);
  return p;
}
// bottom boundary layer
double thickFuncOval(std::vector<double> p) { return 0.1; }

// bottom gap
LineEdge CedgeA5(G_ptsA[0], G_pts[5], bottomGapNy, UNIFORM, 0., 0.);
std::vector<double> edgeA5(double s) { return CedgeA5.Evaluate(s); }
LineEdge CedgeB2(G_ptsA[1], G_pts[2], bottomGapNy, UNIFORM, 0., 0.);
std::vector<double> edgeB2(double s) { return CedgeB2.Evaluate(s); }

// central fluid domain
LineEdge Cedge52(G_pts[5], G_pts[2], centralNx, UNIFORM, 0., 0.);
std::vector<double> edge52(double s) { return Cedge52.Evaluate(s); }
LineEdge Cedge43(G_pts[4], G_pts[3], centralNx, UNIFORM, 0., 0.);
std::vector<double> edge43(double s) { return Cedge43.Evaluate(s); }
LineEdge Cedge54(G_pts[5], G_pts[4], centralNy, UNIFORM, 0., 0);
std::vector<double> edge54(double s) { return Cedge54.Evaluate(s); }
LineEdge Cedge23(G_pts[2], G_pts[3], centralNy, UNIFORM, 0., 0);
std::vector<double> edge23(double s) { return Cedge23.Evaluate(s); }

// upper fluid domain
LineEdge Cedge47(G_pts[4], G_pts[7], upperNy, BOUNDARYLAYER1, 0., 0., 0,
                 hFirstLayerUpper, upperProgress, upperNy - 1);
std::vector<double> edge47(double s) { return Cedge47.Evaluate(s); }
LineEdge Cedge36(G_pts[3], G_pts[6], upperNy, BOUNDARYLAYER1, 0., 0., 0,
                 hFirstLayerUpper, upperProgress, upperNy - 1);
std::vector<double> edge36(double s) { return Cedge36.Evaluate(s); }
LineEdge Cedge76(G_pts[7], G_pts[6], centralNx, UNIFORM, 0., 0);
std::vector<double> edge76(double s) { return Cedge76.Evaluate(s); }

bool leftBnd(std::vector<double> p) {
  return fabs(p[0] - G_pts[5][0]) + fabs(p[1] - G_pts[5][1]) < 1E-6;
}

bool rightBnd(std::vector<double> p) {
  return fabs(p[0] - G_pts[3][0]) + fabs(p[1] - G_pts[3][1]) < 1E-6;
}

bool upperBnd(std::vector<double> p) {
  return fabs(p[1] - G_pts[7][1]) < 1E-6 && G_pts[7][0] < p[0] &&
         p[0] < G_pts[6][0];
}
#endif
