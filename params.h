// for 3D simulation of Re10k, k2, A=0.1, mesh is reduced to extreme,
// flexible boundary thickness, with 3162 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20

double waveLength = 1.;
double waveAmplitude = 0.5;
int numWaves = 6;

double hFirstLayerBottom = 0.003;
double progressBottom = 1.3;
double rBoundaryLayerBottom = 0.1;
double maxLayerhBottom = 0.025;
int bottomNx = 100 * numWaves;

double centralGap = 0.1;
double centralProgress = 1.3;
int centralNy = 5;

double upperHeight = 5.;
double upperProgress = 1.5;
int upperNx = 20 * numWaves;
int upperNy = 20;

int curvedpts = 6;
#endif
