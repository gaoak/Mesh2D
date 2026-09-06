#ifndef PARAMS_H
#define PARAMS_H

// Hartmann channel geometry.
// The classical channel occupies y in [-1, 1].
double xMin = 0.0;
double xMax = 1.0;
double yMin = 0.;
double yLowerLayerEnd = 0.1;
double yUpperLayerStart = 0.9;
double yMax = 1.0;

// Uniform streamwise mesh. Nx may be any integer greater than or equal to 1.
int Nx = 1;

// Wall-normal mesh controls. The lower and upper intervals use geometric
// boundary-layer spacing; the middle interval is uniform.
double yFirstLayerHeight = 0.0001;
double yLayerGrowth = 1.6;
double yMaximumLayerHeight = 0.1;
double yCoreMeshSize = 0.1;

double meshTolerance = 1.0e-10;

#endif
