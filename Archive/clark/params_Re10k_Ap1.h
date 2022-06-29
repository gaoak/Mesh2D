//for 3D simulation of Re10000, k2, A=0.1, mesh is reduced to extreme,
//flexible boundary thickness, with 1500 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
#ifndef FOILAOA
    #define FOILAOA 4
#endif
#ifndef FOILT
    #define FOILT 11.7
#endif
double AoA = FOILAOA/180.*M_PI;
double Thickness = FOILT*0.01;

double xInFoil = 0.01;
double hFirstLayerInFoil = 0.005;
double progressInFoil = 1.6;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.03;
double infoilRatio = 0.03;

double hFirstLayer = 0.005;
double progress = 1.6;
double rBoundaryLayer = 0.2;
double rBoundaryLayer1 = 0.2;
double rBoundaryLayer2 = 0.2;
double rBoundaryLayer3 = 0.2;
double rBoundaryLayer4 = 0.2;
double rBoundaryLayer5 = 0.3;
double rBoundaryLayer6 = 0.3;
double rBoundaryLayer7 = 0.3;
double maxLayerh = 0.05;

double chordLen= 1.;
double xmidLow1 = 0.35;
double xmidUp1 = 0.35;
double xmidLow2 = 0.006;
double growthrateLow2 = 1.5;
double xmidUp2 = 0.006;
double growthrateUp2 = 1.5;
int nLow1 = 16;
int nLow2 = 12;
int nUp1 = 16;
int nUp2 = 12;
int nFront = 8;

double wakeLen = 0.1;
double wakeyUp  = 0.06*Thickness;
double wakeDown =-0.06*Thickness;
int nWake = 4;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -20.;
double xBoxRight = 30.;
double yBoxUp = 20.;
double yBoxDown = -20.;
int nBoxLeft = 4;
int nBoxRight = 4;
int nBoxUp = 5;
int nBoxDown = 5;

int nFarWakex = 28;
int nFarWakey = 16;
double farWakeAoA = 0./180.*M_PI;
double wakeDiffuseAngle = 15./180.*M_PI;
double farWakeRight = 4.;
double farWakeUp = 0.2;
double farWakeDown = -0.6;

int curvedpts = 6;
// also need by extrude.sh
int wingn = 60 ;
int tipn  =  9 ;
double tiprefinelength = 2. ;
#endif