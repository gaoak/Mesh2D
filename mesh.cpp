#include<iostream>
#include<vector>
#include<cmath>
#include<cstring>
#include"CAD2D/CompositEdge.h"
#include"CAD2D/MeshRegions.h"
#include "CAD2D/MeshTool.h"
#include"CAD2D/RectRegion.h"
#include"CAD2D/LineEdge.h"
#include"CAD2D/util.h"
#include"CAD2D/SplineEdge.h"
#include<fstream>
using namespace std;

#include"edgefunctions.h"
int meshingNearBody(MeshRegions &combinedReg);
int meshingWake(MeshRegions &combinedReg);
int meshingInFoil(MeshRegions &nearWallRegion, MeshRegions &inFoilRegion, vector<vector<double> > &breakpts);
int outputXML(MeshRegions &combinedReg, MeshRegions &inFoilRegion);
int outputGeo(MeshRegions &combinedReg, MeshRegions &nearWallRegion, vector<vector<double> > &breakpts, MeshRegions &FarFieldReg, vector<double> pC);
int meshingInFoil_v2(MeshRegions & nearWallRegion, MeshRegions &inFoilRegion, vector<vector<double> > &breakpts);
int meshingOuterBoundary(MeshRegions &combinedReg, vector<double> &p);
int main(int argc, char* argv[])
{
    bool merge = false;
    bool withwake = false;
    string mshfilename;
    string mshinfoilfilename1;
    string mshinfoilfilename2;
    for(int i=1; i<argc; ++i) {
        if(strcmp(argv[i], "merge")==0) {
            if(argc>i+1) mshfilename = string(argv[i+1]);
            if(argc>i+2) mshinfoilfilename1 = string(argv[i+2]);
            if(argc>i+3) mshinfoilfilename2 = string(argv[i+3]);
            merge = true;
        }
        if(strcmp(argv[i], "wake")==0) {
            withwake = true;
        }
    }
    /// initialize
    MeshRegions combinedReg("R_Comb_", 3.E-5);
    MeshRegions nearFieldReg("R_NearField_", 5.E-5);
    MeshRegions nearWallRegion("R_NearField_", 3.E-5);
    MeshRegions inFoilRegion("R_inFoil_", 3.E-5);
    InitPts();
    cout << "start meshing --------" << endl;
    meshingNearBody(combinedReg);
    nearWallRegion.AddRegion(combinedReg);
    combinedReg.transformation(AoA);
    MeshRegions FarFieldReg("RFar", 3.E-5);
    vector<double> pC;
    meshingOuterBoundary(FarFieldReg, pC);
    if(withwake) meshingWake(combinedReg);
    vector<vector<double> > breakpts;
    meshingInFoil_v2(nearWallRegion, inFoilRegion, breakpts);
    nearWallRegion.transformation(AoA);
    inFoilRegion.transformation(AoA);

    if(!merge) {
        outputGeo(combinedReg, nearWallRegion, breakpts, FarFieldReg, pC);
        cout << "output CAD file" << endl;
        cout << "=======================================" << endl;
    }
    /////////////////////////////////////////////////////
    //////////////gmsh region, far field region//////////
    // step 2 import mesh
    if(merge) {
        combinedReg.AddRegion(FarFieldReg);
        MeshRegions gmshReg("R_gmsh_", 3.E-5);
        gmshReg.loadFromMsh(mshfilename, 135./180.*3.14159);
        cout << "load " << mshfilename << endl;
        vector<int> comp1;
        comp1.push_back(0); comp1.push_back(gmshReg.getCellsNumber());
        gmshReg.outXml("FarField.xml");
        gmshReg.outCOMPO("FarField.xml", comp1);
        if(!combinedReg.consistancyCheck(gmshReg)) {
            cout << "Error: node mismatch, exit" << endl;
            return -1;
        }
        if(!gmshReg.consistancyCheck(combinedReg)) {
            cout << "Error: node mismatch, exit" << endl;
            return -1;
        }
        combinedReg.AddRegion(gmshReg);
        //in airfoil mesh
        MeshRegions gmshInFoil1("R_gmsh1_", 3.E-5);
        gmshInFoil1.loadFromMsh(mshinfoilfilename1, 135./180.*3.14159);
        cout << "load " << mshinfoilfilename1 << endl;
        if(!nearWallRegion.consistancyCheck(gmshInFoil1)) {
            cout << "Error: node mismatch, exit" << endl;
            return -1;
        }
        inFoilRegion.AddRegion(gmshInFoil1);
        MeshRegions gmshInFoil2("R_gmsh2_", 3.E-5);
        gmshInFoil2.loadFromMsh(mshinfoilfilename2, 135./180.*3.14159);
        cout << "load " << mshinfoilfilename2 << endl;
        if(!nearWallRegion.consistancyCheck(gmshInFoil2)) {
            cout << "Error: node mismatch, exit" << endl;
            return -1;
        }
        if(!gmshInFoil2.consistancyCheck(nearWallRegion)) {
            cout << "Error: node mismatch, exit" << endl;
            return -1;
        }
        inFoilRegion.AddRegion(gmshInFoil2);
        outputXML(combinedReg, inFoilRegion);
        cout << "------------------------------------" << endl;
        cout << "------------------------------------" << endl;
    }
    return 0;
}

