// for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
double AoA = 0. / 180. * M_PI;
double Thickness = 0.1;
// outside
double hFirstLayer = 0.003;
double progress = 1.1;
double upperBL0 = 0.05;
double upperBL1 = 0.05;
double upperBL2 = 0.05;
double upperBL3 = 0.05;
double upperBL4 = 0.05;
double upperBL5 = 0.05;
double lowerBL0 = 0.05;
double lowerBL1 = 0.05;
double lowerBL2 = 0.05;
double lowerBL3 = 0.05;
double lowerBL4 = 0.05;
double lowerBL5 = 0.05;
double maxLayerh = 0.03;

double upperx1 = 0.1;
double upperx2 = 0.4;
double upperx3 = 0.7;
double lowerx1 = 0.1;
double lowerx2 = 0.4;
double lowerx3 = 0.7;

// number starts from leading to trailing
int nUp0 =
    std::max(int(0.5 * upperBL0 * M_PI / maxLayerh + 0.5),
             std::max(12, int(0.5 + 0.25 * M_PI * Thickness / hFirstLayer)));
int nUp1 = 8;
int nUp2 = 10;
int nUp3 = 10;
int nUp4 = 14;
int nUp5 =
    std::max(int(0.5 * upperBL0 * M_PI / maxLayerh + 0.5),
             std::max(10, int(0.5 + 0.25 * M_PI * Thickness / hFirstLayer)));
int nLow0 =
    std::max(int(0.5 * lowerBL0 * M_PI / maxLayerh + 0.5),
             std::max(10, int(0.5 + 0.25 * M_PI * Thickness / hFirstLayer)));
int nLow1 = 8;
int nLow2 = 10;
int nLow3 = 10;
int nLow4 = 14;
int nLow5 =
    std::max(int(0.5 * lowerBL5 * M_PI / maxLayerh + 0.5),
             std::max(10, int(0.5 + 0.25 * M_PI * Thickness / hFirstLayer)));

double nearBoxLeft = -0.3;
double nearBoxRight = 1.5;
double nearBoxDown = -0.3;
double nearBoxUp = 0.3;
double nearAoA = 0.;

int nFarWakex = 160;
int nFarWakey = 30;
double farWakeAoA = AoA;
double wakeDiffuseAngle = 8. / 180. * M_PI;
double wakedist = 0.1;
double farWakeCx = nearBoxRight + wakedist * cos(AoA);
double farWakeCy = wakedist * sin(AoA);
double farWakeHeight = 1.;
double farWakeLength = 10.;

double xBoxLeft = -40.;
double xBoxRight = 60.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 10;
int nBoxRight = 10;
int nBoxUp = 10;
int nBoxDown = 10;

int curvedpts = 6;

#endif
