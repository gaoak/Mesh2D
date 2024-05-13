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
int meshingInFoil(MeshRegions &inFoilRegion, MeshRegions &outFoilRegion,
                  std::map<std::string, double> &p);
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
  std::map<std::string, double> dparams;
  meshingInFoil(inFoilRegion, outFoilRegion, dparams);
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
      if (!outFoilRegion.consistancyCheck(gmshReg)) {
        cout << "Error: node mismatch, exit" << endl;
        return -1;
      }
      if (!gmshReg.consistancyCheck(outFoilRegion)) {
        cout << "Error: node mismatch, exit" << endl;
        return -1;
      }
      inFoilRegion.AddRegion(gmshReg);
    }
    inFoilRegion.transformation(dparams["AoA"], 0., 0.);
    outputXML(inFoilRegion);
  }
  return 0;
}

int meshingInFoil(MeshRegions &inFoilRegion, MeshRegions &outFoilRegion,
                  std::map<std::string, double> &p) {
  // generate outer boundary layer mesh
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
  outFoilRegion.transformation(-p["AoA"], 0., 0.);
  double TEx = 1. - 0.5 * p["TEThickness"];
  std::set<int> TEpts;
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
    if (pA[0] >= TEx) {
      TEpts.insert(i);
    }
    if (pA[1] * pD[1] < 0. || pB[1] * pC[1] < 0.)
      continue;
    else if (pA[1] * pB[1] >= 0.) {
      double d = p["hFirstLayer"] *
                 fabs((pA[1] + pB[1]) / (pA1[1] - pA[1] + pB1[1] - pB[1]));
      if (d < 2. * p["hFirstLayer"])
        continue;
    }
    std::vector<std::vector<double>> pts;
    pts.push_back(pA);
    pts.push_back(pB);
    pts.push_back(pC);
    pts.push_back(pD);
    outFoilRegion.AddElement(pts);
    inFoilRegion.AddElement(pts);
  }
  if (p["TEThickness"] * 0.25 < p["hFirstLayer"] && TEpts.size() > 1) {
    int i0 = *TEpts.begin(), np = TEpts.size() - 1;
    vector<double> pC(2, 0.);
    pC[0] = p["ChordLen"] - 0.5 * p["TEThickness"];
    for (int i = 0; i < np; ++i) {
      int pid0 = wallPts[i + i0], pid1 = wallPts[1 + i + i0];
      vector<double> pA = outFoilRegion.m_pts[pid0];
      vector<double> pB = outFoilRegion.m_pts[pid1];
      std::vector<std::vector<double>> pts;
      pts.push_back(pA);
      pts.push_back(pB);
      pts.push_back(pC);
      outFoilRegion.AddElement(pts);
      inFoilRegion.AddElement(pts);
    }
  }
  outFoilRegion.FixMesh();
  inFoilRegion.FixMesh();
  outFoilRegion.ResetBndPts();
  inFoilRegion.ResetBndPts();
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