int meshingNearBody(MeshRegions &combinedReg)
{
    /////////near body region////////////////
    std::vector<RectRegion> Rects;
    //boundary layer region 0
    std::vector<void*> edges0;
    //edge 2
    edges0.push_back((void*)edge2);
    edges0.push_back((void*)radiusEdge);
    edges0.push_back((void*)edge0);
    edges0.push_back((void*)edge0);
    Rects.push_back(RectRegion(edges0, "Rwall2", false));
    setRadiusLayers(nLayers2);
    Rects[Rects.size()-1].MeshGen(Cedge2.m_N, nLayers2, eBoundaryLayer1, false, hFirstLayer,0.);
    Rects[Rects.size()-1].Tec360Pts("test0.dat");
    // edge 3
    edges0[0] = (void*)edge3;
    Rects.push_back(RectRegion(edges0, "Rwall3", false));
    setRadiusLayers(nLayers3);
    Rects[Rects.size()-1].MeshGen(Cedge3.m_N, nLayers3, eBoundaryLayer1, false);
    Rects[Rects.size()-1].Tec360Pts("test1.dat");
    // edge 4
    edges0[0] = (void*)edge4;
    Rects.push_back(RectRegion(edges0, "Rwall4", false));
    setRadiusLayers(nLayers4);
    Rects[Rects.size()-1].MeshGen(Cedge4.m_N, nLayers4, eBoundaryLayer1, false);
    Rects[Rects.size()-1].Tec360Pts("test3.dat");
    // edge 5
    edges0[0] = (void*)edge5;
    Rects.push_back(RectRegion(edges0, "Rwall5", false));
    setRadiusLayers(nLayers5);
    Rects[Rects.size()-1].MeshGen(Cedge5.m_N, nLayers5, eBoundaryLayer1, false);
    Rects[Rects.size()-1].Tec360Pts("test4.dat");
    // edge 6
    edges0[0] = (void*)edge6;
    Rects.push_back(RectRegion(edges0, "Rwall6", false));
    setRadiusLayers(nLayers6);
    Rects[Rects.size()-1].MeshGen(Cedge6.m_N, nLayers6, eBoundaryLayer1, false, 0., hFirstLayer);
    Rects[Rects.size()-1].Tec360Pts("test5.dat");

    //give the normal direction, and points 12, 15
    vector<double> p1  = Rects[Rects.size()-1].getVertexOffset(1);
    vector<double> n12 = Rects[Rects.size()-1].getVertex(2);
    pts[7][0] = p1[0];
    pts[7][1] = p1[1];
    pts[15][0] = n12[0];
    pts[15][1] = n12[1];
    n12[0] = n12[0] - p1[0]; n12[1] = n12[1] - p1[1];
    norm18[0] = n12[0]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    norm18[1] = n12[1]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    setRadiusLayers(nLayers7);
    pts[14][0] = pts[0][0] + norm18[0]*radiusEdge(1.)[0];
    pts[14][1] = 0. + norm18[1]*radiusEdge(1.)[0];
    p1  = Rects[0].getVertexOffset(0);
    n12 = Rects[0].getVertex(3);
    pts[2][0] = p1[0];
    pts[2][1] = p1[1];
    pts[12][0] = n12[0];
    pts[12][1] = n12[1];
    n12[0] = n12[0] - p1[0]; n12[1] = n12[1] - p1[1];
    norm13[0] = n12[0]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    norm13[1] = n12[1]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    setRadiusLayers(nLayers1);
    pts[13][0] = pts[1][0] + norm13[0]*radiusEdge(1.)[0];
    pts[13][1] = 0. + norm13[1]*radiusEdge(1.)[0];
    //region 1
    std::vector<void*> edges1;
    edges1.push_back((void*)edge1);
    edges1.push_back((void*)edge0);
    edges1.push_back((void*)edge7);
    edges1.push_back((void*)edge8);
    Rects.push_back(RectRegion(edges1, "RTrailing"));
    Rects[Rects.size()-1].MeshGen(Cedge1.m_N, Cedge0.m_N);
    Rects[Rects.size()-1].Tec360Pts("testwake.dat");
    //region 2
    std::vector<void*> edges2;
    edges2.push_back((void*)edge14);
    edges2.push_back((void*)edge15);
    edges2.push_back((void*)edge1);
    edges2.push_back((void*)edge13);
    Rects.push_back(RectRegion(edges2, "RTrailDown"));
    Rects[Rects.size()-1].MeshGen(Cedge14.m_N, Cedge15.m_N);
    Rects[Rects.size()-1].Tec360Pts("test6.dat");
    //region 3
    std::vector<void*> edges3;
    edges3.push_back((void*)edge7);
    edges3.push_back((void*)edge16);
    edges3.push_back((void*)edge17);
    edges3.push_back((void*)edge18);
    Rects.push_back(RectRegion(edges3, "RTrailUp"));
    Rects[Rects.size()-1].MeshGen(Cedge7.m_N, Cedge16.m_N);
    Rects[Rects.size()-1].Tec360Pts("test7.dat");
    //regin trailing edge
    std::vector<void*> edgest;
    edgest.push_back((void*)edge8);
    edgest.push_back((void*)edgeb7_7);
    edgest.push_back((void*)edgeb2_b7);
    edgest.push_back((void*)edgeb2_2);
    Rects.push_back(RectRegion(edgest, "R_wake_Up"));
    Rects[Rects.size()-1].MeshGen(Cedge8.m_N, Cedgeb7_7.m_N);
    Rects[Rects.size()-1].Tec360Pts("test8.dat");
    
    ///////////// combine the near field mesh
    for(unsigned int i=0; i<Rects.size(); ++i) {
    	combinedReg.AddRegion(Rects[i]);
    }
    return 0;
}

