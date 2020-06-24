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
    bool withwake = false;
    string mshfilename;
    for(int i=1; i<argc; ++i) {
        if(strcmp(argv[i], "merge")==0) {
            mshfilename = string(argv[i+1]);
            merge = true;
        }
        if(strcmp(argv[i], "wake")==0) {
            withwake = true;
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
    Rects[Rects.size()-1].MeshGen(CinnerEdge.m_N, CradiusEdge.m_N, eBoundaryLayer0, false);
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
    combinedReg.AddRegion(nearFieldReg);
    if(withwake) combinedReg.AddRegion(farwakeRegion);
    if(!merge) {
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
        combinedReg.outOuterRegion("airfoil.geo",nobox, center, .1, false);
        cout << "output CAD file" << endl;
        cout << "=======================================" << endl;
    }
    /////////////////////////////////////////////////////
    //////////////gmsh region, far field region//////////
    // step 2 import mesh
    if(merge) {
        MeshRegions gmshReg("R_gmsh_", 1.E-8);
        gmshReg.loadFromMsh(mshfilename);
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
        ///////
        vector<int> comp3;
        comp3.push_back(0);
        comp3.push_back(nearFieldReg.getCellsNumber());
        if(withwake) comp3.push_back(combinedReg.getCellsNumber());
        combinedReg.AddRegion(gmshReg);
        comp3.push_back(combinedReg.getCellsNumber());
        //wall
        combinedReg.defineBoundary((void*)edge2, Cedge2.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge3, Cedge3.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge4, Cedge4.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge5, Cedge5.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge6, Cedge6.m_N, 0, 12, AoA);
        combinedReg.defineBoundary((void*)edge8,  Cedge8.m_N,0,  2, AoA);
        //inlet
        combinedReg.defineBoundary((void*)edge11,  Cedge11.m_N, 1);
        //outlet
        combinedReg.defineBoundary((void*)edge9 ,   Cedge9.m_N, 2);
        //side
        combinedReg.defineBoundary((void*)edge10,  Cedge10.m_N, 3);
        combinedReg.defineBoundary((void*)edge12,  Cedge12.m_N, 4, 2, 0., -1);
        //output
        combinedReg.outXml("test.xml");
        combinedReg.outCOMPO("test.xml", comp3);
        cout << "------------------------------------" << endl;
        cout << "------------------------------------" << endl;
    }
    return 0;
}
