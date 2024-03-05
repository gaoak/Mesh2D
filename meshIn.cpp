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
int meshingInFoil(MeshRegions &inFoilRegion, MeshRegions &outFoilRegion);
int outputXML(MeshRegions &combinedReg);
int main(int argc, char *argv[]) {
  bool merge = false;
  std::vector<string> mshfilename;
  for (int i = 1; i < argc;) {
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
  MeshRegions inFoilRegion("RComb_", 1.E-6);
  MeshRegions outFoilRegion("RComb_", 1.E-6);
  meshingInFoil(inFoilRegion, outFoilRegion);
  if (!merge) {
    std::vector<int> OutLevels = {1};
    outputGeo(outFoilRegion, OutLevels);
    cout << "output CAD file" << endl;
    cout << "=======================================" << endl;
  } else {
    // load gmsh1
    for (auto file : mshfilename) {
      MeshRegions gmshReg("R_gmsh_", 1.E-8);
      gmshReg.loadFromMsh(file, 115. / 180. * 3.14159);
      cout << "load " << file << endl;
      if (!inFoilRegion.consistancyCheck(gmshReg)) {
        cout << "Error: node mismatch, exit" << endl;
        return -1;
      }
      if (!gmshReg.consistancyCheck(inFoilRegion)) {
        cout << "Error: node mismatch, exit" << endl;
        return -1;
      }
      inFoilRegion.AddRegion(gmshReg);
    }
  }
  outputXML(inFoilRegion);
  return 0;
}

int meshingInFoil(MeshRegions &inFoilRegion, MeshRegions &outFoilRegion) {
  // generate outer boundary layer mesh
  std::map<std::string, double> p;
  std::map<std::string, int> q;
  DefineBLParams(p, q);
  BLModel->MeshGen(outFoilRegion, BLedges);
  // output wall mapping from wall to one layer offset
  std::map<int, int> wallmapping;
  std::vector<int> wallPts;
  vector<double> center;
  center.push_back(0.);
  center.push_back(0.);
  outFoilRegion.omeshBoundaryMapping(wallmapping, wallPts, "wallmapping.dat",
                                     center, 0.02);
  //
  int npts = wallPts.size();
  for (int i = 0; i < npts; ++i) {
    int pid0 = wallPts[i], pid1 = wallPts[(1 + i) % npts];
    vector<double> pA = outFoilRegion.m_pts[pid0];
    vector<double> pA1 = outFoilRegion.m_pts[wallmapping[pid0]];
    vector<double> pB = outFoilRegion.m_pts[pid1];
    vector<double> pB1 = outFoilRegion.m_pts[wallmapping[pid1]];
    vector<double> pD(2, 0.), pC(2, 0.);
    for (int d = 0; d < 2; ++d) {
      pC[d] = 2. * pB[d] - pB1[d];
      pD[d] = 2. * pA[d] - pA1[d];
    }
    if (pA[1] * pD[1] < 0. || pB[1] * pC[1] < 0.)
      continue;
    else if (pA[1] * pB[1] >= 0.) {
      double d = p["hFirstLayer"] *
                 fabs((pA[1] + pB[1]) / (pA1[1] - pA[1] + pB1[1] - pB[1]));
      if (d < 1.5 * p["hFirstLayer"])
        continue;
    }
    vector<vector<double>> sidel;
    vector<vector<double>> sider;
    vector<vector<double>> bottom;
    vector<vector<double>> ceil;
    bottom.push_back(pD);
    bottom.push_back(pC);
    sider.push_back(pC);
    sider.push_back(pB);
    ceil.push_back(pB);
    ceil.push_back(pA);
    sidel.push_back(pA);
    sidel.push_back(pD);

    std::vector<std::vector<std::vector<double>>> edges;
    edges.push_back(bottom);
    edges.push_back(sider);
    edges.push_back(ceil);
    edges.push_back(sidel);
    RectRegion pic = RectRegion(edges, "pic");
    pic.MeshGen(1, 1);
    outFoilRegion.AddRegion(pic);
    inFoilRegion.AddRegion(pic);
  }
  return 0;
}

int outputXML(MeshRegions &combinedReg) {
  // output outer region
  vector<int> comp3;
  comp3.push_back(0);
  combinedReg.outXml("inFoil.xml");
  combinedReg.outCOMPO("inFoil.xml", comp3);
  return 0;
}