int meshingOuterBoundary(MeshRegions &combinedReg, vector<double> &p)
{
    vector<double> pD = edge10(-1. + 1*2./Cedge10.m_N);
    vector<double> pH = edge10( 1. - 1*2./Cedge10.m_N);
    vector<double> pE = edge12(-1. + 1*2./Cedge12.m_N);
    vector<double> pA = edge12( 1. - 1*2./Cedge12.m_N);
    vector<double> p8 = edge9(-1.);
    vector<double> p9 = edge9(1.);
    vector<double> p10 = edge11(-1.);
    vector<double> p11 = edge11(1.);
    vector<vector<vector<double> > > edges;
    // right edge
    vector<vector<double> > EA8;
    vector<vector<double> > E89;
    vector<vector<double> > ED9;
    vector<vector<double> > EAD;
    EA8.push_back(pA);
    EA8.push_back(p8);
    ED9.push_back(pD);
    ED9.push_back(p9);
    for(int i=0; i<=Cedge9.m_N; ++i) {
        double tmps = -1. + i*2./Cedge9.m_N;
        vector<double> p0 = edge9(tmps);
        E89.push_back(p0);
        p0[0] = pD[0]*(p0[1] - pA[1])/(pD[1] - pA[1]) + pA[0]*(p0[1] - pD[1])/(pA[1] - pD[1]);
        EAD.push_back(p0);
    }
    edges.clear();
    edges.push_back(EA8);
    edges.push_back(E89);
    edges.push_back(ED9);
    edges.push_back(EAD);
    RectRegion pic9 = RectRegion(edges, "pic");
    pic9.MeshGen(1, Cedge9.m_N);
    pic9.Tec360Pts("pic9.dat");
    combinedReg.AddRegion(pic9);
    // left  edge
    vector<vector<double> > EH10;
    vector<vector<double> > E1011;
    vector<vector<double> > EE11;
    vector<vector<double> > EHE;
    EH10.push_back(pH);
    EH10.push_back(p10);
    EE11.push_back(pE);
    EE11.push_back(p11);
    for(int i=0; i<=Cedge11.m_N; ++i) {
        double tmps = -1. + i*2./Cedge11.m_N;
        vector<double> p0 = edge11(tmps);
        E1011.push_back(p0);
        p0[0] = pH[0]*(p0[1] - pE[1])/(pH[1] - pE[1]) + pE[0]*(p0[1] - pH[1])/(pE[1] - pH[1]);
        EHE.push_back(p0);
    }
    edges.clear();
    edges.push_back(EH10);
    edges.push_back(E1011);
    edges.push_back(EE11);
    edges.push_back(EHE);
    RectRegion pic11 = RectRegion(edges, "pic");
    pic11.MeshGen(1, Cedge11.m_N);
    pic11.Tec360Pts("pic11.dat");
    combinedReg.AddRegion(pic11);
    // upper edge
    vector<double> pB = EAD[1];
    vector<double> pC = EAD[EAD.size()-2];
    vector<double> pG = EHE[1];
    vector<double> pF = EHE[EHE.size()-2];
    vector<vector<double> > EDH;
    vector<vector<double> > ECG;
    vector<vector<double> > ECD;
    vector<vector<double> > EHG;
    ECD.push_back(pD);
    ECD.push_back(pC);
    EHG.push_back(pH);
    EHG.push_back(pG);
    for(int i=1; i<Cedge10.m_N; ++i) {
        double tmps = -1. + i*2./Cedge10.m_N;
        vector<double> p0 = edge10(tmps);
        EDH.push_back(p0);
        p0[1] = pC[1]*(p0[0] - pG[0])/(pC[0] - pG[0]) + pG[1]*(p0[0] - pC[0])/(pG[0] - pC[0]);
        if(i==1) {
            ECG.push_back(pC);
        } else if(i==Cedge10.m_N - 1) {
            ECG.push_back(pG);
        } else {
            ECG.push_back(p0);
        }
    }
    edges.clear();
    edges.push_back(ECD);
    edges.push_back(EDH);
    edges.push_back(EHG);
    edges.push_back(ECG);
    RectRegion pic10 = RectRegion(edges, "pic");
    pic10.MeshGen(1, Cedge10.m_N-2);
    pic10.Tec360Pts("pic10.dat");
    combinedReg.AddRegion(pic10);
    // lower edge
    vector<vector<double> > EAE;
    vector<vector<double> > EBF;
    vector<vector<double> > EAB;
    vector<vector<double> > EEF;
    EAB.push_back(pA);
    EAB.push_back(pB);
    EEF.push_back(pE);
    EEF.push_back(pF);
    for(int i=1; i<Cedge12.m_N; ++i) {
        double tmps = -1. + i*2./Cedge12.m_N;
        vector<double> p0 = edge12(tmps);
        EAE.push_back(p0);
        p0[1] = pB[1]*(p0[0] - pF[0])/(pB[0] - pF[0]) + pF[1]*(p0[0] - pB[0])/(pF[0] - pB[0]);
        if(i==1) {
            EBF.push_back(pF);
        } else if(i==Cedge12.m_N - 1) {
            EBF.push_back(pB);
        } else {
            EBF.push_back(p0);
        }
    }
    edges.clear();
    edges.push_back(EAB);
    edges.push_back(EBF);
    edges.push_back(EEF);
    edges.push_back(EAE);
    RectRegion pic12 = RectRegion(edges, "pic");
    pic12.MeshGen(1, Cedge12.m_N-2);
    pic12.Tec360Pts("pic12.dat");
    combinedReg.AddRegion(pic12);
    //
    p = pC;
    //
    //vector<int> comp4;
    //comp4.push_back(0);
    //combinedReg.outXml("manout.xml");
    //combinedReg.outCOMPO("manout.xml", comp4);
    return 0;
}

