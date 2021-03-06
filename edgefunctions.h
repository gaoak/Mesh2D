#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"

/////////////////////////////// do not modify
//the boundary edge should be continuous for real number s
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
int nLayers = findNlayers(hFirstLayer, progress, rBoundaryLayer, maxLayerh);

NACAmpxx naca(chamber, chamberp, Thickness);
int nTrailingEdge = ceil( (naca.up(chordLen)[1] - naca.down(chordLen)[1])/hFirstLayer);
double pts[NUMPTS][2];
double virtualpts[NUMPTS][2];

LineEdge CradiusEdge(pts[0], pts[1], nLayers, UNIFORM, 0., 0.);
std::vector<double> radiusEdge(double s) {
    int n = round(0.5*(1.+s)*nLayers);
    static vector<vector<double> > reses;
    if(reses.size()<nLayers+1) {
        vector<double> p0(2, 0.); reses.push_back(p0);
        double delta = hFirstLayer;
        for(int n=1; n<=nLayers; ++n) {
            vector<double> p1(2, 0.);
            if(delta>=maxLayerh) delta = maxLayerh;
            p1[0] = reses[reses.size()-1][0] + delta;
            delta *= progress;
            reses.push_back(p1);
        }
    }
    return reses[n];
}

int nLayersInFoil = findNlayers(hFirstLayerInFoil, progressInFoil, rBoundaryLayerInFoil, maxLayerhInFoil);

std::vector<double> radiusEdgeInFoil(double s) {
    int n = round(0.5*(1.+s)*nLayersInFoil);
    static vector<vector<double> > reses;
    if(reses.size()<nLayersInFoil+1) {
        vector<double> p0(2, 0.); reses.push_back(p0);
        double delta = hFirstLayerInFoil;
        for(int n=1; n<=nLayersInFoil; ++n) {
            vector<double> p1(2, 0.);
            if(delta>=maxLayerhInFoil) delta = maxLayerhInFoil;
            p1[0] = reses[reses.size()-1][0] + delta;
            delta *= progressInFoil;
            reses.push_back(p1);
        }
    }
    return reses[n];
}

static void transform(std::vector<double> &p, double AoA) {
    double x = p[0], y = p[1];
    p[0] = x*cos(AoA) + y*sin(AoA);
    p[1] =-x*sin(AoA) + y*cos(AoA);
}

int InitPts(){
    pts[0][0] = chordLen + wakeLen;
    pts[0][1] = wakeyUp;
    
    pts[1][0] = chordLen + wakeLen;
    pts[1][1] = wakeDown;
    
    pts[2][0] = naca.down(chordLen)[0];
    pts[2][1] = naca.down(chordLen)[1];
    
    pts[3][0] = naca.down(xmidLow1)[0];
    pts[3][1] = naca.down(xmidLow1)[1];
    
    pts[4][0] = naca.down(xmidLow2)[0];
    pts[4][1] = naca.down(xmidLow2)[1];
    
    pts[5][0] = naca.up(xmidUp2)[0];
    pts[5][1] = naca.up(xmidUp2)[1];
    
    pts[6][0] = naca.up(xmidUp1)[0];
    pts[6][1] = naca.up(xmidUp1)[1];
    
    pts[7][0] = naca.up(chordLen)[0];
    pts[7][1] = naca.up(chordLen)[1];
    
    pts[8][0] = xBoxRight;
    pts[8][1] = yBoxDown;
    
    pts[9][0] = xBoxRight;
    pts[9][1] = yBoxUp;
    
    pts[10][0] = xBoxLeft;
    pts[10][1] = yBoxUp;
    
    pts[11][0] = xBoxLeft;
    pts[11][1] = yBoxDown;
    
    pts[13][0] = chordLen + wakeLen;
    pts[13][1] = -radiusEdge(1.)[0] - wakeLen*tan(nearWakeDiffuseAngle);
    
    pts[14][0] = chordLen + wakeLen;
    pts[14][1] = radiusEdge(1.)[0] + wakeLen*tan(nearWakeDiffuseAngle);

    virtualpts[2][0] = 1.;
    virtualpts[3][0] = xmidLow1;
    virtualpts[4][0] = xmidLow2;
    virtualpts[5][0] = xmidUp2;
    virtualpts[6][0] = xmidUp1;
    virtualpts[7][0] = 1.;
    return 0;
}


double sFrontUp = naca.finds(xmidUp2,    1);
double sFrontLow = naca.finds(xmidLow2, -1);
double smid1= naca.finds(chordLen, 1) - naca.finds(xmidUp1, 1);

CompositEdge CinnerEdge;
std::vector<double> innerEdge(double s) {
    return CinnerEdge.Evaluate(s);
}

