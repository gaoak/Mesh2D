//for 3D simulation of Re400, k2, A=0.5, mesh is reduced to extreme,
//flexible boundary thickness, with 1500 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
double p0x = 0.;
double p0y = 0.;

double p1x = 2.;
double p1y = 0.;

double p2x = 7.;
double p2y = 0.;

double p3x = 0.;
double p3y = 1.;

double p4x = 2.;
double p4y = 1.;

double p5x = 7.;
double p5y = 1.;

double p6x = 0.;
double p6y = 5.;

double p7x = 2.;
double p7y = 5.;

double p8x = 7.;
double p8y = 5.;

double hfirstlayer = 0.005;
double hgrowth = 1.5;
double meshsize = 0.025;
int N2y = 6;
int N2x = 6;

#endif
