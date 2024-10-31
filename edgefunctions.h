#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#define NUMPTS 20
double hcenter = 0.02;
double hgrowth = 1.4;
double hfar = 0.2;

double p0x = 0.;
double p1x = 8.*M_PI;

double p0y = -1;
double p1y = -0.7;
double p2y = 0.7;
double p3y = 1.;

int N1x = std::ceil((p1x - p0x)/hfar);

double pts[NUMPTS][2];

int N0y = findNlayers(hcenter, hgrowth, p1y-p0y, hfar);
int N1y = std::ceil((p2y-p1y)/(hfar));
int N2y = findNlayers(hcenter, hgrowth, p3y-p2y, hfar);

CompositEdge Cedge06;
CompositEdge Cedge17;

LineEdge Cedge01(pts[0], pts[1], N1x, UNIFORM, 0., 0.);
std::vector<double> edge01(double s) {
    return Cedge01.Evaluate(s);
}
std::vector<double> edge17(double s) {
    return Cedge17.Evaluate(s);
}

LineEdge Cedge67(pts[6], pts[7], N1x, UNIFORM, 0., 0.);
std::vector<double> edge67(double s) {
    return Cedge67.Evaluate(s);
}

LineEdge Cedge010(pts[0], pts[10], N0y, BOUNDARYLAYER0, hcenter, hgrowth, N0y-5, 0, 0, 0);
LineEdge Cedge104(pts[10], pts[4], N1y, UNIFORM, 0, 0);
LineEdge Cedge46(pts[4], pts[6], N2y, BOUNDARYLAYER1, 0, 0, 0, hcenter, hgrowth, N2y-5);
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

LineEdge Cedge111(pts[1], pts[11], N0y, BOUNDARYLAYER0, hcenter, hgrowth, N0y-5, 0, 0, 0);
LineEdge Cedge115(pts[11], pts[5], N1y, UNIFORM, 0, 0);
LineEdge Cedge57(pts[5], pts[7], N2y, BOUNDARYLAYER1, 0, 0, 0, hcenter, hgrowth, N2y-5);
std::vector<double> edge111(double s) {
    return Cedge111.Evaluate(s);
}
std::vector<double> edge115(double s) {
    return Cedge115.Evaluate(s);
}
std::vector<double> edge57(double s) {
    return Cedge57.Evaluate(s);
}

int InitPts(){
    pts[0][0] = p0x;
    pts[0][1] = p0y;
    pts[1][0] = p1x;
    pts[1][1] = p0y;

    pts[10][0] = p0x;
    pts[10][1] = p1y;
    pts[11][0] = p1x;
    pts[11][1] = p1y;

    pts[4][0] = p0x;
    pts[4][1] = p2y;
    pts[5][0] = p1x;
    pts[5][1] = p2y;

    pts[6][0] = p0x;
    pts[6][1] = p3y;
    pts[7][0] = p1x;
    pts[7][1] = p3y;

    Cedge06.addEdge(Cedge010, (void*) edge010);
    Cedge06.addEdge(Cedge104, (void*) edge104);
    Cedge06.addEdge(Cedge46, (void*) edge46);

    Cedge17.addEdge(Cedge111, (void*) edge111);
    Cedge17.addEdge(Cedge115, (void*) edge115);
    Cedge17.addEdge(Cedge57, (void*) edge57);
    return 0;
}

#endif
