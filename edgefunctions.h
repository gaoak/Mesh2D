#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"

double pts[NUMPTS][2];

int InitPts(){
    pts[0][0] = 0.;
    pts[0][1] = 0.;
    
    pts[1][0] = 1.;
    pts[1][1] = 0.;
    
    pts[2][0] = 1.;
    pts[2][1] = 1.;
    
    pts[3][0] = 0.;
    pts[3][1] = 1.;
    return 0;
}

LineEdge Cedge0(pts[0], pts[1], Nx, UNIFORM, 0., 0.);
LineEdge Cedge1(pts[1], pts[2], Ny, UNIFORM, 0., 0.);
LineEdge Cedge2(pts[3], pts[2], Nx, UNIFORM, 0., 0.);
LineEdge Cedge3(pts[0], pts[3], Ny, UNIFORM, 0., 0.);

std::vector<double> edge0(double s) {
    return Cedge0.Evaluate(s);
}
std::vector<double> edge1(double s) {
    return Cedge1.Evaluate(s);
}
std::vector<double> edge2(double s) {
    return Cedge2.Evaluate(s);
}
std::vector<double> edge3(double s) {
    return Cedge3.Evaluate(s);
}
#endif
