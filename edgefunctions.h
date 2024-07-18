#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"
double g_ptsW[20][2];
double g_ptsF[20][2];
std::vector<void *> BLedges;

static std::vector<double> BLedge0(double s) { return BLModel->edge0(s); }
static std::vector<double> BLedge1(double s) { return BLModel->edge1(s); }
static std::vector<double> BLedge2(double s) { return BLModel->edge2(s); }
static std::vector<double> BLedge3(double s) { return BLModel->edge3(s); }
static std::vector<double> BLedge4(double s) { return BLModel->edge4(s); }
static std::vector<double> BLedge5(double s) { return BLModel->edge5(s); }
static std::vector<double> BLedge6(double s) { return BLModel->edge6(s); }
static std::vector<double> BLedge7(double s) { return BLModel->edge7(s); }
static std::vector<double> BLedge8(double s) { return BLModel->edge8(s); }
static std::vector<double> BLedge9(double s) { return BLModel->edge9(s); }
static std::vector<double> BLedge10(double s) { return BLModel->edge10(s); }
static std::vector<double> BLedge11(double s) { return BLModel->edge11(s); }
static std::vector<double> BLedge12(double s) { return BLModel->edge12(s); }
static std::vector<double> BLMapFunc(std::vector<double> p) {
  return BLModel->MapFunc(p);
}

int InitPts() {
  // far boundary
  g_ptsF[0][0] = nearBoxLeft;
  g_ptsF[1][0] = nearBoxRight;
  g_ptsF[2][0] = nearBoxRight;
  g_ptsF[3][0] = nearBoxLeft;
  g_ptsF[0][1] = nearBoxDown;
  g_ptsF[1][1] = nearBoxDown;
  g_ptsF[2][1] = nearBoxUp;
  g_ptsF[3][1] = nearBoxUp;

  BLedges.push_back((void *)BLedge0);
  BLedges.push_back((void *)BLedge1);
  BLedges.push_back((void *)BLedge2);
  BLedges.push_back((void *)BLedge3);
  BLedges.push_back((void *)BLedge4);
  BLedges.push_back((void *)BLedge5);
  BLedges.push_back((void *)BLedge6);
  BLedges.push_back((void *)BLedge7);
  BLedges.push_back((void *)BLedge8);
  BLedges.push_back((void *)BLedge9);
  BLedges.push_back((void *)BLedge10);
  BLedges.push_back((void *)BLedge11);
  BLedges.push_back((void *)BLMapFunc);
  return 0;
}

LineEdge Cwake01(g_ptsF[0], g_ptsF[1], nFarWakex, UNIFORM, 0., 0.);
LineEdge Cwake12(g_ptsF[1], g_ptsF[2], nFarWakey, UNIFORM, 0., 0.);
LineEdge Cwake23(g_ptsF[2], g_ptsF[3], nFarWakex, UNIFORM, 0., 0.);
LineEdge Cwake30(g_ptsF[3], g_ptsF[0], nFarWakey, UNIFORM, 0., 0.);
std::vector<double> wake01(double s) { return Cwake01.Evaluate(s); }
std::vector<double> wake12(double s) { return Cwake12.Evaluate(s); }
std::vector<double> wake23(double s) { return Cwake23.Evaluate(s); }
std::vector<double> wake30(double s) { return Cwake30.Evaluate(s); }

std::vector<double> g_boundingbox;
bool toremove(std::vector<double> p) {
  double eps = g_boundingbox[g_boundingbox.size() - 1];
  for (int i = 0; i < 2; ++i) {
    if (p[i] < g_boundingbox[2 * i] - eps ||
        p[i] > g_boundingbox[2 * i + 1] + eps) {
      return false;
    }
  }
  return true;
}

#endif
