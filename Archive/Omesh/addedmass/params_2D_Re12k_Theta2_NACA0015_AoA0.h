//for 2D simulation of Re <= 1000, k2, A=0.5, mesh is refined,
//flexible boundary thickness, with 6377 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
static double FOILAOA = 0.;// AoA
static double FOILM   = 0.; // NACA 0
static double FOILP   = 0.; // 0
static double FOILT   = 15.;// 12

double AoA = FOILAOA/180.*M_PI;
double chamber = FOILM*0.01;
double chamberp = FOILP*0.1;
double Thickness = FOILT*0.01;

double xInFoil = 0.002;
double hFirstLayerInFoil = 0.01;
double progressInFoil = 1.6;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.07;
double infoilRatio = 0.03;

double hFirstLayer = 0.002;
double progress = 1.1;
double rBoundaryLayer  = 0.3;
double rBoundaryLayer1 = 0.3;
double rBoundaryLayer2 = 0.3;
double rBoundaryLayer3 = 0.3;
double rBoundaryLayer4 = 0.3;
double rBoundaryLayer5 = 0.3;
double rBoundaryLayer6 = 0.3;
double rBoundaryLayer7 = 0.3;
double maxLayerh = 0.05;

double chordLen= 1.;
double xmidLow1 = 0.2;
double xmidUp1 = 0.2;
double xmidLow2 = 0.006;
double growthrateLow2 = 1.3;
double xmidUp2 = 0.006;
double growthrateUp2 = 1.3;
int nLow1 = 30;
int nLow2 = 16;
int nUp1 = 30;
int nUp2 = 16;
int nFront = 16;

double wakeLen = 0.08;
double wakeyUp  = 0.02*Thickness;
double wakeDown =-0.02*Thickness;
int nWake = 6;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 60.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 10;
int nBoxRight = 10;
int nBoxUp = 10;
int nBoxDown = 10;

int nFarWakex = 90;
int nFarWakey = 30;
double farWakeAoA = 0./180.*M_PI;
double wakeDiffuseAngle = 9./180.*M_PI;
double farWakeRight = 14.;
double farWakeUp = 0.5;
double farWakeDown = -0.5;

int curvedpts = 6;
#endif