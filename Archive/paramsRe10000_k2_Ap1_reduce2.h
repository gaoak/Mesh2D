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
double progressInFoil = 1.6;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.03;
double infoilRatio = 0.03;

double hFirstLayer = 0.003;
double progress = 1.8;
double rBoundaryLayer = 0.2;
double maxLayerh = 0.08;

double chordLen= 1.;
double xmidLow1 = 0.35;
double xmidUp1 = 0.35;
double xmidLow2 = 0.006;
double xmidUp2 = 0.006;
int nLow1 = 14;
int nLow2 = 7;
int nUp1 = 17;
int nUp2 = 10;
int nFront = 7;
double hTrailingEdge = hFirstLayer*3.;

double wakeLen = 0.1;
double wakeyUp  = 0.03*Thickness;
double wakeDown =-0.03*Thickness;
int nWake = 4;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 40.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 2;
int nBoxRight = 2;
int nBoxUp = 2;
int nBoxDown = 2;

int nFarWakex = 14;
int nFarWakey = 10;
double farWakeAoA = 0./180.*M_PI;
double wakeDiffuseAngle = 15./180.*M_PI;
double farWakeRight = 3.;
double farWakeUp = 0.15;
double farWakeDown = -0.55;
#endif
