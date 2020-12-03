#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"

double pts[NUMPTS][2];
int findNlayers(double h, double q, double R, double m){
    int n = 0;
    double len = 0;
    double delta = h;
    for(n=1;n<=1000000; ++n) {
        if(delta>=m) delta = m;
        len += delta;
        if(len>=R) return n;
        delta *= q;
    }
    return n;
}

int N1x = std::ceil((p2x-p1x)/meshsize);
int N1y = findNlayers(hfirstlayer, hgrowth, p4y-p1y, meshsize);

int InitPts(){
    pts[0][0] = p0x;
    pts[0][1] = p0y;
    
    pts[1][0] = p1x;
    pts[1][1] = p1y;
    
    pts[2][0] = p2x;
    pts[2][1] = p2y;
    
    pts[3][0] = p3x;
    pts[3][1] = p3y;

    pts[4][0] = p4x;
    pts[4][1] = p4y;

    pts[5][0] = p5x;
    pts[5][1] = p5y;

    pts[6][0] = p6x;
    pts[6][1] = p6y;

    pts[7][0] = p7x;
    pts[7][1] = p7y;

    pts[8][0] = p8x;
    pts[8][1] = p8y;
    return 0;
}

LineEdge Cedge12(pts[1], pts[2], N1x, UNIFORM, 0., 0.);
LineEdge Cedge45(pts[4], pts[5], N1x, UNIFORM, 0., 0.);
LineEdge Cedge78(pts[7], pts[8], N1x, UNIFORM, 0., 0.);
std::vector<double> edge12(double s) {
    return Cedge12.Evaluate(s);
}
std::vector<double> edge45(double s) {
    return Cedge45.Evaluate(s);
}
std::vector<double> edge78(double s) {
    return Cedge78.Evaluate(s);
}

LineEdge Cedge25(pts[2], pts[5], N1y, BOUNDARYLAYER0, hfirstlayer, hgrowth, N1y-1, 0, 0, 0);
LineEdge Cedge14(pts[1], pts[4], N1y, BOUNDARYLAYER0, hfirstlayer, hgrowth, N1y-1, 0, 0, 0);
LineEdge Cedge03(pts[0], pts[3], N1y, BOUNDARYLAYER0, hfirstlayer, hgrowth, N1y-1, 0, 0, 0);
std::vector<double> edge25(double s) {
    return Cedge25.Evaluate(s);
}
std::vector<double> edge14(double s) {
    return Cedge14.Evaluate(s);
}
std::vector<double> edge03(double s) {
    return Cedge03.Evaluate(s);
}

LineEdge Cedge58(pts[5], pts[8], N2y, BOUNDARYLAYER0, meshsize, hgrowth, N2y-1, 0, 0, 0);
LineEdge Cedge47(pts[4], pts[7], N2y, BOUNDARYLAYER0, meshsize, hgrowth, N2y-1, 0, 0, 0);
LineEdge Cedge36(pts[3], pts[6], N2y, BOUNDARYLAYER0, meshsize, hgrowth, N2y-1, 0, 0, 0);
std::vector<double> edge58(double s) {
    return Cedge58.Evaluate(s);
}
std::vector<double> edge47(double s) {
    return Cedge47.Evaluate(s);
}
std::vector<double> edge36(double s) {
    return Cedge36.Evaluate(s);
}

LineEdge Cedge01(pts[0], pts[1], N1y, BOUNDARYLAYER1, 0, 0, 0, (p2x-p1x)/N1x, hgrowth, 10);
LineEdge Cedge34(pts[3], pts[4], N1y, BOUNDARYLAYER1, 0, 0, 0, (p5x-p4x)/N1x, hgrowth, 10);
LineEdge Cedge67(pts[6], pts[7], N1y, BOUNDARYLAYER1, 0, 0, 0, (p8x-p7x)/N1x, hgrowth, 10);
std::vector<double> edge01(double s) {
    return Cedge01.Evaluate(s);
}
std::vector<double> edge34(double s) {
    return Cedge34.Evaluate(s);
}
std::vector<double> edge67(double s) {
    return Cedge67.Evaluate(s);
}

#endif
