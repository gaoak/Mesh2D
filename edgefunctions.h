#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"
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
  double deltax = (xBoxRight - xBoxLeft) / nBoxDown;
  double deltay = (yBoxUp - yBoxDown) / nBoxLeft;
  g_ptsF[0][0] = xBoxLeft;
  g_ptsF[1][0] = xBoxLeft + deltax;
  g_ptsF[2][0] = xBoxRight - deltax;
  g_ptsF[3][0] = xBoxRight;
  g_ptsF[0][1] = yBoxDown;
  g_ptsF[1][1] = yBoxDown;
  g_ptsF[2][1] = yBoxDown;
  g_ptsF[3][1] = yBoxDown;

  g_ptsF[8][0] = xBoxLeft;
  g_ptsF[9][0] = xBoxLeft + deltax;
  g_ptsF[10][0] = xBoxRight - deltax;
  g_ptsF[11][0] = xBoxRight;
  g_ptsF[8][1] = yBoxUp;
  g_ptsF[9][1] = yBoxUp;
  g_ptsF[10][1] = yBoxUp;
  g_ptsF[11][1] = yBoxUp;

  g_ptsF[4][0] = xBoxLeft + deltax;
  g_ptsF[5][0] = xBoxRight - deltax;
  g_ptsF[4][1] = yBoxDown + deltay;
  g_ptsF[5][1] = yBoxDown + deltay;

  g_ptsF[6][0] = xBoxLeft + deltax;
  g_ptsF[7][0] = xBoxRight - deltax;
  g_ptsF[6][1] = yBoxUp - deltay;
  g_ptsF[7][1] = yBoxUp - deltay;
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

LineEdge Cfar03(g_ptsF[0], g_ptsF[3], nBoxDown, UNIFORM, 0., 0.);
LineEdge Cfar811(g_ptsF[8], g_ptsF[11], nBoxUp, UNIFORM, 0., 0.);
LineEdge Cfar08(g_ptsF[0], g_ptsF[8], nBoxLeft, UNIFORM, 0., 0.);
LineEdge Cfar311(g_ptsF[3], g_ptsF[11], nBoxRight, UNIFORM, 0., 0.);
std::vector<double> far03(double s) { return Cfar03.Evaluate(s); }
std::vector<double> far811(double s) { return Cfar811.Evaluate(s); }
std::vector<double> far08(double s) { return Cfar08.Evaluate(s); }
std::vector<double> far311(double s) { return Cfar311.Evaluate(s); }

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
