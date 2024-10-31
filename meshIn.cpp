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
int meshingInFoil(MeshRegions &inFoilRegion, std::map<std::string, double> &p);
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
  std::map<std::string, double> dparams;
  meshingInFoil(inFoilRegion, dparams);
  if (!merge) {
    std::vector<int> OutLevels = {1};
    outputGeo(inFoilRegion, OutLevels);
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
    inFoilRegion.transformation(dparams["AoA"], 0., 0.);
    outputXML(inFoilRegion);
  }
  return 0;
}

int meshingInFoil(MeshRegions &inFoilRegion, std::map<std::string, double> &p) {
  // generate outer boundary layer mesh
  std::map<std::string, int> q;
  DefineBLParams(p, q);
  BLModel->MeshGen(inFoilRegion, BLedges);
  inFoilRegion.FixMesh();
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