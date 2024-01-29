#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"

double g_ptsA[20][2];
double g_ptsW[20][2];
double g_ptsF[20][2];
double g_thetaA[20][2];
int nLayersU0 = findNlayers(hFirstLayer, progress, upperBL0, maxLayerh);
int nLayersU1 = findNlayers(hFirstLayer, progress, upperBL1, maxLayerh);
int nLayersU2 = findNlayers(hFirstLayer, progress, upperBL2, maxLayerh);
int nLayersU3 = findNlayers(hFirstLayer, progress, upperBL3, maxLayerh);
int nLayersU4 = findNlayers(hFirstLayer, progress, upperBL4, maxLayerh);
int nLayersU5 = findNlayers(hFirstLayer, progress, upperBL5, maxLayerh);

int nLayersL0 = findNlayers(hFirstLayer, progress, lowerBL0, maxLayerh);
int nLayersL1 = findNlayers(hFirstLayer, progress, lowerBL1, maxLayerh);
int nLayersL2 = findNlayers(hFirstLayer, progress, lowerBL2, maxLayerh);
int nLayersL3 = findNlayers(hFirstLayer, progress, lowerBL3, maxLayerh);
int nLayersL4 = findNlayers(hFirstLayer, progress, lowerBL4, maxLayerh);
int nLayersL5 = findNlayers(hFirstLayer, progress, lowerBL5, maxLayerh);

int InitPts() {
  setRadiusMesh(hFirstLayer, progress, maxLayerh);
  // airfoil
  g_thetaA[0][0] = 0.5 * M_PI;
  g_thetaA[4][0] = 0.5 * M_PI;
  g_thetaA[5][0] = 0.;
  g_thetaA[6][0] = -0.5 * M_PI;
  g_thetaA[10][0] = 1.5 * M_PI;
  g_thetaA[11][0] = M_PI;

  g_ptsA[0][0] = 0.5 * Thickness;
  g_ptsA[1][0] = upperx1;
  g_ptsA[2][0] = upperx2;
  g_ptsA[3][0] = upperx3;
  g_ptsA[4][0] = 1. - 0.5 * Thickness;
  g_ptsA[0][1] = 0.5 * Thickness;
  g_ptsA[1][1] = 0.5 * Thickness;
  g_ptsA[2][1] = 0.5 * Thickness;
  g_ptsA[3][1] = 0.5 * Thickness;
  g_ptsA[4][1] = 0.5 * Thickness;

  g_ptsA[6][0] = 1. - 0.5 * Thickness;
  g_ptsA[7][0] = lowerx3;
  g_ptsA[8][0] = lowerx2;
  g_ptsA[9][0] = lowerx1;
  g_ptsA[10][0] = 0.5 * Thickness;
  g_ptsA[6][1] = -0.5 * Thickness;
  g_ptsA[7][1] = -0.5 * Thickness;
  g_ptsA[8][1] = -0.5 * Thickness;
  g_ptsA[9][1] = -0.5 * Thickness;
  g_ptsA[10][1] = -0.5 * Thickness;
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
  // wake
  g_ptsW[0][0] = farWakeCx + 0.5 * farWakeHeight * sin(farWakeAoA);
  g_ptsW[0][1] = farWakeCy - 0.5 * farWakeHeight * cos(farWakeAoA);
  g_ptsW[3][0] = farWakeCx - 0.5 * farWakeHeight * sin(farWakeAoA);
  g_ptsW[3][1] = farWakeCy + 0.5 * farWakeHeight * cos(farWakeAoA);
  double cx = farWakeCx + farWakeLength * cos(farWakeAoA);
  double cy = farWakeCy + farWakeLength * sin(farWakeAoA);
  double height =
      farWakeHeight + 2. * farWakeLength * tan(wakeDiffuseAngle * 0.5);
  g_ptsW[1][0] = cx + 0.5 * height * sin(farWakeAoA);
  g_ptsW[1][1] = cy - 0.5 * height * cos(farWakeAoA);
  g_ptsW[2][0] = cx - 0.5 * height * sin(farWakeAoA);
  g_ptsW[2][1] = cy + 0.5 * height * cos(farWakeAoA);
  return 0;
}