int meshingWake(MeshRegions &combinedReg)
{
    vector<double> pwakeLU;
    pwakeLU.push_back(pts[14][0]); pwakeLU.push_back(pts[14][1]);
    transform(pwakeLU, AoA);
    pts[16][1] = farWakeDown;
    pts[19][1] = farWakeUp;
    double gapofFarWakeL = 1.8*(pts[19][1]-pts[16][1])/nFarWakey + 0.*pts[14][1]/nLayers;
    pts[16][0] = pwakeLU[0] + gapofFarWakeL;
    pts[19][0] = pwakeLU[0] + gapofFarWakeL;
    pts[17][0] = farWakeRight;
    pts[18][0] = farWakeRight;
    pts[17][1] = pts[16][1] - (pts[17][0]-pts[16][0])*tan(wakeDiffuseAngle);
    pts[18][1] = pts[19][1] + (pts[18][0]-pts[19][0])*tan(wakeDiffuseAngle);
    std::vector<void*> edges4;
    edges4.push_back((void*)edge19);
    edges4.push_back((void*)edge20);
    edges4.push_back((void*)edge21);
    edges4.push_back((void*)edge22);
    RectRegion farwakeRegion = RectRegion(edges4, "R_FarWake");
    farwakeRegion.MeshGen(Cedge19.m_N, Cedge20.m_N);
    farwakeRegion.transformation(farWakeAoA);
    farwakeRegion.Tec360Pts("testfarwake.dat");
    //////////////combine region//////////
    combinedReg.AddRegion(farwakeRegion);
    return 0;
}

