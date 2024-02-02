// for 2D simulation of Re <= 1000, k2, A=0.5, mesh is refined,
// flexible boundary thickness, with 6377 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
static double FOILAOA = 0.; // AoA
static double FOILM = 0.;   // NACA 0
static double FOILP = 0.;   // 0
static double FOILT = 12.;  // 12

// segments infomation
double theta32 = -M_PI / 4.;
double theta6 = M_PI / 4.;
double theta5 = 0.75 * M_PI;
double theta4 = 1.25 * M_PI;
double theta33 = 1.75 * M_PI;

double xInFoil = 0.01;
double hFirstLayerInFoil = 0.01;
double progressInFoil = 1.6;
double rBoundaryLayerInFoil = 1.;
double maxLayerhInFoil = 0.07;
double infoilRatio = 0.03;

double hFirstLayer = 0.01;
double progress = 1.3;
double rBoundaryLayer = 0.2;
double rBoundaryLayer2 = 0.2;
double rBoundaryLayer3 = 0.2;
double rBoundaryLayer4 = 0.2;
double rBoundaryLayer5 = 0.2;
double maxLayerh = 0.05;

int Ncyl2 = 24;
int Ncyl3 = 24;
int Ncyl4 = 20;
int Ncyl5 = 24;

double chordLen = 1.;

double xBoxLeft = -40.;
double xBoxRight = 40.;
double yBoxUp = 40.;
double yBoxDown = -40.;
int nBoxLeft = 10;
int nBoxRight = 10;
int nBoxUp = 10;
int nBoxDown = 10;

int nSquarex = 40;
int nSquarey = 40;
double SquareAoA = 0. / 180. * M_PI;
double SquareDiffuseAngle = 0. / 180. * M_PI;
double SquareLeft = -2.;
double SquareRight = 2.;
double SquareUp = 2.;
double SquareDown = -2.;

int nFarWakex = 80;
int nFarWakey = 40;
double farWakeAoA = 0. / 180. * M_PI;
double wakeDiffuseAngle = 8. / 180. * M_PI;
double farWakeLeft = 2.5;
double farWakeRight = 20.;
double farWakeUp = 2.;
double farWakeDown = -2.;

int curvedpts = 6;
#endif
