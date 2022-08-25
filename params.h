//for 3D simulation of isolated vortex with a hump
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
double hfirstlayer = 0.01;
double hgrowth = 1.5;
double meshsize = 0.1;

double p0x = -10.;
double p1x = 0.;
double p2x = 1.;
double p3x = 25.;

double p0y = -5.;
double p1y = -0.01;
double p2y =  0.01;
double p3y = 5.;

int N0y = 20;
int N1y = 2;
int N2y = 20;
int N0x = 20;
int N1x = 20;
int N2x = 30;

#endif
