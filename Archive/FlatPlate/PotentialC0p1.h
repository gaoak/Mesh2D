// for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
#include "CAD2D/BLAirfoil.h"
#include "CAD2D/BLEllipse.h"
#include "CAD2D/BLFlatPlate.h"
#include <map>
#include <string>

BLMeshModuleShPtr BLModel;
double ChordLen = 0.1;
void DefineBLParams(std::map<std::string, double> &p,
                    std::map<std::string, int> &q) {
  double AoA = 0. / 180. * M_PI;
  p["AoA"] = AoA;
  double Thickness = 0.05;
  p["Thickness"] = Thickness;
  p["ChordLen"] = ChordLen;
  // outside
  double hFirstLayer = 0.003;
  p["hFirstLayer"] = hFirstLayer;
  double progress = 1.1;
  p["progress"] = progress;
  double upperBL0 = 0.05;
  p["upperBL0"] = upperBL0;
  double upperBL1 = 0.05;
  p["upperBL1"] = upperBL1;
  double upperBL2 = 0.05;
  p["upperBL2"] = upperBL2;
  double upperBL3 = 0.05;
  p["upperBL3"] = upperBL3;
  double upperBL4 = 0.05;
  p["upperBL4"] = upperBL4;
  double upperBL5 = 0.05;
  p["upperBL5"] = upperBL5;
  double lowerBL0 = 0.05;
  p["lowerBL0"] = lowerBL0;
  double lowerBL1 = 0.05;
  p["lowerBL1"] = lowerBL1;
  double lowerBL2 = 0.05;
  p["lowerBL2"] = lowerBL2;
  double lowerBL3 = 0.05;
  p["lowerBL3"] = lowerBL3;
  double lowerBL4 = 0.05;
  p["lowerBL4"] = lowerBL4;
  double lowerBL5 = 0.05;
  p["lowerBL5"] = lowerBL5;
  double maxLayerh = 0.01;
  p["maxLayerh"] = maxLayerh;

  double upperx1 = ChordLen * 0.375;
  p["upperx1"] = upperx1;
  double upperx2 = ChordLen * 0.5;
  p["upperx2"] = upperx2;
  double upperx3 = ChordLen * 0.625;
  p["upperx3"] = upperx3;
  double lowerx1 = ChordLen * 0.375;
  p["lowerx1"] = lowerx1;
  double lowerx2 = ChordLen * 0.5;
  p["lowerx2"] = lowerx2;
  double lowerx3 = ChordLen * 0.625;
  p["lowerx3"] = lowerx3;

  // number starts from leading to trailing
  int nUp0 =
      std::max(int(0.5 * upperBL0 * M_PI / maxLayerh + 0.5),
               std::max(12, int(0.5 + 0.25 * M_PI * Thickness / hFirstLayer)));
  q["nUp0"] = nUp0;
  int nUp1 = (upperx1) / maxLayerh + 4;
  q["nUp1"] = nUp1;
  int nUp2 = (upperx2 - upperx1) / maxLayerh + 1;
  q["nUp2"] = nUp2;
  int nUp3 = (upperx3 - upperx2) / maxLayerh + 1;
  q["nUp3"] = nUp3;
  int nUp4 = (ChordLen - upperx3) / maxLayerh + 4;
  q["nUp4"] = nUp4;
  int nUp5 =
      std::max(int(0.5 * upperBL0 * M_PI / maxLayerh + 0.5),
               std::max(10, int(0.5 + 0.25 * M_PI * Thickness / hFirstLayer)));
  q["nUp5"] = nUp5;
  int nLow0 =
      std::max(int(0.5 * lowerBL0 * M_PI / maxLayerh + 0.5),
               std::max(10, int(0.5 + 0.25 * M_PI * Thickness / hFirstLayer)));
  q["nLow0"] = nLow0;
  int nLow1 = (lowerx1) / maxLayerh + 4;
  q["nLow1"] = nLow1;
  int nLow2 = (lowerx2 - lowerx1) / maxLayerh + 0.5;
  q["nLow2"] = nLow2;
  int nLow3 = (lowerx3 - lowerx2) / maxLayerh + 0.5;
  q["nLow3"] = nLow3;
  int nLow4 = (ChordLen - lowerx3) / maxLayerh + 4;
  q["nLow4"] = nLow4;
  int nLow5 =
      std::max(int(0.5 * lowerBL5 * M_PI / maxLayerh + 0.5),
               std::max(10, int(0.5 + 0.25 * M_PI * Thickness / hFirstLayer)));
  q["nLow5"] = nLow5;
  int curvedpts = 6;
  q["curvedpts"] = curvedpts;
  BLModel = std::make_shared<BLFlatPlate>(p, q);
  BLModel->Initialise();
}

double nearmaxLayerh = 0.2;
double nearBoxLeft = -6.;
double nearBoxRight = ChordLen + 6.;
double nearBoxDown = -6.;
double nearBoxUp = 6.;
double nearAoA = 0.;
double neargap = 1.;

double maxLayerhWake = 0.05;
double farWakeAoA = 0.;
double wakeDiffuseAngle = 12. / 180. * M_PI;
double wakedist = 0.1;
double farWakeCx = nearBoxRight + wakedist * cos(farWakeAoA);
double farWakeCy = wakedist * sin(farWakeAoA);
double farWakeHeight = 2.;
double farWakeLength = 10.;
int nFarWakey = farWakeHeight / maxLayerhWake + 0.5;
int nFarWakex = farWakeLength / maxLayerhWake / 2;

double xBoxLeft = -40.;
double xBoxRight = 60.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 10;
int nBoxRight = 10;
int nBoxUp = 10;
int nBoxDown = 10;

#endif
