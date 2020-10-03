#include<iostream>
#include<vector>
#include<cmath>
#include<cstring>
#include"CAD2D/CompositEdge.h"
#include"CAD2D/MeshRegions.h"
#include"CAD2D/RectRegion.h"
#include"CAD2D/LineEdge.h"
#include"CAD2D/airfoil.h"
using namespace std;

#include"edgefunctions.h"
int meshingNearBody(MeshRegions &combinedReg);
int meshingWake(MeshRegions &combinedReg);
int meshingInFoil(MeshRegions &nearWallRegion, MeshRegions &inFoilRegion, vector<vector<double> > &breakpts);
int outputXML(MeshRegions &combinedReg, MeshRegions &inFoilRegion);
int outputGeo(MeshRegions &combinedReg, MeshRegions &nearWallRegion, vector<vector<double> > &breakpts);
int meshingInFoil_v2(MeshRegions & nearWallRegion, MeshRegions &inFoilRegion, vector<vector<double> > &breakpts);

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
    MeshRegions combinedReg("R_Comb_", 1.E-6);
    MeshRegions nearFieldReg("R_NearField_", 1.E-6);
    MeshRegions nearWallRegion("R_NearField_", 1.E-6);
    MeshRegions inFoilRegion("R_inFoil_", 1.E-6);
    InitPts();
    cout << "start meshing --------" << endl;
    meshingNearBody(combinedReg);
    nearWallRegion.AddRegion(combinedReg);
    combinedReg.transformation(AoA);
    if(withwake) meshingWake(combinedReg);
    vector<vector<double> > breakpts;
    meshingInFoil_v2(nearWallRegion, inFoilRegion, breakpts);
    nearWallRegion.transformation(AoA);
    inFoilRegion.transformation(AoA);

    if(!merge) {
        outputGeo(combinedReg, nearWallRegion, breakpts);
        cout << "output CAD file" << endl;
        cout << "=======================================" << endl;
    }
    /////////////////////////////////////////////////////
    //////////////gmsh region, far field region//////////
    // step 2 import mesh
    if(merge) {
        MeshRegions gmshReg("R_gmsh_", 1.E-8);
        gmshReg.loadFromMsh(mshfilename, 140./180.*3.14159);
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
        MeshRegions gmshInFoil1("R_gmsh1_", 1.E-8);
        gmshInFoil1.loadFromMsh(mshinfoilfilename1, 140./180.*3.14159);
        cout << "load " << mshinfoilfilename1 << endl;
        if(!nearWallRegion.consistancyCheck(gmshInFoil1)) {
            cout << "Error: node mismatch, exit" << endl;
            return -1;
        }
        inFoilRegion.AddRegion(gmshInFoil1);
        MeshRegions gmshInFoil2("R_gmsh2_", 1.E-8);
        gmshInFoil2.loadFromMsh(mshinfoilfilename2, 140./180.*3.14159);
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
    Rects.push_back(RectRegion(edges0, "R_near_wall", false));
    setRadiusLayers(nLayers2);
    Rects[Rects.size()-1].MeshGen(Cedge2.m_N, nLayers2, eBoundaryLayer1, false);
    Rects[Rects.size()-1].Tec360Pts("test2.dat");
    // edge 3
    edges0[0] = (void*)edge3;
    Rects.push_back(RectRegion(edges0, "R_near_wall", false));
    setRadiusLayers(nLayers3);
    Rects[Rects.size()-1].MeshGen(Cedge3.m_N, nLayers3, eBoundaryLayer1, false);
    // edge 4
    edges0[0] = (void*)edge4;
    Rects.push_back(RectRegion(edges0, "R_near_wall", false));
    setRadiusLayers(nLayers4);
    Rects[Rects.size()-1].MeshGen(Cedge4.m_N, nLayers4, eBoundaryLayer1, false);
    // edge 5
    edges0[0] = (void*)edge5;
    Rects.push_back(RectRegion(edges0, "R_near_wall", false));
    setRadiusLayers(nLayers5);
    Rects[Rects.size()-1].MeshGen(Cedge5.m_N, nLayers5, eBoundaryLayer1, false);
    // edge 6
    edges0[0] = (void*)edge6;
    Rects.push_back(RectRegion(edges0, "R_near_wall", false));
    setRadiusLayers(nLayers6);
    Rects[Rects.size()-1].MeshGen(Cedge6.m_N, nLayers6, eBoundaryLayer1, false);

    //give the normal direction, and points 12, 15
    vector<double> p1  = Rects[Rects.size()-1].getVertex(1);
    vector<double> n12 = Rects[Rects.size()-1].getVertex(2);
    n12[0] = n12[0] - p1[0]; n12[1] = n12[1] - p1[1];
    norm18[0] = n12[0]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    norm18[1] = n12[1]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    setRadiusLayers(nLayers7);
    pts[15][0] = p1[0] + norm18[0]*radiusEdge(1.)[0];
    pts[15][1] = p1[1] + norm18[1]*radiusEdge(1.)[0];
    pts[14][0] = pts[0][0] + norm18[0]*radiusEdge(1.)[0];
    pts[14][1] = 0. + norm18[1]*radiusEdge(1.)[0];
    p1  = Rects[0].getVertex(0);
    n12 = Rects[0].getVertex(3);
    n12[0] = n12[0] - p1[0]; n12[1] = n12[1] - p1[1];
    norm13[0] = n12[0]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    norm13[1] = n12[1]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    setRadiusLayers(nLayers1);
    pts[12][0] = p1[0] + norm13[0]*radiusEdge(1.)[0];
    pts[12][1] = p1[1] + norm13[1]*radiusEdge(1.)[0];
    pts[13][0] = pts[1][0] + norm13[0]*radiusEdge(1.)[0];
    pts[13][1] = 0. + norm13[1]*radiusEdge(1.)[0];
    //region 1
    std::vector<void*> edges1;
    edges1.push_back((void*)edge1);
    edges1.push_back((void*)edge0);
    edges1.push_back((void*)edge7);
    edges1.push_back((void*)edge8);
    Rects.push_back(RectRegion(edges1, "R_wake"));
    Rects[Rects.size()-1].MeshGen(Cedge1.m_N, Cedge0.m_N);
    //region 2
    std::vector<void*> edges2;
    edges2.push_back((void*)edge14);
    edges2.push_back((void*)edge15);
    edges2.push_back((void*)edge1);
    edges2.push_back((void*)edge13);
    Rects.push_back(RectRegion(edges2, "R_wake_Down"));
    Rects[Rects.size()-1].MeshGen(Cedge14.m_N, Cedge15.m_N);
    Rects[Rects.size()-1].Tec360Pts("test1.dat");
    //region 3
    std::vector<void*> edges3;
    edges3.push_back((void*)edge7);
    edges3.push_back((void*)edge16);
    edges3.push_back((void*)edge17);
    edges3.push_back((void*)edge18);
    Rects.push_back(RectRegion(edges3, "R_wake_Up"));
    Rects[Rects.size()-1].MeshGen(Cedge7.m_N, Cedge16.m_N);
    
    ///////////// combine the near field mesh
    for(unsigned int i=0; i<Rects.size(); ++i) {
    	combinedReg.AddRegion(Rects[i]);
    }
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
    //////////////combine region//////////
    combinedReg.AddRegion(farwakeRegion);
    return 0;
}

