// for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
// ellipse shape
double AoA = 0. / 180. * M_PI;
double Thickness = 1.;
double ChordLen = 1.;
// wall boundary layer
double hFirstLayer = 0.002;
double progress = 1.6;
double wallBLThickness0 = 0.05;
double maxBLSize = 0.1;
int Ncylinder = 128;
//near field
double theta0 = 0.;
double theta1 = 120./180.*M_PI;
double theta2 = -120./180.*M_PI;
double radiusL = 3.;
double radiusNearo = 6.;
double radiusNeari = 2.;
int Nnear = 96;
//far field
double radiusFar = 40.;
int Nfar = 32;

int curvedpts = 6;

#endif
