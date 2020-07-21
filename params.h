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
double infoilRatio = 0.03;

double hFirstLayer = 0.003;
double progress = 1.6;
double rBoundaryLayer = 0.7;
double maxLayerh = 0.08;

double chordLen= 1.;
double xmidLow1 = 0.2;
double xmidUp1 = 0.2;
double xmidLow2 = 0.006;
double xmidUp2 = 0.006;
int nLow1 = 22;
int nLow2 = 8;
int nUp1 = 22;
int nUp2 = 8;
int nFront = 9;
double hTrailingEdge = hFirstLayer*3.;

double wakeLen = 1.;
double wakeyUp  = 0.133*Thickness;
double wakeDown =-0.133*Thickness;
int nWake = 20;
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
int nFarWakey = 28;
double farWakeAoA = 5./180.*M_PI;
double wakeDiffuseAngle = 10./180.*M_PI;
double farWakeRight = 6.;
double farWakeUp = 0.4;
double farWakeDown = -1.;
#endif
