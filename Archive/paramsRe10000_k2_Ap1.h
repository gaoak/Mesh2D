//2D mesh for Re10k, k2, k3, A=0.1, fine mesh9000 elements
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

double xInFoil = 0.01;
double hFirstLayerInFoil = 0.002;
double progressInFoil = 1.2;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.02;
double infoilRatio = 0.03;

double hFirstLayer = 0.002;
double progress = 1.2;
double rBoundaryLayer = 0.7;
double maxLayerh = 0.05;

double chordLen= 1.;
double xmidLow1 = 0.2;
double xmidUp1 = 0.2;
double xmidLow2 = 0.006;
double xmidUp2 = 0.006;
int nLow1 = 28;
int nLow2 = 10;
int nUp1 = 34;
int nUp2 = 10;
int nFront = 12;
double hTrailingEdge = hFirstLayer*3.;

double wakeLen = 1.;
double wakeyUp  = 0.133*Thickness;
double wakeDown =-0.133*Thickness;
int nWake = 32;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 40.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 8;
int nBoxRight = 8;
int nBoxUp = 8;
int nBoxDown = 8;

int nFarWakex = 80;
int nFarWakey = 28;
double farWakeAoA = 5./180.*M_PI;
double wakeDiffuseAngle = 10./180.*M_PI;
double farWakeRight = 13.;
double farWakeUp = 0.4;
double farWakeDown = -1.;
#endif