LineEdge Cedge1011(g_thetaA[10], g_thetaA[11], nLow0, UNIFORM, 0., 0.);
LineEdge Cedge110(g_thetaA[11], g_thetaA[0], nUp0, UNIFORM, 0., 0.);
LineEdge Cedge45(g_thetaA[4], g_thetaA[5], nUp5, UNIFORM, 0., 0.);
LineEdge Cedge56(g_thetaA[5], g_thetaA[6], nLow5, UNIFORM, 0., 0.);
std::vector<double> edge1011(double s) {
  double x0 = 0.5 * Thickness, radius = 0.5 * Thickness;
  double t = Cedge1011.Evaluate(s)[0];
  std::vector<double> res(2, 0.);
  res[0] = x0 + radius * cos(t);
  res[1] = radius * sin(t);
  return res;
}
std::vector<double> edge110(double s) {
  double x0 = 0.5 * Thickness, radius = 0.5 * Thickness;
  double t = Cedge110.Evaluate(s)[0];
  std::vector<double> res(2, 0.);
  res[0] = x0 + radius * cos(t);
  res[1] = radius * sin(t);
  return res;
}
std::vector<double> edge45(double s) {
  double x0 = 1. - 0.5 * Thickness, radius = 0.5 * Thickness;
  double t = Cedge45.Evaluate(s)[0];
  std::vector<double> res(2, 0.);
  res[0] = x0 + radius * cos(t);
  res[1] = radius * sin(t);
  return res;
}
std::vector<double> edge56(double s) {
  double x0 = 1. - 0.5 * Thickness, radius = 0.5 * Thickness;
  double t = Cedge56.Evaluate(s)[0];
  std::vector<double> res(2, 0.);
  res[0] = x0 + radius * cos(t);
  res[1] = radius * sin(t);
  return res;
}

LineEdge Cedge01(g_ptsA[0], g_ptsA[1], nUp1, BOUNDARYLAYER0, hFirstLayer, 1.6,
                 5, 0., 0., 0);
LineEdge Cedge12(g_ptsA[1], g_ptsA[2], nUp2, UNIFORM, 0., 0.);
LineEdge Cedge23(g_ptsA[2], g_ptsA[3], nUp3, UNIFORM, 0., 0.);
LineEdge Cedge34(g_ptsA[3], g_ptsA[4], nUp4, BOUNDARYLAYER1, 0., 0., 0,
                 hFirstLayer, 1.6, 5);
std::vector<double> edge01(double s) { return Cedge01.Evaluate(s); }
std::vector<double> edge12(double s) { return Cedge12.Evaluate(s); }
std::vector<double> edge23(double s) { return Cedge23.Evaluate(s); }
std::vector<double> edge34(double s) { return Cedge34.Evaluate(s); }

LineEdge Cedge67(g_ptsA[6], g_ptsA[7], nLow4, BOUNDARYLAYER0, hFirstLayer, 1.6,
                 5, 0., 0., 0);
LineEdge Cedge78(g_ptsA[7], g_ptsA[8], nLow3, UNIFORM, 0., 0.);
LineEdge Cedge89(g_ptsA[8], g_ptsA[9], nLow2, UNIFORM, 0., 0.);
LineEdge Cedge910(g_ptsA[9], g_ptsA[10], nLow1, BOUNDARYLAYER1, 0., 0., 0,
                  hFirstLayer, 1.6, 5);
std::vector<double> edge67(double s) { return Cedge67.Evaluate(s); }
std::vector<double> edge78(double s) { return Cedge78.Evaluate(s); }
std::vector<double> edge89(double s) { return Cedge89.Evaluate(s); }
std::vector<double> edge910(double s) { return Cedge910.Evaluate(s); }

LineEdge Cwake01(g_ptsW[0], g_ptsW[1], nFarWakex, QUDREFINE0,
                 farWakeHeight / nFarWakey, 0.);
LineEdge Cwake12(g_ptsW[1], g_ptsW[2], nFarWakey, UNIFORM, 0., 0.);
LineEdge Cwake23(g_ptsW[2], g_ptsW[3], nFarWakex, QUDREFINE1, 0.,
                 farWakeHeight / nFarWakey);
LineEdge Cwake30(g_ptsW[3], g_ptsW[0], nFarWakey, UNIFORM, 0., 0.);
std::vector<double> wake01(double s) { return Cwake01.Evaluate(s); }
std::vector<double> wake12(double s) { return Cwake12.Evaluate(s); }
std::vector<double> wake23(double s) { return Cwake23.Evaluate(s); }
std::vector<double> wake30(double s) { return Cwake30.Evaluate(s); }

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
