#include<iostream>
#include<vector>
#include<cmath>
#include<cstring>
#include"CAD2D/CompositEdge.h"
#include"CAD2D/MeshRegions.h"
#include"CAD2D/RectRegion.h"
#include"CAD2D/LineEdge.h"
#include"CAD2D/airfoil.h"
#include"CAD2D/util.h"
using namespace std;

#include"edgefunctions.h"
int main(int argc, char* argv[])
{
    std::vector<void*> edges0;
    edges0.push_back((void*)edge03);
    edges0.push_back((void*)edge39);
    edges0.push_back((void*)edge69);
    edges0.push_back((void*)edge06);
    RectRegion reg1(edges0, "reg1");
    reg1.MeshGen(Nx, Ny);

    MeshRegions combinedReg("domain", 1E-6);
    combinedReg.AddRegion(reg1);

    vector<int> comp2;
    comp2.push_back(0);
    // Keep the existing bottom, top, left and right boundary numbering.
    combinedReg.defineBoundary((void*)edge03, Nx, 1);
    combinedReg.defineBoundary((void*)edge69, Nx, 2);
    combinedReg.defineBoundary((void*)edge06, Ny, 3);
    combinedReg.defineBoundary((void*)edge39, Ny, 4);
    //output
    combinedReg.outXml("square.xml");
    combinedReg.outCOMPO("square.xml", comp2);
    return 0;
}
