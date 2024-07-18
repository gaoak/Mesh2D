#include "CAD2D/BLMeshModule.h"
#include "CAD2D/CompositEdge.h"
#include "CAD2D/LineEdge.h"
#include "CAD2D/MeshRegions.h"
#include "CAD2D/MeshTool.h"
#include "CAD2D/RectRegion.h"
#include "CAD2D/airfoil.h"
#include "CAD2D/util.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

#include "edgefunctions.h"
int meshingNearBody(MeshRegions &combinedReg);
int meshingBoundaryLayer(MeshRegions &combinedReg);
int outputXML(MeshRegions &combinedReg);
int main(int argc, char *argv[]) {
  bool merge = false, withwake = false;
  std::vector<string> mshfilename;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "merge") == 0) {
      merge = true;
      for (++i; i < argc; ++i) {
        std::string tmpname(argv[i]);
        int len = tmpname.length();
        if (len >= 4 && tmpname.substr(len - 4, 4) == ".msh") {
          mshfilename.push_back(argv[i]);
        } else {
          break;
        }
      }
    }
  }
  /// initialise
  InitPts();
  cout << "start meshing --------" << endl;
  MeshRegions combinedReg("RComb_", 1.E-6);
  meshingBoundaryLayer(combinedReg);
  meshingNearBody(combinedReg);
  if (!merge) {
    std::vector<int> OutLevels = {1};
    outputGeo(combinedReg, OutLevels);
    cout << "output CAD file" << endl;
    cout << "=======================================" << endl;
  } else {
    // load gmsh1
    for (auto file : mshfilename) {
      MeshRegions gmshReg("R_gmsh_", 1.E-8);
      gmshReg.loadFromMsh(file, 125. / 180. * 3.14159);
      cout << "load " << file << endl;
      if (!combinedReg.consistancyCheck(gmshReg)) {
        cout << "Error: node mismatch, exit" << endl;
        return -1;
      }
      if (!gmshReg.consistancyCheck(combinedReg)) {
        cout << "Error: node mismatch, exit" << endl;
        return -1;
      }
      combinedReg.AddRegion(gmshReg);
    }
    cout << "------------------------------------" << endl;
    outputXML(combinedReg);
    cout << "------------------------------------" << endl;
  }
  return 0;
}

int meshingBoundaryLayer(MeshRegions &combinedReg) {
  std::map<std::string, double> p;
  std::map<std::string, int> q;
  DefineBLParams(p, q);
  BLModel->MeshGen(combinedReg, BLedges);
  return 0;
}

int meshingNearBody(MeshRegions &combinedReg) {
  MeshRegions nearBodyRegion("Rnear_", 1e-8);
  vector<vector<double>> p = {{nearBoxLeft, nearBoxDown},
                              {nearBoxRight, nearBoxDown},
                              {nearBoxRight, nearBoxUp},
                              {nearBoxLeft, nearBoxUp}};
  vector<vector<vector<double>>> edges;
  // left
  vector<int> pi = {0, 1, 2, 3, 0};
  edges.clear();
  for (int i = 0; i < 4; ++i) {
    vector<vector<double>> edge;
    edge.push_back(p[pi[i]]);
    edge.push_back(p[pi[i + 1]]);
    edges.push_back(edge);
  }
  RectRegion pic0 = RectRegion(edges, "pic");
  pic0.MeshGen(nFarWakex, nFarWakey);
  nearBodyRegion.AddRegion(pic0);

  combinedReg.GetBoundBox(g_boundingbox);
  g_boundingbox.push_back(neargap);
  nearBodyRegion.RemoveElements((void *)toremove);
  //////////////combine region//////////
  combinedReg.AddRegion(nearBodyRegion);
  return 0;
}

int outputXML(MeshRegions &combinedReg) {
  // output outer region
  vector<int> comp3;
  comp3.push_back(0);
  // wall
  int offset = BLModel->DefineBCs(combinedReg, 0, BLedges);
  // inlet
  combinedReg.defineBoundary((void *)wake30, Cwake30.m_N, offset);
  // outlet
  combinedReg.defineBoundary((void *)wake12, Cwake12.m_N, 1+ offset);
  // side
  combinedReg.defineBoundary((void *)wake01, Cwake01.m_N, 2+ offset);
  combinedReg.defineBoundary((void *)wake23, Cwake23.m_N, 3+ offset);
  // output
  combinedReg.outXml("outerRegion.xml");
  combinedReg.outCOMPO("outerRegion.xml", comp3);
  return 0;
}