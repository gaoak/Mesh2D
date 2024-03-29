//for 3D simulation of Re400, k2, A=0.5, mesh is reduced to extreme,
//flexible boundary thickness, with 1500 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20

#define FOILAOA 15
#define FOILM 0
#define FOILP 0
#define FOILT 12

double AoA = FOILAOA/180.*M_PI;
double chamber = FOILM*0.01;
double chamberp = FOILP*0.1;
double Thickness = FOILT*0.01;

double xInFoil = 0.01;
double hFirstLayerInFoil = 0.005;
double progressInFoil = 1.6;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.04;
double infoilRatio = 0.03;

double hFirstLayer = 0.005;
double progress = 1.6;
double rBoundaryLayer = 0.2;
double rBoundaryLayer1 = 0.2;
double rBoundaryLayer2 = 0.2;
double rBoundaryLayer3 = 0.2;
double rBoundaryLayer4 = 0.2;
double rBoundaryLayer5 = 0.4;
double rBoundaryLayer6 = 0.8;
double rBoundaryLayer7 = 0.8;
double maxLayerh = 0.1;

double chordLen= 1.;
double xmidLow1 = 0.5;
double xmidUp1 = 0.5;
double xmidLow2 = 0.006;
double growthrateLow2 = 1.5;
double xmidUp2 = 0.006;
double growthrateUp2 = 1.5;
int nLow1 = 9;
int nLow2 = 11;
int nUp1 = 9;
int nUp2 = 11;
int nFront = 6;

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

int nFarWakex = 21;
int nFarWakey = 18;
double farWakeAoA = 13./180.*M_PI;
double wakeDiffuseAngle = 18./180.*M_PI;
double farWakeRight = 6.;
double farWakeUp = 0.8;
double farWakeDown = -1.;

int curvedpts = 6;
// also need by extrude.sh
int wingn = 40 ;
int tipn  =  9 ;
#endif