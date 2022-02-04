//for 2D simulation of Re <= 1000, k2, A=0.5, mesh is reduced to extreme,
//flexible boundary thickness, with 8500 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
static double FOILAOA = 2.;// AoA
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

double hFirstLayer = 0.001;
double progress = 1.2;
double rBoundaryLayer  = 0.3;
double rBoundaryLayer1 = 0.3;
double rBoundaryLayer2 = 0.3;
double rBoundaryLayer3 = 0.3;
double rBoundaryLayer4 = 0.3;
double rBoundaryLayer5 = 0.3;
double rBoundaryLayer6 = 0.3;
double rBoundaryLayer7 = 0.3;
double maxLayerh = 0.015;

double chordLen= 1.;
double xmidLow1 = 0.2;
double xmidUp1 = 0.2;
double xmidLow2 = 0.006;
double growthrateLow2 = 1.3;
double xmidUp2 = 0.006;
double growthrateUp2 = 1.3;
int nLow1 = 64;
int nLow2 = 26;
int nUp1 = 65;
int nUp2 = 26;
int nFront = 20;

double wakeLen = 0.08;
double wakeyUp  = 0.02*Thickness;
double wakeDown =-0.02*Thickness;
int nWake = 6;
double nearWakeDiffuseAngle = 0./180.*M_PI;

double xBoxLeft = -40.;
double xBoxRight = 60.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 56;
int nBoxRight = 56;
int nBoxUp = 70;
int nBoxDown = 70;

int nFarWakex = 50;
int nFarWakey = 50;
double farWakeAoA = 1./180.*M_PI;
double wakeDiffuseAngle = 8./180.*M_PI;
double farWakeRight = 2.5;
double farWakeUp = 0.5;
double farWakeDown = -0.5;

int curvedpts = 6;
#endif
