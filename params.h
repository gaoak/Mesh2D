// for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
// hub
bool withhub = true;
double hubRada = 0.5;
double hubRadb = 0.5;
double wallHubBLThickness = 0.1;
double maxHubBLSize = 0.1;
int Nhub = 64;
// ellipse shape
double AoA0 = 90. / 180. * M_PI;
double AoA1 = -30. / 180. * M_PI;
double AoA2 = 30. / 180. * M_PI;
double Thickness = 0.125;
double ChordLen = 1.;
// wall boundary layer
double hFirstLayer = 0.002;
double progress = 1.6;
double wallBLThickness0 = 0.05;
double maxBLSize = 0.1;
int Ncylinder = 160;
// near field
double theta0 = 0.;
double theta1 = 120. / 180. * M_PI;
double theta2 = -120. / 180. * M_PI;
double radiusL = 3.;
double radiusNearo = 3.5;
double radiusNeari = 2.5;
int Nnear = 128;
// gap
double BLNearGap = 3.5 * M_PI * radiusL / Nnear;
// far field
double radiusFar = 80.;
int Nfar = 96;

int curvedpts = 6;

#endif
