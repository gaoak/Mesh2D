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

double xInFoil = 0.1;
double hFirstLayerInFoil = 0.01;
double progressInFoil = 1.01;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.02;

double hFirstLayer = 0.1;
double progress = 2.;
double rBoundaryLayer = 0.2;
double maxLayerh = 0.7;

double chordLen= 1.;
double xmidLow1 = 0.2;
double xmidUp1 = 0.2;
double xmidLow2 = 0.006;
double xmidUp2  = 0.006;
int nLow1 = 2;
int nLow2 = 1;
int nUp1 = 2;
int nUp2 = 1;
int nFront = 1;
double hTrailingEdge = hFirstLayer*0.8;

double wakeLen = 1.;
double wakeyUp = 0.5*Thickness;
double wakeDown =-0.5*Thickness;
int nWake = 1;
double nearWakeDiffuseAngle = 10./180.*M_PI;

double xBoxLeft = -2.;
double xBoxRight = 4.;
double yBoxUp = 3.;
double yBoxDown = -3.;
int nBoxLeft = 2;
int nBoxRight = 2;
int nBoxUp = 1;
int nBoxDown = 1;

int nFarWakex = 36;
int nFarWakey = 30;
double farWakeAoA = 10./180.*M_PI;
double wakeDiffuseAngle = 20./180.*M_PI;
double farWakeRight = 13.;
double farWakeUp = 1.5;
double farWakeDown = -1.5;
#endif
