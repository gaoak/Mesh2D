#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"

/////////////////////////////// do not modify
// the boundary edge should be continuous for real number s

double G_pts[NUMPTS][2];
double G_ptsA[NUMPTS][2];

static void transform(std::vector<double> &p, double AoA) {
  double x = p[0], y = p[1];
  p[0] = x * cos(AoA) + y * sin(AoA);
  p[1] = -x * sin(AoA) + y * cos(AoA);
}

int InitPts() {
  G_pts[0][0] = 0.;
  G_pts[0][1] = 0.;

  G_pts[1][0] = 1.;
  G_pts[1][1] = 0.;
  return 0;
}

// bottom wavy wall
LineEdge Cedge01(G_pts[0], G_pts[1], 200, UNIFORM, 0., 0.);
std::vector<double> edge01(double s) {
  double x = Cedge01.Evaluate(s)[0];
  std::vector<double> p(3, 0.);
  p[0] = x;
  p[1] = 0.2 * (cos(x * 2 * M_PI) - 1.);
  return p;
}

double thickFunc01(std::vector<double> p) {
  return 0.0 * (p[0] - 0.5) * (p[0] - 0.5) * 4 + 0.1;
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

double thickFuncOval(std::vector<double> p) { return 0.1; }

LineEdge Cedge52(G_pts[5], G_pts[2], 20, UNIFORM, 0., 0.);
std::vector<double> edge52(double s) { return Cedge52.Evaluate(s); }

LineEdge CedgeA5(G_ptsA[0], G_pts[5], 10, BOUNDARYLAYER0, maxLayerh, 1.2, 5, 0.,
                 0., 0);
LineEdge CedgeB2(G_ptsA[1], G_pts[2], 10, BOUNDARYLAYER0, maxLayerh, 1.2, 5, 0.,
                 0., 0);
std::vector<double> edgeA5(double s) { return CedgeA5.Evaluate(s); }
std::vector<double> edgeB2(double s) { return CedgeB2.Evaluate(s); }

bool leftBnd(std::vector<double> p) { return fabs(p[0] - G_pts[0][0]) < 1E-6; }

bool rightBnd(std::vector<double> p) { return fabs(p[0] - G_pts[1][0]) < 1E-6; }

bool upperBnd(std::vector<double> p) { return fabs(p[1] - G_pts[4][1]) < 1E-6; }
#endif