// airfoil surfaces
LineEdge Cedge2(virtualpts[2], virtualpts[3], nLow1 , QUDREFINE0, hTrailingEdge, 0.);
LineEdge Cedge3(virtualpts[3], virtualpts[4], nLow2 , QUDREFINE1, 0., (sFrontUp+sFrontLow)/nFront);
LineEdge Cedge4(virtualpts[4], virtualpts[5], nFront, UNIFORM, 0., 0.);
LineEdge Cedge5(virtualpts[5], virtualpts[6], nUp2  , QUDREFINE0, (sFrontUp+sFrontLow)/nFront, 0.);
LineEdge Cedge6(virtualpts[6], virtualpts[7], nUp1  , QUDREFINE1, 0., hTrailingEdge);
std::vector<double> edge2(double s)
{
	std::vector<double> res = Cedge2.Evaluate(s);
	return naca.down(res[0]);
}
std::vector<double> edge3(double s)
{
	std::vector<double> res = Cedge3.Evaluate(s);
	return naca.down(res[0]);
}
std::vector<double> edge5(double s)
{
	std::vector<double> res = Cedge5.Evaluate(s);
	return naca.up(res[0]);
}
std::vector<double> edge6(double s)
{
	std::vector<double> res = Cedge6.Evaluate(s);
	return naca.up(res[0]);
}
std::vector<double> edge4(double s)
{
	double ys = -sqrt(xmidLow2), ye = sqrt(xmidUp2);
	s = ys + (s+1.)*(ye-ys)/2.;
	double x = s*s;
    if(s<0.) return naca.down(x);
    else     return naca.up(x);
}

// straight edges around the airfoil
LineEdge Cedge0(pts[0], pts[1], nTrailingEdge,  UNIFORM, 0., 0.);
LineEdge Cedge1(pts[1], pts[2], nWake, QUDREFINE1, 0., 0.7*hTrailingEdge);
LineEdge Cedge7(pts[7], pts[0], nWake, QUDREFINE0, 0.7*hTrailingEdge, 0.);
LineEdge Cedge8(pts[2], pts[7], nTrailingEdge, UNIFORM, 0., 0.);
std::vector<double> edge0(double s) {
    return Cedge0.Evaluate(s);
}
std::vector<double> edge1(double s) {
    return Cedge1.Evaluate(s);
}
std::vector<double> edge7(double s) {
    return Cedge7.Evaluate(s);
}
std::vector<double> edge8(double s) {
    return Cedge8.Evaluate(s);
}

// straight edges in the outter box
CompositEdge CoutterEdge;
std::vector<double> outterEdge(double s) {
    return CoutterEdge.Evaluate(s);
}
LineEdge Cedge9 (pts[ 8], pts[ 9], nBoxRight,  UNIFORM, 0., 0.);
LineEdge Cedge10(pts[ 9], pts[10], nBoxUp   , UNIFORM, 0., 0.);
LineEdge Cedge11(pts[10], pts[11], nBoxLeft , UNIFORM, 0., 0.);
LineEdge Cedge12(pts[11], pts[ 8], nBoxDown , UNIFORM, 0., 0.);
std::vector<double> edge9(double s) {
    return Cedge9.Evaluate(s);
}
std::vector<double> edge10(double s) {
    return Cedge10.Evaluate(s);
}
std::vector<double> edge11(double s) {
    return Cedge11.Evaluate(s);
}
std::vector<double> edge12(double s) {
    return Cedge12.Evaluate(s);
}

// straight edges in the wake
LineEdge Cedge14(pts[12], pts[13], nWake, QUDREFINE0,  hTrailingEdge, 0.);
LineEdge Cedge15(pts[13],  pts[1], nLayers, QUDREFINE1, 0., (wakeyUp-wakeDown)/nTrailingEdge);
LineEdge Cedge16(pts[0],  pts[14], nLayers, QUDREFINE0, (wakeyUp-wakeDown)/nTrailingEdge, 0.);
LineEdge Cedge17(pts[14], pts[15], nWake, QUDREFINE1, 0., hTrailingEdge);
std::vector<double> edge14(double s) {
    return Cedge14.Evaluate(s);
}
std::vector<double> edge15(double s) {
    return Cedge15.Evaluate(s);
}
std::vector<double> edge16(double s) {
    return Cedge16.Evaluate(s);
}
std::vector<double> edge17(double s) {
    return Cedge17.Evaluate(s);
}
// imaged edge
LineEdge Cedge13(pts[2], pts[12], nLayers,  UNIFORM, 0., 0.);
LineEdge Cedge18(pts[15], pts[7], nLayers, UNIFORM, 0., 0.);
vector<double> norm13(2,0.);
vector<double> norm18(2,0.);
std::vector<double> edge13(double s) {
    vector<double> res(2);
    res[0] = pts[2][0] + radiusEdge(s)[0]*norm13[0];
    res[1] = pts[2][1] + radiusEdge(s)[0]*norm13[1];
    return res;
}
std::vector<double> edge18(double s) {
    vector<double> res(2);
    res[0] = pts[7][0] + radiusEdge(s)[0]*norm18[0];
    res[1] = pts[7][1] + radiusEdge(s)[0]*norm18[1];
    return res;
}
// straight edges in the far wake
LineEdge Cedge19(pts[16], pts[17], nFarWakex, QUDREFINE0,  (farWakeUp-farWakeDown)/nFarWakey, 0.);
LineEdge Cedge20(pts[17], pts[18], nFarWakey, UNIFORM, 0., 0.);
LineEdge Cedge21(pts[18], pts[19], nFarWakex, QUDREFINE1, 0., (farWakeUp-farWakeDown)/nFarWakey);
LineEdge Cedge22(pts[19], pts[16], nFarWakey, UNIFORM, 0., 0.);
std::vector<double> edge19(double s) {
    return Cedge19.Evaluate(s);
}
std::vector<double> edge20(double s) {
    return Cedge20.Evaluate(s);
}
std::vector<double> edge21(double s) {
    return Cedge21.Evaluate(s);
}
std::vector<double> edge22(double s) {
    return Cedge22.Evaluate(s);
}
#endif
