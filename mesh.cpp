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
    InitPts();
    std::vector<void*> edges0;
    edges0.push_back((void*)edge0);
    edges0.push_back((void*)edge1);
    edges0.push_back((void*)edge2);
    edges0.push_back((void*)edge3);
    RectRegion reg(edges0, "square");
    reg.MeshGen(Cedge0.m_N, Cedge1.m_N);
    reg.Tec360Pts("test2.dat");

    MeshRegions combinedReg("domain", 1E-6);
    combinedReg.AddRegion(reg);

    vector<int> comp2;
    comp2.push_back(0);
    //inlet
    combinedReg.defineBoundary((void*)edge0,  Cedge0.m_N, 1);
    combinedReg.defineBoundary((void*)edge1 , Cedge1.m_N, 2);
    combinedReg.defineBoundary((void*)edge2,  Cedge2.m_N, 3);
    combinedReg.defineBoundary((void*)edge3,  Cedge3.m_N, 4);
    //output
    combinedReg.outXml("square.xml");
    combinedReg.outCOMPO("square.xml", comp2);
    return 0;
}