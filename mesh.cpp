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

int main(int argc, char* argv[])
{
    bool merge = false;
    string mshinfoilfilename1;
    string mshinfoilfilename2;
    for(int i=1; i<argc; ++i) {
        if(strcmp(argv[i], "merge")==0) {
            mshinfoilfilename1 = string(argv[i+1]);
            mshinfoilfilename2 = string(argv[i+2]);
            merge = true;
        }
    }
    ////////////////////////////////////////
    cout << "start meshing --------" << endl;
    InitPts();
    CinnerEdge.addEdge(Cedge2, (void*)edge2);
    CinnerEdge.addEdge(Cedge3, (void*)edge3);
    CinnerEdge.addEdge(Cedge4, (void*)edge4);
    CinnerEdge.addEdge(Cedge5, (void*)edge5);
    CinnerEdge.addEdge(Cedge6, (void*)edge6);
    /////////////////////////////////////////
    /////////near body region////////////////
    std::vector<RectRegion> Rects;
    //boundary layer region 0
    std::vector<void*> edges0;
    edges0.push_back((void*)innerEdge);
    edges0.push_back((void*)radiusEdge);
    edges0.push_back((void*)edge0);
    edges0.push_back((void*)edge0);
    Rects.push_back(RectRegion(edges0, "R_near_wall", false));
    Rects[Rects.size()-1].MeshGen(CinnerEdge.m_N, CradiusEdge.m_N, eBoundaryLayer1, false);
    Rects[Rects.size()-1].Tec360Pts("test0.dat");
    
    //give the normal direction, and points 12, 15
    vector<double> p1  = Rects[Rects.size()-1].getVertex(1);
    vector<double> n12 = Rects[Rects.size()-1].getVertex(2);
    pts[15][0] = n12[0];
    pts[15][1] = n12[1];
    n12[0] = n12[0] - p1[0]; n12[1] = n12[1] - p1[1];
    norm18[0] = n12[0]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    norm18[1] = n12[1]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    p1  = Rects[Rects.size()-1].getVertex(0);
    n12 = Rects[Rects.size()-1].getVertex(3);
    pts[12][0] = n12[0];
    pts[12][1] = n12[1];
    n12[0] = n12[0] - p1[0]; n12[1] = n12[1] - p1[1];
    norm13[0] = n12[0]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
    norm13[1] = n12[1]/sqrt(n12[0]*n12[0] + n12[1]*n12[1]);
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
    //region 3
    std::vector<void*> edges3;
    edges3.push_back((void*)edge7);
    edges3.push_back((void*)edge16);
    edges3.push_back((void*)edge17);
    edges3.push_back((void*)edge18);
    Rects.push_back(RectRegion(edges3, "R_wake_Up"));
    Rects[Rects.size()-1].MeshGen(Cedge7.m_N, Cedge16.m_N);
    
    ///////////// combine the near field mesh
    MeshRegions nearFieldReg("R_NearField_", 1.E-6);
    for(unsigned int i=0; i<Rects.size(); ++i) {
    	nearFieldReg.AddRegion(Rects[i]);
    }
    nearFieldReg.transformation(AoA);
    /////////////////////////////////////////////////////
    //////////////far wake capture region//////////
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
    MeshRegions combinedReg("R_Comb_", 1.E-6);
    MeshRegions combinedRegwake("R_Comb_", 1.E-6);
    combinedReg.AddRegion(nearFieldReg);
    combinedRegwake.AddRegion(farwakeRegion);

    ///////////// generate inner airfoil boundary layer
    MeshRegions nearWallRegion("R_NearField_", 1.E-6);
    MeshRegions inFoilRegion("R_inFoil_", 1.E-6);
    for(unsigned int i=0; i<Rects.size(); ++i) {
        nearWallRegion.AddRegion(Rects[i]);
    }
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
        double length = fabs(nearWallRegion.m_pts[boundary[wallID][i]][1]) - 0.1*fabs(nearWallRegion.m_pts[boundary[wallID][i+1]][0] - nearWallRegion.m_pts[boundary[wallID][i]][0]);
        if(length<hFirstLayerInFoil) continue;
        layersInFoil[i] = findNlayers(hFirstLayerInFoil, progressInFoil, length, maxLayerhInFoil) -1;
    }
    vector<vector<double> > breakpts;
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
    nearWallRegion.transformation(AoA);
    inFoilRegion.transformation(AoA);
    transform(breakpts[0], AoA);
    transform(breakpts[1], AoA);

    if(!merge) {
        //generate gmsh geo file
        vector<double> center;
        center.push_back(0.); center.push_back(0.);
        vector<vector<double>> nobox;
        nearWallRegion.outInnerRegion("airfoil.geo", breakpts, center, .1);
        cout << "output CAD file" << endl;
        cout << "=======================================" << endl;
    }
    /////////////////////////////////////////////////////
    //////////////gmsh region, far field region//////////
    // step 2 import mesh
    if(merge) {
        vector<int> comp3;
        comp3.push_back(0);
        //wall
        combinedReg.defineBoundary((void*)edge2, Cedge2.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge3, Cedge3.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge4, Cedge4.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge5, Cedge5.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge6, Cedge6.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge8,  Cedge8.m_N,0,  2, AoA);
        //output
        combinedReg.outXml("outerRegion.xml");
        combinedReg.outCOMPO("outerRegion.xml", comp3);
        cout << "------------------------------------" << endl;
        cout << "------------------------------------" << endl;
        
        //wake
        combinedRegwake.outXml("wakeRegion.xml");
        combinedRegwake.outCOMPO("wakeRegion.xml", comp3);
        cout << "------------------------------------" << endl;
        cout << "------------------------------------" << endl;
        
        //in airfoil mesh
        MeshRegions gmshInFoil1("R_gmsh1_", 1.E-8);
        MeshRegions gmshInFoil2("R_gmsh2_", 1.E-8);
        gmshInFoil1.loadFromMsh(mshinfoilfilename1);
        gmshInFoil2.loadFromMsh(mshinfoilfilename2);
        cout << "load " << mshinfoilfilename1 << endl;
        cout << "load " << mshinfoilfilename2 << endl;
        if(!nearWallRegion.consistancyCheck(gmshInFoil1)) {
            cout << "Error: node mismatch, exit" << endl;
            return -1;
        }
        if(!nearWallRegion.consistancyCheck(gmshInFoil2)) {
            cout << "Error: node mismatch, exit" << endl;
            return -1;
        }
        inFoilRegion.AddRegion(gmshInFoil1);
        inFoilRegion.AddRegion(gmshInFoil2);
        ///////
        vector<int> comp4;
        comp4.push_back(0);
        //wall
        inFoilRegion.defineBoundary((void*)edge2, Cedge2.m_N, 0, 12, AoA);
        inFoilRegion.defineBoundary((void*)edge3, Cedge3.m_N, 0, 12, AoA);
        inFoilRegion.defineBoundary((void*)edge4, Cedge4.m_N, 0, 12, AoA);
        inFoilRegion.defineBoundary((void*)edge5, Cedge5.m_N, 0, 12, AoA);
        inFoilRegion.defineBoundary((void*)edge6, Cedge6.m_N, 0, 12, AoA);
        inFoilRegion.defineBoundary((void*)edge8,  Cedge8.m_N,0,  2, AoA);
        //output
        inFoilRegion.outXml("inFoil.xml");
        inFoilRegion.outCOMPO("inFoil.xml", comp4);
        cout << "------------------------------------" << endl;
        cout << "------------------------------------" << endl;
    }
    return 0;
}