int meshingInFoil(MeshRegions & nearWallRegion, MeshRegions &inFoilRegion, vector<vector<double> > &breakpts)
{
    std::vector<std::vector<int>> boundary = nearWallRegion.extractBoundaryPoints();
    int wallID = -1;
    for(int i=0; i<boundary.size(); ++i) {
        for(int j=0; j<boundary[i].size(); ++j) {
            if(fabs(nearWallRegion.m_pts[boundary[i][j]][0]) + fabs(nearWallRegion.m_pts[boundary[i][j]][1]) < 0.1) {
                wallID = i;
                break;
            }
        }
        if(wallID != -1) break;
    }
    int nWallPts = boundary[wallID].size();
    vector<int> layersInFoil(nWallPts, 0);

    for(int i=1; i<nWallPts-1; ++i) {
        if(nearWallRegion.m_pts[boundary[wallID][i]][0] < xInFoil) continue;
        double length = fabs(nearWallRegion.m_pts[boundary[wallID][i]][1]) - infoilRatio*fabs(nearWallRegion.m_pts[boundary[wallID][i+1]][0] - nearWallRegion.m_pts[boundary[wallID][i]][0]);
        if(length<hFirstLayerInFoil) continue;
        layersInFoil[i] = findNlayers(hFirstLayerInFoil, progressInFoil, length, maxLayerhInFoil) -1;
    }
    for(int i=0; i<2; ++i) {
        vector<double> p0(2, 100.);
        breakpts.push_back(p0);
    }
    for(int i=0; i<nWallPts-1; ++i) {
        int nl = min(layersInFoil[i], layersInFoil[i+1]);
        if(nl<=0) continue;
        vector<vector<double> > side0;
        vector<vector<double> > side1;
        vector<vector<double> > bottom0;
        vector<vector<double> > bottom1;
        double sig = -1.;
        if(nearWallRegion.m_pts[boundary[wallID][i]][1]<0.) sig = 1.;
        for(int k=0; k<=nl; ++k) {
            vector<double> p0(2, nearWallRegion.m_pts[boundary[wallID][i  ]][0]);
            vector<double> p1(2, nearWallRegion.m_pts[boundary[wallID][i+1]][0]);
            p0[1] = nearWallRegion.m_pts[boundary[wallID][i  ]][1] + sig*radiusEdgeInFoil(k*2./nLayersInFoil - 1.)[0];
            p1[1] = nearWallRegion.m_pts[boundary[wallID][i+1]][1] + sig*radiusEdgeInFoil(k*2./nLayersInFoil - 1.)[0];
            side0.push_back(p0);
            side1.push_back(p1);
        }
        bottom0.push_back(side0[0]);
        bottom0.push_back(side1[0]);
        bottom1.push_back(side0[nl]);
        bottom1.push_back(side1[nl]);
        int upperflap = 0;
        if(side0[nl][1]<0.) {
            upperflap = 1;
        }
        if(side0[nl][0]<breakpts[upperflap][0]) breakpts[upperflap] = side0[nl];
        if(side1[nl][0]<breakpts[upperflap][0]) breakpts[upperflap] = side1[nl];
        std::vector<std::vector<std::vector<double> > > edges;
        edges.push_back(bottom0);
        edges.push_back(side0);
        edges.push_back(bottom1);
        edges.push_back(side1);
        RectRegion pic = RectRegion(edges, "pic");
        pic.MeshGen(1, nl);
        nearWallRegion.AddRegion(pic);
        inFoilRegion.AddRegion(pic);
    }
    transform(breakpts[0], AoA);
    transform(breakpts[1], AoA);
    return 0;
}

