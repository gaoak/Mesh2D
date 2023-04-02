// for 3D simulation of Re400, k2, A=0.5, mesh is reduced to extreme,
// flexible boundary thickness, with 1500 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20

double hFirstLayer = 0.000460;
double progress = 1.2;
double maxLayerh = 0.001;

double xBoxLeft = 0.;
double xBoxRight = 0.260838;
double yBoxUp1 = 0.005;
double yBoxUp2 = 0.1;
double yBoxDown = hFirstLayer;

int nBoxLeft = 16;
int nBoxRight = 16;
int nBoxUp = 567;
int nBoxDown = 567;

int NxNozzel = 20;
int NyNozzel = 10;

double nozzlex0 = 0.13;
double nozzley0 = 0.03;
double nozzletheta0 = 85. / 180. * M_PI;
double nozzletheta1 = 90. / 180. * M_PI;
double nozzleradius0 = 0.05;
double nozzleradius1 = 0.052;

#endif