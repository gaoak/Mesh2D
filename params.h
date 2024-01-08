// for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
double AoA = 15. / 180. * M_PI;
double Thickness = 0.12;
// in foil
double hFirstLayerInFoil = 0.01;
double progressInFoil = 1.6;
double rBoundaryLayerInFoil = Thickness * 0.3;
double maxLayerhInFoil = 0.04;
// outside
double hFirstLayer = 0.01;
double progress = 1.2;
double upperBL0 = 0.8;
double upperBL1 = 0.8;
double upperBL2 = 0.8;
double upperBL3 = 0.4;
double upperBL4 = 0.3;
double upperBL5 = 0.4;
double lowerBL0 = 0.8;
double lowerBL1 = 0.8;
double lowerBL2 = 0.8;
double lowerBL3 = 0.4;
double lowerBL4 = 0.3;
double lowerBL5 = 0.4;
double maxLayerh = 0.1;

double upperx1 = 0.1;
double upperx2 = 0.2;
double upperx3 = 0.6;
double lowerx1 = 0.1;
double lowerx2 = 0.2;
double lowerx3 = 0.6;

// number starts from leading to trailing
int nUp0 = 28;
int nUp1 = 6;
int nUp2 = 6;
int nUp3 = 6;
int nUp4 = 6;
int nUp5 = 6;
int nLow0 = 28;
int nLow1 = 6;
int nLow2 = 28;
int nLow3 = 6;
int nLow4 = 28;
int nLow5 = 6;

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
double farWakeAoA = 0. / 180. * M_PI;
double wakeDiffuseAngle = 15. / 180. * M_PI;
double farWakeCx = 2.;
double farWakeCy = 0.;
double farWakeHeight = 2.;
double farWakeLength = 10.;

int curvedpts = 6;

#endif
