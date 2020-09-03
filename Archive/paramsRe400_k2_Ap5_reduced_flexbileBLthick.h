//for 3D simulation of Re400, k2, A=0.5, mesh is reduced to extreme,
//flexible boundary thickness, with 1500 elements
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
double hFirstLayerInFoil = 0.018;
double progressInFoil = 1.4;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.04;
double infoilRatio = 0.02;

double hFirstLayer = 0.016;
double progress = 1.6;
double rBoundaryLayer = 0.2;
double rBoundaryLayer1 = 0.2;
double rBoundaryLayer2 = 0.2;
double rBoundaryLayer3 = 0.2;
double rBoundaryLayer4 = 0.2;
double rBoundaryLayer5 = 0.2;
double rBoundaryLayer6 = 0.8;
double rBoundaryLayer7 = 0.8;
double maxLayerh = 0.1;

double chordLen= 1.;
double xmidLow1 = 0.5;
double xmidUp1 = 0.5;
double xmidLow2 = 0.006;
double xmidUp2 = 0.006;
int nLow1 = 7;
int nLow2 = 9;
int nUp1 = 8;
int nUp2 = 10;
int nFront = 5;
double hTrailingEdge = hFirstLayer*2.;

double wakeLen = 0.1;
double wakeyUp  = 0.08*Thickness;
double wakeDown =-0.08*Thickness;
int nWake = 2;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 40.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 2;
int nBoxRight = 2;
int nBoxUp = 2;
int nBoxDown = 2;

int nFarWakex = 21;
int nFarWakey = 18;
double farWakeAoA = 13./180.*M_PI;
double wakeDiffuseAngle = 18./180.*M_PI;
double farWakeRight = 6.;
double farWakeUp = 0.8;
double farWakeDown = -1.;
#endif
