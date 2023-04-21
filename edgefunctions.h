#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
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
int nLayers =
    findNlayers(hFirstLayer, progress, yBoxUp1 - hFirstLayer, maxLayerh);

double pts[NUMPTS][2];
double vpts[NUMPTS][2];

static void transform(std::vector<double> &p, double AoA) {
  double x = p[0], y = p[1];
  p[0] = x * cos(AoA) + y * sin(AoA);
  p[1] = -x * sin(AoA) + y * cos(AoA);
}

int InitPts() {
  pts[0][0] = xBoxLeft;
  pts[0][1] = yBoxDown;

  pts[1][0] = xBoxRight;
  pts[1][1] = yBoxDown;

  pts[2][0] = xBoxRight;
  pts[2][1] = yBoxUp2;

  pts[3][0] = xBoxLeft;
  pts[3][1] = yBoxUp2;

  pts[4][0] = nozzlex0;
  pts[4][1] = nozzley0;

  pts[5][0] = nozzlex0 + nozzleradius0 * cos(nozzletheta0) - nozzleradius0 * cos(nozzletheta1);
  pts[5][1] = nozzley0 + nozzleradius0 * sin(nozzletheta0) - nozzleradius0 * sin(nozzletheta1);

  pts[6][0] = nozzlex0 + nozzleradius1 * cos(nozzletheta0) -  nozzleradius0 * cos(nozzletheta1);
  pts[6][1] = nozzley0 + nozzleradius1 * sin(nozzletheta0) - nozzleradius0 * sin(nozzletheta1);

  pts[7][0] = nozzlex0 + nozzleradius1 * cos(nozzletheta1) - nozzleradius0 * cos(nozzletheta1);
  pts[7][1] = nozzley0 + nozzleradius1 * sin(nozzletheta1) - nozzleradius0 * sin(nozzletheta1);
  return 0;
}

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
LineEdge Cedge0(pts[0], pts[1], nBoxDown, UNIFORM, 0., 0.);
std::vector<double> edge0(double s) { return Cedge0.Evaluate(s); }

LineEdge Cedgev0(vpts[0], vpts[1], nBoxDown, UNIFORM, 0., 0.);
std::vector<double> edgev0(double s) { return Cedgev0.Evaluate(s); }

LineEdge Cedge1(vpts[1], pts[2], nBoxRight, BOUNDARYLAYER0, maxLayerh, 1.2, 6, 0., 0., 0);
std::vector<double> edge1(double s) { return Cedge1.Evaluate(s); }

LineEdge Cedge2(pts[2], pts[3], nBoxDown, UNIFORM, 0., 0.);
std::vector<double> edge2(double s) { return Cedge2.Evaluate(s); }

LineEdge Cedge3(vpts[0], pts[3], nBoxLeft, BOUNDARYLAYER0, maxLayerh, 1.2, 6, 0., 0., 0);
std::vector<double> edge3(double s) { return Cedge3.Evaluate(s); }

LineEdge Cedge4(pts[4], pts[5], NxNozzel, UNIFORM, 0., 0.);
std::vector<double> edge4(double s) { return Cedge4.Evaluate(s); }

LineEdge Cedge5(pts[5], pts[6], NyNozzel, UNIFORM, 0., 0.);
std::vector<double> edge5(double s) { return Cedge5.Evaluate(s); }

LineEdge Cedge6(pts[6], pts[7], NxNozzel, UNIFORM, 0., 0.);
std::vector<double> edge6(double s) { return Cedge6.Evaluate(s); }

LineEdge Cedge7(pts[7], pts[4], NyNozzel, UNIFORM, 0., 0.);
std::vector<double> edge7(double s) { return Cedge7.Evaluate(s); }

bool bottomwall(vector<double> p) {
  return p[1]<=-0.014;
}

bool leftside(vector<double> p) {
  return p[0]<= xBoxLeft + 1E-10 && (p[1]>0.04) && (p[1]<0.06);
}

bool rightside(vector<double> p) {
  return p[0]>=xBoxRight-1E-10 && (p[1]>0.04) && (p[1]<0.06);
}

bool upperside(vector<double> p) {
  return p[1]>= 0.1;
}

bool nozzleoutlet(vector<double> p) {
  double x0 = 0.142214;
  double y0 = 0.0284089;
  double r = 0.002;
  return fabs(p[0]-x0)+fabs(p[1]-y0) <= r;
}

bool nozzlewall(vector<double> p) {
  double x0 = 0.1423;
  double y0 = 0.0323;
  double r = 0.01;
  return fabs(p[0]-x0)+fabs(p[1]-y0) <= r;
}

bool tool(vector<double> p) {
  return true;
}
#endif