int meshingInFoil_v2(MeshRegions & nearWallRegion, MeshRegions &inFoilRegion, vector<vector<double> > &breakpts)
{
    std::vector<std::vector<int>> boundary = nearWallRegion.extractBoundaryPoints();
    int wallID = -1;
    for(int i=0; i<boundary.size(); ++i) {
        for(int j=0; j<boundary[i].size(); ++j) {
            if(fabs(nearWallRegion.m_pts[boundary[i][j]][0]) + fabs(nearWallRegion.m_pts[boundary[i][j]][1]) < 0.1) {
                wallID = i;
                break;
            }
        }
        if(wallID != -1) break;
    }
    int nWallPts = boundary[wallID].size();
    vector<int> layersInFoil(nWallPts, 0);
    vector<vector<double> > wallnorm(nWallPts);
    
    for(int i=1; i<nWallPts-1; ++i) {
        vector<double> t1(2.);
        vector<double> t2(2.);
        vector<double> norm(2.);
        double l1, l2;
        t1[0] = nearWallRegion.m_pts[boundary[wallID][i  ]][0] - nearWallRegion.m_pts[boundary[wallID][i-1]][0];
        t1[1] = nearWallRegion.m_pts[boundary[wallID][i  ]][1] - nearWallRegion.m_pts[boundary[wallID][i-1]][1];
        t2[0] = nearWallRegion.m_pts[boundary[wallID][i+1]][0] - nearWallRegion.m_pts[boundary[wallID][i  ]][0];
        t2[1] = nearWallRegion.m_pts[boundary[wallID][i+1]][1] - nearWallRegion.m_pts[boundary[wallID][i  ]][1];
        l1 = t1[0]*t1[0] + t1[1]*t1[1];
        l2 = t2[0]*t2[0] + t2[1]*t2[1];
        norm[1] =-t1[0]/l1 - t2[0]/l2;
        norm[0] = t1[1]/l1 + t2[1]/l2;
        l1 = sqrt(norm[0]*norm[0] + norm[1]*norm[1]);
        norm[0] /= l1;
        norm[1] /= l1;
        wallnorm[i] = norm;
        if(nearWallRegion.m_pts[boundary[wallID][i]][0] < xInFoil) {
            layersInFoil[i] = 1;
        } else{
            double length = fabs(nearWallRegion.m_pts[boundary[wallID][i]][1]) - infoilRatio*fabs(nearWallRegion.m_pts[boundary[wallID][i+1]][0] - nearWallRegion.m_pts[boundary[wallID][i]][0]);
            if(length>=hFirstLayerInFoil) {
                layersInFoil[i] = findNlayers(hFirstLayerInFoil, progressInFoil, length, maxLayerhInFoil) -1;
            }
        }
    }
    std::vector<int> trailingedge;
    for(int i=nWallPts/2; i<nWallPts/2 + nWallPts;++i) {
        if(nearWallRegion.m_pts[boundary[wallID][i%nWallPts]][0] > 1. - 1.5*hTrailingEdge) {
            layersInFoil[i%nWallPts] = 0;
            trailingedge.push_back(boundary[wallID][i%nWallPts]);
        };
    }
    for(int i=0; i<2; ++i) {
        vector<double> p0(2, 100.);
        breakpts.push_back(p0);
    }
    for(int i=0; i<nWallPts-1; ++i) {
        int nl = min(layersInFoil[i], layersInFoil[i+1]);
        if(nl<=0) continue;
        vector<vector<double> > side0;
        vector<vector<double> > side1;
        vector<vector<double> > bottom0;
        vector<vector<double> > bottom1;
        double sig = 1.;
        for(int k=0; k<=nl; ++k) {
            vector<double> p0(2);
            vector<double> p1(2);
            p0[0] = nearWallRegion.m_pts[boundary[wallID][i  ]][0] + wallnorm[i  ][0]*sig*radiusEdgeInFoil(k*2./nLayersInFoil - 1.)[0];
            p0[1] = nearWallRegion.m_pts[boundary[wallID][i  ]][1] + wallnorm[i  ][1]*sig*radiusEdgeInFoil(k*2./nLayersInFoil - 1.)[0];
            p1[0] = nearWallRegion.m_pts[boundary[wallID][i+1]][0] + wallnorm[i+1][0]*sig*radiusEdgeInFoil(k*2./nLayersInFoil - 1.)[0];
            p1[1] = nearWallRegion.m_pts[boundary[wallID][i+1]][1] + wallnorm[i+1][1]*sig*radiusEdgeInFoil(k*2./nLayersInFoil - 1.)[0];
            side0.push_back(p0);
            side1.push_back(p1);
        }
        bottom0.push_back(side0[0]);
        bottom0.push_back(side1[0]);
        bottom1.push_back(side0[nl]);
        bottom1.push_back(side1[nl]);
        int upperflap = 0;
        if(side0[nl][1]<0.) {
            upperflap = 1;
        }
        if(side0[nl][0]<breakpts[upperflap][0]) breakpts[upperflap] = side0[nl];
        if(side1[nl][0]<breakpts[upperflap][0]) breakpts[upperflap] = side1[nl];
        std::vector<std::vector<std::vector<double> > > edges;
        edges.push_back(bottom0);
        edges.push_back(side0);
        edges.push_back(bottom1);
        edges.push_back(side1);
        RectRegion pic = RectRegion(edges, "pic");
        pic.MeshGen(1, nl);
        nearWallRegion.AddRegion(pic);
        inFoilRegion.AddRegion(pic);
    }
    int nround = trailingedge.size();
    double xc = 0.5*(nearWallRegion.m_pts[trailingedge[0]][0] + nearWallRegion.m_pts[trailingedge[nround-1]][0]);
    std::vector<double> center;
    center.push_back(xc); center.push_back(0.);
    for(int i=0; i<nround-1; ++i) {
        std::vector<std::vector<double>> tmp;
        tmp.push_back(nearWallRegion.m_pts[trailingedge[i+1]]);
        tmp.push_back(nearWallRegion.m_pts[trailingedge[i]]);
        tmp.push_back(center);
        MeshRegion pic("trig", tmp, false);
        nearWallRegion.AddRegion(pic);
        inFoilRegion.AddRegion(pic);
    }
    transform(breakpts[0], AoA);
    transform(breakpts[1], AoA);
    return 0;
}

