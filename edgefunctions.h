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

CompositEdge Cedge03;
CompositEdge Cedge69;
CompositEdge Cedge06;
CompositEdge Cedge39;
CompositEdge Cedge45;
CompositEdge Cedge1011;
CompositEdge Cedgewall;

LineEdge Cedge01(pts[0], pts[1], N0x, BOUNDARYLAYER1, 0, 0, 0, hfirstlayer, hgrowth, N0x-1);
LineEdge Cedge12(pts[1], pts[2], N1x, BOUNDARYLAYER2, hfirstlayer, hgrowth, 4, hfirstlayer, hgrowth, 4);
LineEdge Cedge23(pts[2], pts[3], N2x, BOUNDARYLAYER0, hfirstlayer, hgrowth, N2x-1, 0, 0, 0);
std::vector<double> edge01(double s) {
    return Cedge01.Evaluate(s);
}
std::vector<double> edge12(double s) {
    return Cedge12.Evaluate(s);
}
std::vector<double> edge23(double s) {
    return Cedge23.Evaluate(s);
}
std::vector<double> edge03(double s) {
    return Cedge03.Evaluate(s);
}

LineEdge Cedge1012(pts[10], pts[12], N0x, BOUNDARYLAYER1, 0, 0, 0, hfirstlayer, hgrowth, N0x-1);
LineEdge Cedge414(pts[4], pts[14], N0x, BOUNDARYLAYER1, 0, 0, 0, hfirstlayer, hgrowth, N0x-1);
std::vector<double> edge1012(double s) {
    return Cedge1012.Evaluate(s);
}
std::vector<double> edge414(double s) {
    return Cedge414.Evaluate(s);
}
LineEdge Cedge1311(pts[13], pts[11], N2x, BOUNDARYLAYER0, hfirstlayer, hgrowth, N2x-1, 0, 0, 0);
LineEdge Cedge155(pts[15], pts[5], N2x, BOUNDARYLAYER0, hfirstlayer, hgrowth, N2x-1, 0, 0, 0);
std::vector<double> edge1311(double s) {
    return Cedge1311.Evaluate(s);
}
std::vector<double> edge155(double s) {
    return Cedge155.Evaluate(s);
}

LineEdge Cedge67(pts[6], pts[7], N0x, BOUNDARYLAYER1, 0, 0, 0, hfirstlayer, hgrowth, N0x-1);
LineEdge Cedge78(pts[7], pts[8], N1x, BOUNDARYLAYER2, hfirstlayer, hgrowth, 4, hfirstlayer, hgrowth, 4);
LineEdge Cedge89(pts[8], pts[9], N2x, BOUNDARYLAYER0, hfirstlayer, hgrowth, N2x-1, 0, 0, 0);
std::vector<double> edge67(double s) {
    return Cedge67.Evaluate(s);
}
std::vector<double> edge78(double s) {
    return Cedge78.Evaluate(s);
}
std::vector<double> edge89(double s) {
    return Cedge89.Evaluate(s);
}
std::vector<double> edge69(double s) {
    return Cedge69.Evaluate(s);
}

LineEdge Cedge010(pts[0], pts[10], N0y, BOUNDARYLAYER1, 0, 0, 0, hfirstlayer, hgrowth, N0y-1);
LineEdge Cedge104(pts[10], pts[4], N1y, BOUNDARYLAYER2, hfirstlayer, hgrowth, 4, hfirstlayer, hgrowth, 4);
LineEdge Cedge46(pts[4], pts[6], N2y, BOUNDARYLAYER0, hfirstlayer, hgrowth, N2y-1, 0, 0, 0);
std::vector<double> edge010(double s) {
    return Cedge010.Evaluate(s);
}
std::vector<double> edge104(double s) {
    return Cedge104.Evaluate(s);
}
std::vector<double> edge46(double s) {
    return Cedge46.Evaluate(s);
}
std::vector<double> edge06(double s) {
    return Cedge06.Evaluate(s);
}

LineEdge Cedge311(pts[3], pts[11], N0y, BOUNDARYLAYER1, 0, 0, 0, hfirstlayer, hgrowth, N0y-1);
LineEdge Cedge115(pts[11], pts[5], N1y, BOUNDARYLAYER2, hfirstlayer, hgrowth, 4, hfirstlayer, hgrowth, 4);
LineEdge Cedge59(pts[5], pts[9], N2y, BOUNDARYLAYER0, hfirstlayer, hgrowth, N2y-1, 0, 0, 0);
std::vector<double> edge311(double s) {
    return Cedge311.Evaluate(s);
}
std::vector<double> edge115(double s) {
    return Cedge115.Evaluate(s);
}
std::vector<double> edge59(double s) {
    return Cedge59.Evaluate(s);
}
std::vector<double> edge39(double s) {
    return Cedge39.Evaluate(s);
}

