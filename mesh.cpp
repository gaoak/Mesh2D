#include "CAD2D/CompositEdge.h"
#include "CAD2D/LineEdge.h"
#include "CAD2D/MeshRegions.h"
#include "CAD2D/RectRegion.h"
#include "CAD2D/SplineEdge.h"
#include "CAD2D/airfoil.h"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "edgefunctions.h"

int outputGeo()
{
  // bottom region
  std::vector<void *> edges0;
  edges0.push_back((void *)edge4);
  edges0.push_back((void *)edge5);
  edges0.push_back((void *)edge6);
  edges0.push_back((void *)edge7);
  RectRegion Rect(edges0, "Nozzel", false);
  Rect.MeshGen(Cedge4.m_N, Cedge5.m_N);
  Rect.Tec360Pts("test0.dat");

  MeshRegions combinedReg("Combine", 3.E-5);
  combinedReg.AddRegion(Rect);

  vector<vector<double>> box;
  for(int i=0; i<=nBoxDown; ++i) {
    double x = xBoxLeft + i * (xBoxRight - xBoxLeft) / nBoxDown;
    double y = vpts[0][1];
    vector<double> p{x, y};
    box.push_back(p);
  }
  for(int i=1; i<Cedge1.m_N; ++i) {
    box.push_back(edge1(-1. + 2./Cedge1.m_N*i));
  }
  vector<double> p{pts[2][0], pts[2][1]};
  box.push_back(p);
  p[0] = pts[3][0]; p[1] = pts[3][1];
  box.push_back(p);
  for(int i=1; i<Cedge3.m_N; ++i) {
    box.push_back(edge3(1. - 2./Cedge3.m_N*i));
  }
    vector<double> center;
    center.push_back(vpts[0][0]); center.push_back(vpts[0][1]);
  combinedReg.outOuterRegion("FarField.geo", box, center, .1, true);
  return 0;
}

int outputXML(MeshRegions &combinedReg)
{
    //output outer region without wall for Omesh
    combinedReg.CheckMesh();
    //
    vector<void*> conditions;
    conditions.push_back((void*)bottomwall);
    conditions.push_back((void*)nozzleoutlet);
    conditions.push_back((void*)leftside);
    conditions.push_back((void*)rightside);
    conditions.push_back((void*)upperside);
    conditions.push_back((void*)nozzlewall);
    conditions.push_back((void*)tool);
    combinedReg.defineBoundary(conditions, 80./180*M_PI);
    //output outer region
    vector<int> comp3;
    comp3.push_back(0);
    //output
    combinedReg.outXml("outerRegion.xml");
    combinedReg.outCOMPO("outerRegion.xml", comp3);

    return 0;
}

int main(int argc, char *argv[]) {
  bool merge = false;
  vector<string> mshfilenames;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "merge") == 0) {
      for (int j=i+1; j<argc; ++j) {
        mshfilenames.push_back(string(argv[j]));
      }
      merge = true;
      break;
    }
  }
  if(merge) {
    MeshRegions gmshReg("R_gmsh_", 3.E-5);
    gmshReg.loadFromMsh(mshfilenames[0], 135./180.*3.14159);
    cout << "load " << mshfilenames[0] << endl;
    gmshReg.CheckMesh();
    outputXML(gmshReg);
  }
  return 0;
}