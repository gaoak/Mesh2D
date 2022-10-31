//for a schematic show of partial slip boundary condition,
//not for accurate simulation
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20

// segments infomation
double theta32 = -M_PI/4.;
double theta6 = M_PI/4.;
double theta5 = 0.75*M_PI;
double theta4 = 1.25*M_PI;
double theta33 = 1.75*M_PI;

double hFirstLayer = 0.005;
double progress = 1.2;
double rBoundaryLayer2 = 1.;
double rBoundaryLayer3 = 1.;
double rBoundaryLayer4 = 1.;
double rBoundaryLayer5 = 1.;
double maxLayerh = 0.05;

int Ncyl2 = 60;
int Ncyl3 = 24;
int Ncyl4 = 20;
int Ncyl5 = 60;

// geometry infomation
double chordLen= 1.;

double xBoxLeft = -5.;
double xBoxRight = 10.;
double yBoxUp = 5.;
double yBoxDown = -5.;
int nBoxLeft = 40;
int nBoxRight = 40;
int nBoxUp = 40;
int nBoxDown = 40;

int nFarWakex = 10;
int nFarWakey = 60;
double farWakeAoA = 0./180.*M_PI;
double wakeDiffuseAngle = 5./180.*M_PI;
double farWakeRight = 2.1;
double farWakeUp = 1.5;
double farWakeDown = -1.5;

int curvedpts = 6;
#endif