int outputXML(MeshRegions &combinedReg, MeshRegions &inFoilRegion)
{
    //output outer region without wall for Omesh
    MeshRegions oRegion("Oreg", 1E-6);
    std::vector<std::vector<int>> boundary = combinedReg.extractBoundaryPoints();
    int wallID = -1;
    for(int i=0; i<boundary.size(); ++i) {
        for(int j=0; j<boundary[i].size(); ++j) {
            if(fabs(combinedReg.m_pts[boundary[i][j]][0]) + fabs(combinedReg.m_pts[boundary[i][j]][1]) < 0.1) {
                wallID = i;
                break;
            }
        }
        if(wallID != -1) break;
    }
    std::set<int> excludepts;
    for(int i=0; i<boundary[wallID].size(); ++i) {
        excludepts.insert(boundary[wallID][i]);
    }
    oRegion.AddRegion(combinedReg, excludepts);
    vector<int> comp2;
    comp2.push_back(0);
    //inlet
    oRegion.defineBoundary((void*)edge11,  Cedge11.m_N, 1);
    //outlet
    oRegion.defineBoundary((void*)edge9 ,   Cedge9.m_N, 2);
    //side
    oRegion.defineBoundary((void*)edge10,  Cedge10.m_N, 3);
    oRegion.defineBoundary((void*)edge12,  Cedge12.m_N, 4, 2, 0., -1);
    //output
    oRegion.outXml("outerRegion_Otip.xml");
    oRegion.outCOMPO("outerRegion_Otip.xml", comp2);

    //output outer region
    vector<int> comp3;
    comp3.push_back(0);
    //wall
    combinedReg.defineBoundary((void*)edge2, Cedge2.m_N, 0, curvedpts, AoA, 1);
    combinedReg.defineBoundary((void*)edge3, Cedge3.m_N, 0, curvedpts, AoA);
    combinedReg.defineBoundary((void*)edge4, Cedge4.m_N, 0, curvedpts, AoA);
    combinedReg.defineBoundary((void*)edge5, Cedge5.m_N, 0, curvedpts, AoA);
    combinedReg.defineBoundary((void*)edge6, Cedge6.m_N, 0, curvedpts, AoA, 1);
    combinedReg.defineBoundary((void*)edgeb2_b7, Cedgeb2_b7.m_N, 0,  curvedpts, AoA, 1);
    //inlet
    combinedReg.defineBoundary((void*)edge11,  Cedge11.m_N, 1);
    //outlet
    combinedReg.defineBoundary((void*)edge9 ,   Cedge9.m_N, 2);
    //side
    combinedReg.defineBoundary((void*)edge10,  Cedge10.m_N, 3);
    combinedReg.defineBoundary((void*)edge12,  Cedge12.m_N, 4, 2, 0., -1);
    //output
    combinedReg.outXml("outerRegion.xml");
    combinedReg.outCOMPO("outerRegion.xml", comp3);

    //in airfoil mesh
    vector<int> comp4;
    comp4.push_back(0);
    //wall
    inFoilRegion.defineBoundary((void*)edge2, Cedge2.m_N, 0, 2, AoA, 1);
    inFoilRegion.defineBoundary((void*)edge3, Cedge3.m_N, 0, 2, AoA);
    inFoilRegion.defineBoundary((void*)edge4, Cedge4.m_N, 0, 2, AoA);
    inFoilRegion.defineBoundary((void*)edge5, Cedge5.m_N, 0, 2, AoA);
    inFoilRegion.defineBoundary((void*)edge6, Cedge6.m_N, 0, 2, AoA, 1);
    inFoilRegion.defineBoundary((void*)edgeb2_b7, Cedgeb2_b7.m_N, 0, 2, AoA, 1);
    //output
    inFoilRegion.outXml("inFoil.xml");
    inFoilRegion.outCOMPO("inFoil.xml", comp4);
    return 0;
}

