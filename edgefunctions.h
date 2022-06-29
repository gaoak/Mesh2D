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
int nLayers  = findNlayers(hFirstLayer, progress, rBoundaryLayer,  maxLayerh);
int nLayers1 = findNlayers(hFirstLayer, progress, rBoundaryLayer1, maxLayerh);
int nLayers2 = findNlayers(hFirstLayer, progress, rBoundaryLayer2, maxLayerh);
int nLayers3 = findNlayers(hFirstLayer, progress, rBoundaryLayer3, maxLayerh);
int nLayers4 = findNlayers(hFirstLayer, progress, rBoundaryLayer4, maxLayerh);
int nLayers5 = findNlayers(hFirstLayer, progress, rBoundaryLayer5, maxLayerh);
int nLayers6 = findNlayers(hFirstLayer, progress, rBoundaryLayer6, maxLayerh);
int nLayers7 = findNlayers(hFirstLayer, progress, rBoundaryLayer7, maxLayerh);

SplineEdge upper("data/uppersurface");
SplineEdge lower("data/lowersurface");
int nTrailingEdge = std::max(2, (int)ceil( (upper.Evaluate(chordLen)[1] - lower.Evaluate(chordLen)[1])/hFirstLayer) );
double pts[NUMPTS][2];
double virtualpts[NUMPTS][2];

