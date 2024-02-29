// for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
#include "CAD2D/BLAirfoil.h"
#include "CAD2D/BLEllipse.h"
#include "CAD2D/BLFlatPlate.h"
#include <map>
#include <string>

BLMeshModuleShPtr BLModel;
void DefineBLParams(std::map<std::string, double> &p,
                    std::map<std::string, int> &q) {
  double AoA = 0. / 180. * M_PI;
  p["AoA"] = AoA;
  double Thickness = 0.05;
  p["Thickness"] = Thickness;
  double ChordLen = 0.6;
  p["ChordLen"] = ChordLen;
  double Tx0 = 0.5 * ChordLen;
  p["Tx0"] = Tx0;
  // outside
  double hFirstLayer = 0.005;
  p["hFirstLayer"] = hFirstLayer;
  double progress = 1.1;
  p["progress"] = progress;
  double maxLayerh = 0.1;
  p["maxLayerh"] = maxLayerh;
  double wallBLThickness0 = 0.05;
  p["wallBLThickness0"] = wallBLThickness0;

  // number starts from leading to trailing
  int Ncylinder = 160;
  q["Ncylinder"] = Ncylinder;
  int curvedpts = 6;
  q["curvedpts"] = curvedpts;
  BLModel = std::make_shared<BLEllipse>(p, q);
  BLModel->Initialise();
}

double AoA = 0. / 180. * M_PI;
double maxLayerh = 0.03;
double nearBoxLeft = -0.3;
double nearBoxRight = 1.5;
double nearBoxDown = -0.3;
double nearBoxUp = 0.3;
double nearAoA = 0.;

double farWakeAoA = AoA;
double wakeDiffuseAngle = 8. / 180. * M_PI;
double wakedist = 0.1;
double farWakeCx = nearBoxRight + wakedist * cos(AoA);
double farWakeCy = wakedist * sin(AoA);
double farWakeHeight = 1.;
double farWakeLength = 10.;
int nFarWakey = farWakeHeight / maxLayerh + 0.5;
int nFarWakex = farWakeLength / maxLayerh / 2;

double xBoxLeft = -40.;
double xBoxRight = 60.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 10;
int nBoxRight = 10;
int nBoxUp = 10;
int nBoxDown = 10;

#endif