int outputGeo(MeshRegions &combinedReg, MeshRegions &nearWallRegion, vector<vector<double> > &breakpts,  MeshRegions &FarFieldReg, vector<double> pC)
{
    //generate gmsh geo file
    vector<vector<double>> box;
    std::vector<std::vector<int>> boundary = FarFieldReg.extractBoundaryPoints();
    int wallID = -1;
    for(int i=0; i<boundary.size(); ++i) {
        for(int j=0; j<boundary[i].size(); ++j) {
            if(fabs(FarFieldReg.m_pts[boundary[i][j]][0] - pC[0]) + fabs(FarFieldReg.m_pts[boundary[i][j]][1] - pC[1]) < 1E-6) {
                wallID = i;
                break;
            }
        }
        if(wallID != -1) break;
    }
    for(int i=0; i<boundary[wallID].size(); ++i) {
        vector<double> p0 = FarFieldReg.m_pts[boundary[wallID][i]];
        box.push_back(p0);
    }
    vector<double> center;
    center.push_back(0.); center.push_back(0.);

    //output outer inner region (no wall) for Omesh
    combinedReg.outOuterRegion("FarField.geo", box, center, .1, true);
    combinedReg.omeshBoundaryMapping("wallmapping.dat", center, 0.1);
    vector<vector<double>> nobox;
    //nearWallRegion.outOuterRegion("airfoil.geo",nobox, center, .1, false);
    nearWallRegion.outOuterRegion("airfoil.geo", nobox, center, .1, false);
    return 0;
}
