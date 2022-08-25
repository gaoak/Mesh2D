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
    edges0.push_back((void*)edge311);
    edges0.push_back((void*)edge1011);
    edges0.push_back((void*)edge010);
    RectRegion reg1(edges0, "reg1");
    reg1.MeshGen(Cedge03.m_N, Cedge010.m_N);

    edges0.clear();
    edges0.push_back((void*)edge45);
    edges0.push_back((void*)edge59);
    edges0.push_back((void*)edge69);
    edges0.push_back((void*)edge46);
    RectRegion reg2(edges0, "reg2");
    reg2.MeshGen(Cedge45.m_N, Cedge59.m_N);

    edges0.clear();
    edges0.push_back((void*)edge1012);
    edges0.push_back((void*)edge1214);
    edges0.push_back((void*)edge414);
    edges0.push_back((void*)edge104);
    RectRegion reg3(edges0, "reg3");
    reg3.MeshGen(Cedge1012.m_N, Cedge1214.m_N);

    edges0.clear();
    edges0.push_back((void*)edge1311);
    edges0.push_back((void*)edge115);
    edges0.push_back((void*)edge155);
    edges0.push_back((void*)edge1315);
    RectRegion reg4(edges0, "reg4");
    reg4.MeshGen(Cedge1311.m_N, Cedge1315.m_N);

    MeshRegions combinedReg("domain", 1E-6);
    combinedReg.AddRegion(reg1);
    combinedReg.AddRegion(reg2);
    combinedReg.AddRegion(reg3);
    combinedReg.AddRegion(reg4);

    vector<int> comp2;
    comp2.push_back(0);
    //inlet
    combinedReg.defineBoundary((void*)edge03,  Cedge03.m_N, 1);
    combinedReg.defineBoundary((void*)edge69 , Cedge69.m_N, 2);
    combinedReg.defineBoundary((void*)edge06,  Cedge06.m_N, 3);
    combinedReg.defineBoundary((void*)edge39,  Cedge39.m_N, 4);
    combinedReg.defineBoundary((void*)edgewall,  Cedgewall.m_N, 5);
    //output
    combinedReg.outXml("square.xml");
    combinedReg.outCOMPO("square.xml", comp2);
    return 0;
}