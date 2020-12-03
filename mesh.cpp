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
    edges0.push_back((void*)edge03);
    edges0.push_back((void*)edge39);
    edges0.push_back((void*)edge69);
    edges0.push_back((void*)edge06);
    RectRegion reg1(edges0, "reg1");
    reg1.MeshGen(Cedge03.m_N, Cedge06.m_N);

    MeshRegions combinedReg("domain", 1E-6);
    combinedReg.AddRegion(reg1);

    vector<int> comp2;
    comp2.push_back(0);
    //inlet
    combinedReg.defineBoundary((void*)edge03,  Cedge03.m_N, 1);
    combinedReg.defineBoundary((void*)edge69 , Cedge69.m_N, 2);
    combinedReg.defineBoundary((void*)edge06,  Cedge06.m_N, 3);
    combinedReg.defineBoundary((void*)edge39,  Cedge39.m_N, 4);
    //output
    combinedReg.outXml("square.xml");
    combinedReg.outCOMPO("square.xml", comp2);
    return 0;
}