// for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
#include "CAD2D/BLAirfoil.h"
#include "CAD2D/BLEllipse.h"
#include "CAD2D/BLFlatPlate.h"
#include <map>
#include <string>

BLMeshModuleShPtr BLModel;
double ChordLen = 1.0;
void DefineBLParams(std::map<std::string, double> &p,
                    std::map<std::string, int> &q) {
  double AoA = 0. / 180. * M_PI;
  p["AoA"] = AoA;
  double Thickness = 1.0;
  p["Thickness"] = Thickness;
  p["ChordLen"] = ChordLen;
  double Tx0 = 0.;
  p["Tx0"] = Tx0;
  // outside
  double hFirstLayer = 0.01;
  p["hFirstLayer"] = hFirstLayer;
  double progress = 1.;
  p["progress"] = progress;
  double maxLayerh = 0.025;
  p["maxLayerh"] = maxLayerh;
  double wallBLThickness0 = 0.02;
  p["wallBLThickness0"] = wallBLThickness0;

  // number starts from leading to trailing
  double eps = 0.25;
  double Theta0 = M_PI * (1. + eps);
  p["Theta0"] = Theta0;
  double Theta1 = M_PI * (1. - eps);
  p["Theta1"] = Theta1;
  double Theta2 = M_PI * eps;
  p["Theta2"] = Theta2;
  double Theta3 = -M_PI * eps;
  p["Theta3"] = Theta3;
  double Theta4 = -M_PI * (1. - eps);
  p["Theta4"] = Theta4;
  int nLE = 40;
  q["nLE"] = nLE;
  int nUp = 40;
  q["nUp"] = nUp;
  int nTE = 40;
  q["nTE"] = nTE;
  int nLow = 40;
  q["nLow"] = nLow;
  int curvedpts = 6;
  q["curvedpts"] = curvedpts;
  BLModel = std::make_shared<BLEllipse>(p, q);
  BLModel->Initialise();
}

double nearmaxLayerh = 0.05;
double nearBoxLeft = -3;
double nearBoxRight = 3.;
double nearBoxDown = -3;
double nearBoxUp = 3;
double nearAoA = 0.;
double neargap = 0.03;

double maxLayerhWake = 0.2;
double farWakeAoA = 0.;
double wakeDiffuseAngle = 6. / 180. * M_PI;
double wakedist = 0.3;
double farWakeCx = nearBoxRight + wakedist * cos(farWakeAoA);
double farWakeCy = wakedist * sin(farWakeAoA);
double farWakeHeight = 10.;
double farWakeLength = 150.;
int nFarWakey = farWakeHeight / maxLayerhWake + 0.5;
int nFarWakex = farWakeLength / maxLayerhWake / 1.8;

double xBoxLeft = -100.;
double xBoxRight = 250.;
double yBoxUp = 100.;
double yBoxDown = -100.;
int nBoxLeft = 24;
int nBoxRight = 24;
int nBoxUp = 42;
int nBoxDown = 42;

#endif
