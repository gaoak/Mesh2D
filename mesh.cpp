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
    edges0.push_back((void*)edge12);
    edges0.push_back((void*)edge25);
    edges0.push_back((void*)edge45);
    edges0.push_back((void*)edge14);
    RectRegion reg1(edges0, "reg1");
    reg1.MeshGen(Cedge12.m_N, Cedge25.m_N);

    edges0.clear();
    edges0.push_back((void*)edge01);
    edges0.push_back((void*)edge14);
    edges0.push_back((void*)edge34);
    edges0.push_back((void*)edge03);
    RectRegion reg0(edges0, "reg0");
    reg0.MeshGen(Cedge01.m_N, Cedge14.m_N);

    edges0.clear();
    edges0.push_back((void*)edge34);
    edges0.push_back((void*)edge47);
    edges0.push_back((void*)edge67);
    edges0.push_back((void*)edge36);
    RectRegion reg2(edges0, "reg2");
    reg2.MeshGen(Cedge34.m_N, Cedge47.m_N);

    edges0.clear();
    edges0.push_back((void*)edge45);
    edges0.push_back((void*)edge58);
    edges0.push_back((void*)edge78);
    edges0.push_back((void*)edge47);
    RectRegion reg3(edges0, "reg3");
    reg3.MeshGen(Cedge45.m_N, Cedge58.m_N);

    MeshRegions combinedReg("domain", 1E-6);
    combinedReg.AddRegion(reg1);
    combinedReg.AddRegion(reg0);
    combinedReg.AddRegion(reg2);
    combinedReg.AddRegion(reg3);

    vector<int> comp2;
    comp2.push_back(0);
    //inlet
    combinedReg.defineBoundary((void*)edge01,  Cedge01.m_N, 1);
    combinedReg.defineBoundary((void*)edge12,  Cedge12.m_N, 1);
    combinedReg.defineBoundary((void*)edge67 , Cedge67.m_N, 2);
    combinedReg.defineBoundary((void*)edge78 , Cedge78.m_N, 2);
    combinedReg.defineBoundary((void*)edge03,  Cedge03.m_N, 3);
    combinedReg.defineBoundary((void*)edge36,  Cedge36.m_N, 3);
    combinedReg.defineBoundary((void*)edge25,  Cedge25.m_N, 4);
    combinedReg.defineBoundary((void*)edge58,  Cedge58.m_N, 4);
    //output
    combinedReg.outXml("square.xml");
    combinedReg.outCOMPO("square.xml", comp2);
    return 0;
}