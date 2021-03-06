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

double xInFoil = 0.04;
double hFirstLayerInFoil = 0.01;
double progressInFoil = 1.01;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.02;

double hFirstLayer = 0.015;
double progress = 1.2;
double rBoundaryLayer = 0.7;
double maxLayerh = 0.075;

double chordLen= 1.;
double xmidLow1 = 0.2;
double xmidUp1 = 0.2;
double xmidLow2 = 0.006;
double xmidUp2 = 0.006;
int nLow1 = 23;
int nLow2 = 8;
int nUp1 = 24;
int nUp2 = 8;
int nFront = 8;
double hTrailingEdge = hFirstLayer*0.8;

double wakeLen = 1.;
double wakeyUp  = 0.133*Thickness;
double wakeDown =-0.133*Thickness;
int nWake = 20;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 40.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 8;
int nBoxRight = 8;
int nBoxUp = 8;
int nBoxDown = 8;

int nFarWakex = 36;
int nFarWakey = 30;
double farWakeAoA = 10./180.*M_PI;
double wakeDiffuseAngle = 20./180.*M_PI;
double farWakeRight = 13.;
double farWakeUp = 1.5;
double farWakeDown = -1.5;
#endif