//LineEdge Cedge1213(pts[12], pts[13], N0y, BOUNDARYLAYER2, hfirstlayer, hgrowth, 4, hfirstlayer, hgrowth, 4);
//LineEdge Cedge1315(pts[13], pts[15], N1y, BOUNDARYLAYER2, hfirstlayer, hgrowth, 4, hfirstlayer, hgrowth, 4);
//LineEdge Cedge1214(pts[12], pts[14], N1y, BOUNDARYLAYER2, hfirstlayer, hgrowth, 4, hfirstlayer, hgrowth, 4);
//LineEdge Cedge1415(pts[14], pts[15], N2y, BOUNDARYLAYER2, hfirstlayer, hgrowth, 4, hfirstlayer, hgrowth, 4);
LineEdge Cedge1213(pts[12], pts[13], N0y, SINREFINE2, hfirstlayer, hfirstlayer);
LineEdge Cedge1315(pts[13], pts[15], N1y, SINREFINE2, hfirstlayer, hfirstlayer);
LineEdge Cedge1214(pts[12], pts[14], N1y, SINREFINE2, hfirstlayer, hfirstlayer);
LineEdge Cedge1415(pts[14], pts[15], N2y, SINREFINE2, hfirstlayer, hfirstlayer);
std::vector<double> edge1213(double s) {
    return Cedge1213.Evaluate(s);
}
std::vector<double> edge1315(double s) {
    return Cedge1315.Evaluate(s);
}
std::vector<double> edge1214(double s) {
    return Cedge1214.Evaluate(s);
}
std::vector<double> edge1415(double s) {
    return Cedge1415.Evaluate(s);
}

std::vector<double> edge45(double s) {
    return Cedge45.Evaluate(s);
}
std::vector<double> edge1011(double s) {
    return Cedge1011.Evaluate(s);
}
std::vector<double> edgewall(double s) {
    return Cedgewall.Evaluate(s);
}

int InitPts(){
    pts[0][0] = p0x;
    pts[0][1] = p0y;
    pts[1][0] = p1x;
    pts[1][1] = p0y;
    pts[2][0] = p2x;
    pts[2][1] = p0y;
    pts[3][0] = p3x;
    pts[3][1] = p0y;

    pts[10][0] = p0x;
    pts[10][1] = p1y;
    pts[11][0] = p3x;
    pts[11][1] = p1y;

    pts[4][0] = p0x;
    pts[4][1] = p2y;
    pts[5][0] = p3x;
    pts[5][1] = p2y;

    pts[6][0] = p0x;
    pts[6][1] = p3y;
    pts[7][0] = p1x;
    pts[7][1] = p3y;
    pts[8][0] = p2x;
    pts[8][1] = p3y;
    pts[9][0] = p3x;
    pts[9][1] = p3y;

    pts[12][0] = p1x;
    pts[12][1] = p1y;
    pts[13][0] = p2x;
    pts[13][1] = p1y;
    pts[14][0] = p1x;
    pts[14][1] = p2y;
    pts[15][0] = p2x;
    pts[15][1] = p2y;

    Cedge03.addEdge(Cedge01, (void*) edge01);
    Cedge03.addEdge(Cedge12, (void*) edge12);
    Cedge03.addEdge(Cedge23, (void*) edge23);

    Cedge69.addEdge(Cedge67, (void*) edge67);
    Cedge69.addEdge(Cedge78, (void*) edge78);
    Cedge69.addEdge(Cedge89, (void*) edge89);

    Cedge06.addEdge(Cedge010, (void*) edge010);
    Cedge06.addEdge(Cedge104, (void*) edge104);
    Cedge06.addEdge(Cedge46, (void*) edge46);

    Cedge39.addEdge(Cedge311, (void*) edge311);
    Cedge39.addEdge(Cedge115, (void*) edge115);
    Cedge39.addEdge(Cedge59, (void*) edge59);

    Cedge45.addEdge(Cedge414, (void*) edge414);
    Cedge45.addEdge(Cedge1415, (void*) edge1415);
    Cedge45.addEdge(Cedge155, (void*) edge155);

    Cedge1011.addEdge(Cedge1012, (void*) edge1012);
    Cedge1011.addEdge(Cedge1213, (void*) edge1213);
    Cedge1011.addEdge(Cedge1311, (void*) edge1311);

    Cedgewall.addEdge(Cedge1213, (void*) edge1213);
    Cedgewall.addEdge(Cedge1315, (void*) edge1315);
    Cedgewall.addEdge(Cedge1214, (void*) edge1214);
    Cedgewall.addEdge(Cedge1415, (void*) edge1415);
    return 0;
}

#endif
