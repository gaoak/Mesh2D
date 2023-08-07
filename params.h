// for 3D simulation of Re10k, k2, A=0.1, mesh is reduced to extreme,
// flexible boundary thickness, with 3162 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
// wave parameters
double waveLength = 1.;
double waveAmplitude = 0.5;
int numWaves = 1;
// bottom boundary layer
double hFirstLayerBottom = 0.003;
double progressBottom = 1.3;
double rBoundaryLayerBottom = 0.1;
double maxLayerhBottom = 0.025;
int bottomNx = 100 * numWaves;
// central mesh, filled by Gmsh
double centralGap = 0.05;
int centralNy = 2;
// upper structured layer
double upperHeight = 0.6;
double upperProgress = 1.3;
int upperNx = 20 * numWaves;
int upperNy = 10;
// far field
double farHeight = 5.;
double farProgress = 2;
int farNy = 12;
int farNx = 2 * numWaves;
//
int curvedpts = 6;
#endif
