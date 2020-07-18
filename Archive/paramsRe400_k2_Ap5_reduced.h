#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
#ifndef FOILAOA
    #define FOILAOA 15
#endif
#ifndef FOILM
    #define FOILM 0
#endif
#ifndef FOILP
    #define FOILP 0
#endif
#ifndef FOILT
    #define FOILT 12
#endif
double AoA = FOILAOA/180.*M_PI;
double chamber = FOILM*0.01;
double chamberp = FOILP*0.1;
double Thickness = FOILT*0.01;

double xInFoil = 0.02;
double hFirstLayerInFoil = 0.01;
double progressInFoil = 1.1;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.02;
double infoilRatio = 0.03;

double hFirstLayer = 0.015;
double progress = 1.6;
double rBoundaryLayer = 1.;
double maxLayerh = 0.12;

double chordLen= 1.;
double xmidLow1 = 0.2;
double xmidUp1 = 0.2;
double xmidLow2 = 0.006;
double xmidUp2 = 0.006;
int nLow1 = 14;
int nLow2 = 6;
int nUp1 = 14;
int nUp2 = 6;
int nFront = 7;
double hTrailingEdge = hFirstLayer*2.;

double wakeLen = 1.;
double wakeyUp  = 0.2*Thickness;
double wakeDown =-0.2*Thickness;
int nWake = 10;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 40.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 2;
int nBoxRight = 2;
int nBoxUp = 2;
int nBoxDown = 2;

int nFarWakex = 15;
int nFarWakey = 18;
double farWakeAoA = 10./180.*M_PI;
double wakeDiffuseAngle = 20./180.*M_PI;
double farWakeRight = 6.;
double farWakeUp = 1.0;
double farWakeDown = -2.0;
#endif
