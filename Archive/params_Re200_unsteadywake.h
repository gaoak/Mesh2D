//for 2D simulation of Re <= 1000, k2, A=0.5, mesh is reduced to extreme,
//flexible boundary thickness, with 8500 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20

// segments infomation
double theta32 = -M_PI/4.;
double theta6 = M_PI/4.;
double theta5 = 0.75*M_PI;
double theta4 = 1.25*M_PI;
double theta33 = 1.75*M_PI;

double hFirstLayer = 0.03;
double progress = 1.2;
double rBoundaryLayer2 = 1.;
double rBoundaryLayer3 = 1.;
double rBoundaryLayer4 = 1.;
double rBoundaryLayer5 = 1.;
double maxLayerh = 0.1;

int Ncyl2 = 24;
int Ncyl3 = 24;
int Ncyl4 = 20;
int Ncyl5 = 24;

// geometry infomation
double chordLen= 1.;

double xBoxLeft = -30.;
double xBoxRight = 60.;
double yBoxUp = 30.;
double yBoxDown = -30.;
int nBoxLeft = 16;
int nBoxRight = 16;
int nBoxUp = 24;
int nBoxDown = 24;

int nFarWakex = 66;
int nFarWakey = 40;
double farWakeAoA = 0./180.*M_PI;
double wakeDiffuseAngle = 5./180.*M_PI;
double farWakeRight = 20.;
double farWakeUp = 3.;
double farWakeDown = -3.;

int curvedpts = 6;
#endif
