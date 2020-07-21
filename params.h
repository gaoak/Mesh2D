//3D mesh for Re10k, k3, A=0.1, coarese mesh, 2000 elements
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
double hFirstLayerInFoil = 0.003;
double progressInFoil = 1.2;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.02;
double infoilRatio = 0.05;

double hFirstLayer = 0.003;
double progress = 1.6;
double rBoundaryLayer = 0.35;
double maxLayerh = 0.05;

double chordLen= 1.;
double xmidLow1 = 0.35;
double xmidUp1 = 0.35;
double xmidLow2 = 0.006;
double xmidUp2 = 0.006;
int nLow1 = 13;
int nLow2 = 11;
int nUp1 = 21;
int nUp2 = 13;
int nFront = 7;
double hTrailingEdge = hFirstLayer*3.;

double wakeLen = 0.5;
double wakeyUp  = 0.12*Thickness;
double wakeDown =-0.12*Thickness;
int nWake = 14;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 40.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 2;
int nBoxRight = 2;
int nBoxUp = 2;
int nBoxDown = 2;

int nFarWakex = 20;
int nFarWakey = 20;
double farWakeAoA = 0./180.*M_PI;
double wakeDiffuseAngle = 12./180.*M_PI;
double farWakeRight = 4.;
double farWakeUp = 0.4;
double farWakeDown = -1.;
#endif
