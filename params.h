// for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
double AoA = 15. / 180. * M_PI;
double Thickness = 0.05;
// in foil
double hFirstLayerInFoil = 0.01;
double progressInFoil = 1.6;
double rBoundaryLayerInFoil = Thickness * 0.3;
double maxLayerhInFoil = 0.04;
// outside
double hFirstLayer = 0.004;
double progress = 1.1;
double upperBL0 = 0.3;
double upperBL1 = 0.3;
double upperBL2 = 0.3;
double upperBL3 = 0.3;
double upperBL4 = 0.3;
double upperBL5 = 0.3;
double lowerBL0 = 0.3;
double lowerBL1 = 0.2;
double lowerBL2 = 0.2;
double lowerBL3 = 0.2;
double lowerBL4 = 0.2;
double lowerBL5 = 0.3;
double maxLayerh = 0.03;

double upperx1 = 0.1;
double upperx2 = 0.4;
double upperx3 = 0.7;
double lowerx1 = 0.1;
double lowerx2 = 0.4;
double lowerx3 = 0.7;

// number starts from leading to trailing
int nUp0 = 24;
int nUp1 = 10;
int nUp2 = 10;
int nUp3 = 10;
int nUp4 = 10;
int nUp5 = 24;
int nLow0 = 24;
int nLow1 = 10;
int nLow2 = 10;
int nLow3 = 10;
int nLow4 = 10;
int nLow5 = 24;

double xBoxLeft = -40.;
double xBoxRight = 60.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 10;
int nBoxRight = 10;
int nBoxUp = 10;
int nBoxDown = 10;

int nFarWakex = 100;
int nFarWakey = 50;
double farWakeAoA = AoA;
double wakeDiffuseAngle = 8. / 180. * M_PI;
double wakedist = 1.;
double farWakeCx = 0.5 + wakedist * cos(AoA);
double farWakeCy = wakedist * sin(AoA);
double farWakeHeight = 2.;
double farWakeLength = 10.;

int curvedpts = 6;

#endif
