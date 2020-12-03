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
int N0y = findNlayers(hfirstlayer, hgrowth, p4y-p0y, meshsize);

int InitPts(){
    pts[0][0] = p0x;
    pts[0][1] = p0y;
    pts[1][0] = p1x;
    pts[1][1] = p0y;
    pts[2][0] = p2x;
    pts[2][1] = p0y;
    pts[3][0] = p3x;
    pts[3][1] = p0y;

    pts[4][0] = p0x;
    pts[4][1] = p1y;
    pts[5][0] = p3x;
    pts[5][1] = p1y;

    pts[6][0] = p0x;
    pts[6][1] = p2y;
    pts[7][0] = p1x;
    pts[7][1] = p2y;
    pts[8][0] = p2x;
    pts[8][1] = p2y;
    pts[9][0] = p3x;
    pts[9][1] = p2y;

    Cedge03.addEdge(Cedge01);
    Cedge03.addEdge(Cedge12);
    Cedge03.addEdge(Cedge23);

    Cedge69.addEdge(Cedge67);
    Cedge69.addEdge(Cedge78);
    Cedge69.addEdge(Cedge89);

    Cedge06.addEdge(Cedge04);
    Cedge06.addEdge(Cedge46);

    Cedge39.addEdge(Cedge35);
    Cedge39.addEdge(Cedge59);
    return 0;
}

CompositEdge Cedge03;
LineEdge Cedge01(pts[0], pts[1], N0x, BOUNDARYLAYER1, 0, 0, 0, (p2x-p1x)/N1x, hgrowth, N0x-1);
LineEdge Cedge12(pts[1], pts[2], N1x, UNIFORM, 0., 0.);
LineEdge Cedge23(pts[2], pts[3], N2x, BOUNDARYLAYER0, (p2x-p1x)/N1x, hgrowth, N0x-1, 0, 0, 0);
std::vector<double> edge03(double s) {
    return Cedge03.Evaluate(s);
}

CompositEdge Cedge69;
LineEdge Cedge67(pts[6], pts[7], N0x, BOUNDARYLAYER1, 0, 0, 0, (p2x-p1x)/N1x, hgrowth, N0x-1);
LineEdge Cedge78(pts[7], pts[8], N1x, UNIFORM, 0., 0.);
LineEdge Cedge89(pts[8], pts[9], N2x, BOUNDARYLAYER0, (p2x-p1x)/N1x, hgrowth, N0x-1, 0, 0, 0);
std::vector<double> edge69(double s) {
    return Cedge69.Evaluate(s);
}

CompositEdge Cedge06;
LineEdge Cedge04(pts[0], pts[4], N0y, BOUNDARYLAYER0, hfirstlayer, hgrowth, N0y-1, 0, 0, 0);
LineEdge Cedge46(pts[4], pts[6], N1y, BOUNDARYLAYER0, meshsize, hgrowth, N1y-1, 0, 0, 0);
std::vector<double> edge06(double s) {
    return Cedge06.Evaluate(s);
}

CompositEdge Cedge39;
LineEdge Cedge35(pts[3], pts[5], N0y, BOUNDARYLAYER0, hfirstlayer, hgrowth, N0y-1, 0, 0, 0);
LineEdge Cedge59(pts[5], pts[9], N1y, BOUNDARYLAYER0, meshsize, hgrowth, N1y-1, 0, 0, 0);
std::vector<double> edge39(double s) {
    return Cedge39.Evaluate(s);
}

#endif
