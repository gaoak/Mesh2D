// for 3D simulation of Re10k, k2, A=0.1, mesh is reduced to extreme,
// flexible boundary thickness, with 3162 elements
#ifndef PARAMS_H
#define PARAMS_H
#define NUMPTS 20
// gemoetry parameters
double waveLength = 1.;
double waveAmplitude = 0.039788735772974;
int numWaves = 1;
// basic mesh parameters
double centralSize = waveLength / 10.;
double upperHeight = 4.;
// bottom boundary layer
double hFirstLayerBottom = 0.01;
double progressBottom = 1.1;
double rBoundaryLayerBottom = 0.06;
double maxLayerhBottom = 0.025;
int bottomNx = 30 * numWaves;
// upper boundary layer
double upperGap = centralSize;
double hFirstLayerUpper = hFirstLayerBottom;
double upperProgress = 1.3;
int upperNy = 4;
// bottom gap mesh, filled by Gmsh
double bottomGap = centralSize;
int bottomGapNy = std::ceil(bottomGap / maxLayerhBottom);
// central layer
double centralHeight = upperHeight - upperGap;
int centralNx = std::round(waveLength / centralSize);
int centralNy =
    std::round((centralHeight - waveAmplitude - bottomGap) / centralSize);
// curved edges
int curvedpts = 6;
#endif
