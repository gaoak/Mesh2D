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
double hFirstLayerInFoil = 0.004;
double progressInFoil = 1.3;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.04;
double infoilRatio = 0.03;

double hFirstLayer = 0.004;
double progress = 1.3;
double rBoundaryLayer = 0.2;
double rBoundaryLayer1 = 0.2;
double rBoundaryLayer2 = 0.2;
double rBoundaryLayer3 = 0.2;
double rBoundaryLayer4 = 0.2;
double rBoundaryLayer5 = 0.4;
double rBoundaryLayer6 = 0.6;
double rBoundaryLayer7 = 0.6;
double maxLayerh = 0.05;

double chordLen= 1.;
double xmidLow1 = 0.4;
double xmidUp1 = 0.4;
double xmidLow2 = 0.006;
double growthrateLow2 = 1.5;
double xmidUp2 = 0.006;
double growthrateUp2 = 1.5;
int nLow1 = 16;
int nLow2 = 12;
int nUp1 = 16;
int nUp2 = 15;
int nFront = 7;

double wakeLen = 0.1;
double wakeyUp  = 0.06*Thickness;
double wakeDown =-0.06*Thickness;
int nWake = 4;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -30.;
double xBoxRight = 40.;
double yBoxUp = 30.;
double yBoxDown = -30.;
int nBoxLeft = 8;
int nBoxRight = 8;
int nBoxUp = 10;
int nBoxDown = 10;

int nFarWakex = 50;
int nFarWakey = 40;
double farWakeAoA = 0./180.*M_PI;
double wakeDiffuseAngle = 8./180.*M_PI;
double farWakeRight = 10.;
double farWakeUp = 1.5;
double farWakeDown = -1.5;

int curvedpts = 6;

// also need by extrude.sh
int wingn = 72 ;
int tipn  =  9 ;
double tiprefinelength = 2.5 ;
#endif