LineEdge CradiusEdge(pts[0], pts[1], nLayers, UNIFORM, 0., 0.);
void setRadiusLayers(int n) {
    nLayers = n;
}
std::vector<double> radiusEdge(double s) {
    int n = round(0.5*(1.+s)*nLayers);
    static vector<vector<double> > reses;
    if(reses.size()<nLayers+1) {
        reses.clear();
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
    
    pts[2][0] = lower.Evaluate(chordLen)[0];
    pts[2][1] = lower.Evaluate(chordLen)[1];
    
    pts[3][0] = lower.Evaluate(xmidLow1)[0];
    pts[3][1] = lower.Evaluate(xmidLow1)[1];
    
    pts[4][0] = lower.Evaluate(xmidLow2)[0];
    pts[4][1] = lower.Evaluate(xmidLow2)[1];
    
    pts[5][0] = upper.Evaluate(xmidUp2)[0];
    pts[5][1] = upper.Evaluate(xmidUp2)[1];
    
    pts[6][0] = upper.Evaluate(xmidUp1)[0];
    pts[6][1] = upper.Evaluate(xmidUp1)[1];
    
    pts[7][0] = upper.Evaluate(chordLen)[0];
    pts[7][1] = upper.Evaluate(chordLen)[1];
    
    pts[8][0] = xBoxRight;
    pts[8][1] = yBoxDown;
    
    pts[9][0] = xBoxRight;
    pts[9][1] = yBoxUp;
    
    pts[10][0] = xBoxLeft;
    pts[10][1] = yBoxUp;
    
    pts[11][0] = xBoxLeft;
    pts[11][1] = yBoxDown;
    
    setRadiusLayers(nLayers1);
    pts[13][0] = chordLen + wakeLen;
    pts[13][1] = -radiusEdge(1.)[0] - wakeLen*tan(nearWakeDiffuseAngle);
    
    setRadiusLayers(nLayers7);
    pts[14][0] = chordLen + wakeLen;
    pts[14][1] = radiusEdge(1.)[0] + wakeLen*tan(nearWakeDiffuseAngle);

    virtualpts[2][0] = pts[2][0];
    virtualpts[2][1] = pts[2][1];
    virtualpts[7][0] = pts[7][0];
    virtualpts[7][1] = pts[7][1];

    virtualpts[3][0] = lower.Evaluate(xmidLow1)[0];
    virtualpts[3][1] = lower.Evaluate(xmidLow1)[1];

    virtualpts[4][0] = lower.Evaluate(xmidLow2)[0];
    virtualpts[4][1] = lower.Evaluate(xmidLow2)[1];

    virtualpts[5][0] = upper.Evaluate(xmidUp2)[0];
    virtualpts[5][1] = upper.Evaluate(xmidUp2)[1];

    virtualpts[6][0] = upper.Evaluate(xmidUp1)[0];
    virtualpts[6][1] = upper.Evaluate(xmidUp1)[1];
    return 0;
}

double sFrontUp  = upper.finds(xmidUp2,  0);
double sFrontLow = lower.finds(xmidLow2, 0);
double smid1= upper.finds(chordLen, 0) - upper.finds(xmidUp1, 0);

CompositEdge CinnerEdge;
std::vector<double> innerEdge(double s) {
    return CinnerEdge.Evaluate(s);
}

// airfoil surfaces
double hTrailingEdge = upper.Evaluate(chordLen)[1] - lower.Evaluate(chordLen)[1];
LineEdge Cedge2(virtualpts[2], virtualpts[3], nLow1 , BOUNDARYLAYER0, hTrailingEdge, (hTrailingEdge+hFirstLayer)/hTrailingEdge, 5, 0., 0., 0);
LineEdge Cedge3(virtualpts[3], virtualpts[4], nLow2 , BOUNDARYLAYER1, 0., 0., 0, (sFrontUp+sFrontLow)/nFront, growthrateLow2, std::min(10, nLow2-1) );
LineEdge Cedge4(virtualpts[4], virtualpts[5], nFront, UNIFORM, 0., 0.);
LineEdge Cedge5(virtualpts[5], virtualpts[6], nUp2  , BOUNDARYLAYER0, (sFrontUp+sFrontLow)/nFront, growthrateUp2, std::min(10, nUp2-1), 0., 0., 1);
LineEdge Cedge6(virtualpts[6], virtualpts[7], nUp1  , BOUNDARYLAYER1, 0., 0., 0, hTrailingEdge, (hTrailingEdge+hFirstLayer)/hTrailingEdge, 5);
std::vector<double> edge2(double s)
{
	std::vector<double> res = Cedge2.Evaluate(s);
	return lower.Evaluate(res[0]);
}
std::vector<double> edge3(double s)
{
	std::vector<double> res = Cedge3.Evaluate(s);
	return lower.Evaluate(res[0]);
}
std::vector<double> edge5(double s)
{
	std::vector<double> res = Cedge5.Evaluate(s);
	return upper.Evaluate(res[0]);
}
std::vector<double> edge6(double s)
{
	std::vector<double> res = Cedge6.Evaluate(s);
	return upper.Evaluate(res[0]);
}
std::vector<double> edge4(double s)
{
    std::vector<double> LE{0, 0.00047};
    std::vector<double> res = Cedge4.Evaluate(s);
    if(res[1]<=LE[1]) res = lower.Evaluate(res[1], 1);
    else     res = upper.Evaluate(res[1], 1);
    return res;
}

// straight edges around the airfoil
LineEdge Cedge0(pts[0], pts[1], nTrailingEdge,  UNIFORM, 0., 0.);
LineEdge Cedge1(pts[1], pts[2], nWake, BOUNDARYLAYER1, 0., 0., 0, hTrailingEdge+hFirstLayer, 2., 2);
LineEdge Cedge7(pts[7], pts[0], nWake, BOUNDARYLAYER0, hTrailingEdge+hFirstLayer, 2., 2, 0., 0., 0);
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
LineEdge Cedge14(pts[12], pts[13], nWake, BOUNDARYLAYER0, hTrailingEdge+hFirstLayer, 2., 2, 0., 0., 0);
LineEdge Cedge15(pts[13],  pts[1], nLayers1-1, BOUNDARYLAYER1, 0., 0., 0, hFirstLayer*progress, progress, (nLayers1-1)*2/3);
LineEdge Cedge16(pts[0],  pts[14], nLayers7-1, BOUNDARYLAYER0, hFirstLayer*progress, progress, (nLayers7-1)*2/3, 0., 0., 0);
LineEdge Cedge17(pts[14], pts[15], nWake, BOUNDARYLAYER1, 0., 0., 0, hTrailingEdge+hFirstLayer, 2., 2);
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
LineEdge Cedge13(pts[2], pts[12], nLayers1-1,  UNIFORM, 0., 0.);
LineEdge Cedge18(pts[15], pts[7], nLayers7-1, UNIFORM, 0., 0.);
vector<double> norm13(2,0.);
vector<double> norm18(2,0.);
std::vector<double> edge13(double s) {
    setRadiusLayers(nLayers1);
    vector<double> res(2);
    double h0 = radiusEdge(2./nLayers1 - 1.)[0];
    s = (s+1.)*(1.-1./nLayers1) + 2./nLayers1 - 1.;
    double h = radiusEdge(s)[0]-h0;
    res[0] = pts[2][0] + h*norm13[0];
    res[1] = pts[2][1] + h*norm13[1];
    return res;
}
std::vector<double> edge18(double s) {
    setRadiusLayers(nLayers7);
    vector<double> res(2);
    double h0 = radiusEdge(2./nLayers7 - 1.)[0];
    s = (s+1.)*(1.-1./nLayers7) + 2./nLayers7 - 1.;
    double h = radiusEdge(s)[0]-h0;
    res[0] = pts[7][0] + h*norm18[0];
    res[1] = pts[7][1] + h*norm18[1];
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

//trailing edge
LineEdge Cedgeb2_2(virtualpts[2], pts[2], 1, UNIFORM, 0., 0.);
LineEdge Cedgeb7_7(virtualpts[7], pts[7], 1, UNIFORM, 0., 0.);
LineEdge Cedgeb2_b7(virtualpts[7], virtualpts[2], nTrailingEdge, UNIFORM, 0., 0.);
std::vector<double> edgeb2_2(double s) {
    return Cedgeb2_2.Evaluate(s);
}
std::vector<double> edgeb7_7(double s) {
    return Cedgeb7_7.Evaluate(s);
}
std::vector<double> edgeb2_b7(double s) {
    return Cedgeb2_b7.Evaluate(s);
}
#endif
