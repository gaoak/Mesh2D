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
  double AoA = 15. / 180. * M_PI;
  p["AoA"] = AoA;
  double Thickness = 0.125;
  p["Thickness"] = Thickness;
  p["ChordLen"] = ChordLen;
  double Tx0 = 0.5*ChordLen;
  p["Tx0"] = Tx0;
  // outside
  double hFirstLayer = 0.001;
  p["hFirstLayer"] = hFirstLayer;
  double progress = 1.5;
  p["progress"] = progress;
  double maxLayerh = 0.1;
  p["maxLayerh"] = maxLayerh;
  double wallBLThickness0 = 0.05;
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
  int nLE = 30;
  q["nLE"] = nLE;
  int nUp = 25;
  q["nUp"] = nUp;
  int nTE = 30;
  q["nTE"] = nTE;
  int nLow = 25;
  q["nLow"] = nLow;
  int curvedpts = 5;
  q["curvedpts"] = curvedpts;
  BLModel = std::make_shared<BLEllipse>(p, q);
  BLModel->Initialise();
}

double nearmaxLayerh = 0.075;
double nearBoxLeft = -1;
double nearBoxRight = 2;
double nearBoxDown = -1.5;
double nearBoxUp = 1.5;
double nearAoA = 0.;
double neargap = nearmaxLayerh;

double farmaxLayerh = 0.4;
double farBoxLeft = -5.;
double farBoxRight = 5.;
double farBoxDown = -20.;
double farBoxUp = 20.;
double farAoA = 0.;
double fargap = farmaxLayerh;

double maxLayerhWake = 0.4;
double farWakeAoA = 0.;
double wakeDiffuseAngle = 16. / 180. * M_PI;
double wakegap = farmaxLayerh;
double farWakeCx = farBoxRight + wakegap * cos(farWakeAoA);
double farWakeCy = 0.5 * (farBoxDown + farBoxUp) +
                   wakegap * sin(farWakeAoA);
double farWakeHeight = 8.;
double farWakeLength = 20.;
int nFarWakey = farWakeHeight / maxLayerhWake + 0.5;
int nFarWakex = farWakeLength / maxLayerhWake / 1.5;

double xBoxLeft = -60.;
double xBoxRight = 60.;
double yBoxUp = 60.;
double yBoxDown = -60.;
int nBoxLeft = 20;
int nBoxRight = 20;
int nBoxUp = 20;
int nBoxDown = 20;

#endif
