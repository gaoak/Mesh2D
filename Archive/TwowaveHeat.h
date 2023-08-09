// for 3D simulation of Re10k, k2, A=0.1, mesh is reduced to extreme,
// flexible boundary thickness, with 3162 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
// gemoetry parameters
double waveLength = 0.5;
double waveAmplitude = 0.025;
int numWaves = 2;
// basic mesh parameters
double centralSize = waveLength / 10.;
double upperHeight = 1.;
// bottom boundary layer
double hFirstLayerBottom = 0.003;
double progressBottom = 1.2;
double rBoundaryLayerBottom = 0.03;
double maxLayerhBottom = 0.01;
int bottomNx = 30 * numWaves;
// upper boundary layer
double upperGap = 2 * centralSize;
double hFirstLayerUpper = hFirstLayerBottom;
double upperProgress = 1.2;
int upperNy = 8;
// bottom gap mesh, filled by Gmsh
double bottomGap = centralSize;
int bottomGapNy = std::ceil(bottomGap / maxLayerhBottom);
// central layer
double centralHeight = upperHeight - upperGap;
int centralNx = std::round(waveLength * numWaves / centralSize);
int centralNy =
    std::round((centralHeight - waveAmplitude - bottomGap) / centralSize);
// curved edges
int curvedpts = 6;
#endif