int meshingInFoil(MeshRegions & nearWallRegion, MeshRegions &inFoilRegion, vector<vector<double> > &breakpts)
{
    std::vector<std::vector<int>> boundary = nearWallRegion.extractBoundary();
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
    std::vector<std::vector<int>> boundary = nearWallRegion.extractBoundary();
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
    transform(breakpts[0], AoA);
    transform(breakpts[1], AoA);
    return 0;
}

int outputXML(MeshRegions &combinedReg, MeshRegions &inFoilRegion)
{
    vector<int> comp3;
    comp3.push_back(0);
    //wall
    combinedReg.defineBoundary((void*)edge2, Cedge2.m_N, 0, curvedpts, AoA);
    combinedReg.defineBoundary((void*)edge3, Cedge3.m_N, 0, curvedpts, AoA);
    combinedReg.defineBoundary((void*)edge4, Cedge4.m_N, 0, curvedpts, AoA);
    combinedReg.defineBoundary((void*)edge5, Cedge5.m_N, 0, curvedpts, AoA);
    combinedReg.defineBoundary((void*)edge6, Cedge6.m_N, 0, curvedpts, AoA);
    combinedReg.defineBoundary((void*)edge8,  Cedge8.m_N,0,  2, AoA);
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
    inFoilRegion.defineBoundary((void*)edge2, Cedge2.m_N, 0, curvedpts, AoA);
    inFoilRegion.defineBoundary((void*)edge3, Cedge3.m_N, 0, curvedpts, AoA);
    inFoilRegion.defineBoundary((void*)edge4, Cedge4.m_N, 0, curvedpts, AoA);
    inFoilRegion.defineBoundary((void*)edge5, Cedge5.m_N, 0, curvedpts, AoA);
    inFoilRegion.defineBoundary((void*)edge6, Cedge6.m_N, 0, curvedpts, AoA);
    inFoilRegion.defineBoundary((void*)edge8,  Cedge8.m_N,0,  2, AoA);
    //output
    inFoilRegion.outXml("inFoil.xml");
    inFoilRegion.outCOMPO("inFoil.xml", comp4);
    return 0;
}

int outputGeo(MeshRegions &combinedReg, MeshRegions &nearWallRegion, vector<vector<double> > &breakpts)
{
    vector<int> comp0;
    comp0.push_back(0); comp0.push_back(combinedReg.getCellsNumber());
    combinedReg.outXml("manMade.xml");
    combinedReg.outCOMPO("manMade.xml", comp0);
    //generate gmsh geo file
    CoutterEdge.addEdge(Cedge9, (void*)edge9);
    CoutterEdge.addEdge(Cedge10, (void*)edge10);
    CoutterEdge.addEdge(Cedge11, (void*)edge11);
    CoutterEdge.addEdge(Cedge12, (void*)edge12);
    vector<vector<double>> box;
    for(int i=0; i<CoutterEdge.m_N; ++i) {
        double tmps = -1. + i*2./CoutterEdge.m_N;
        vector<double> p0 = outterEdge(tmps);
        box.push_back(p0);
    }
    vector<double> center;
    center.push_back(0.); center.push_back(0.);
    combinedReg.outOuterRegion("FarField.geo", box, center, .1, true);
    vector<vector<double>> nobox;
    //nearWallRegion.outOuterRegion("airfoil.geo",nobox, center, .1, false);
    nearWallRegion.outOuterRegion("airfoil.geo", nobox, center, .1, false);
    return 0;
}
