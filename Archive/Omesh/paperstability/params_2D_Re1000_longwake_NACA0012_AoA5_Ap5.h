//for 2D simulation of Re <= 1000, k2, A=0.5, mesh is reduced to extreme,
//flexible boundary thickness, with 8500 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
static double FOILAOA = 5.;// AoA
static double FOILM   = 0.; // NACA 0
static double FOILP   = 0.; // 0
static double FOILT   = 12.;// 12

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
double progress = 1.2;
double rBoundaryLayer  = 0.8;
double rBoundaryLayer1 = 0.8;
double rBoundaryLayer2 = 0.8;
double rBoundaryLayer3 = 0.8;
double rBoundaryLayer4 = 0.8;
double rBoundaryLayer5 = 0.8;
double rBoundaryLayer6 = 0.8;
double rBoundaryLayer7 = 0.8;
double maxLayerh = 0.05;

double chordLen= 1.;
double xmidLow1 = 0.2;
double xmidUp1 = 0.2;
double xmidLow2 = 0.006;
double growthrateLow2 = 1.3;
double xmidUp2 = 0.006;
double growthrateUp2 = 1.3;
int nLow1 = 28;
int nLow2 = 13;
int nUp1 = 28;
int nUp2 = 13;
int nFront = 12;

double wakeLen = 0.08;
double wakeyUp  = 0.06*Thickness;
double wakeDown =-0.06*Thickness;
int nWake = 4;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 60.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 10;
int nBoxRight = 10;
int nBoxUp = 10;
int nBoxDown = 10;

int nFarWakex = 100;
int nFarWakey = 50;
double farWakeAoA = 0./180.*M_PI;
double wakeDiffuseAngle = 14./180.*M_PI;
double farWakeRight = 20.;
double farWakeUp = 1.5;
double farWakeDown = -1.5;

int curvedpts = 6;
#endif
