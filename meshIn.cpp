#include "CAD2D/BLMeshModule.h"
#include "CAD2D/CompositEdge.h"
#include "CAD2D/LineEdge.h"
#include "CAD2D/MeshRegions.h"
#include "CAD2D/MeshTool.h"
#include "CAD2D/RectRegion.h"
#include "CAD2D/util.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "edgefunctions.h"

// The ellipse wall is clockwise. Negative growth points into the pipe.
static vector<double> inwardRadiusEdge(double s) {
  vector<double> point = radiusEdge(s);
  point[0] = -point[0];
  return point;
}

int meshingInFoil(MeshRegions &region) {
  map<string, double> p;
  map<string, int> q;
  DefineBLParams(p, q);
  if (!isfinite(wallBLThickness0) || wallBLThickness0 < p["hFirstLayer"] ||
      wallBLThickness0 >= 0.5 * min(p["ChordLen"], p["Thickness"]) ||
      p["hFirstLayer"] <= 0. || p["progress"] < 1. ||
      p["maxLayerh"] < p["hFirstLayer"] || coreMeshSize < p["maxLayerh"] ||
      q["curvedpts"] < 3) {
    cerr << "Error: require first-layer <= BL thickness < pipe radius, growth >= 1, "
            "maxLayerh >= first-layer, coreMeshSize > maxLayerh and curvedpts >= 3\n";
    return -1;
  }
  int layers = findNlayers(p["hFirstLayer"], p["progress"], wallBLThickness0, p["maxLayerh"]);
  setRadiusMesh(p["hFirstLayer"], p["progress"], p["maxLayerh"]);
  setRadiusLayers(layers);
  // Keep complete layers with the prescribed growth; never squeeze the last
  // layer to fill the remaining thickness. Allow roundoff at an exact fit.
  if (radiusEdge(1.)[0] > wallBLThickness0 * (1. + 1.e-12))
    setRadiusLayers(--layers);
  double actualThickness = radiusEdge(1.)[0];
  const char *names[] = {"LE", "Up", "TE", "Low"};
  const char *counts[] = {"nLE", "nUp", "nTE", "nLow"};
  for (int i = 0; i < 4; ++i) {
    vector<void *> edges = {BLedges[i], (void *)inwardRadiusEdge, nullptr, nullptr};
    RectRegion block(edges, names[i], false);
    block.MeshGen(q[counts[i]], layers, eBoundaryLayer1);
    block.Tec360Pts(string(names[i]) + ".dat");
    region.AddRegion(block);
  }
  region.FixMesh();
  region.ResetBndPts();
  region.rebuildEdgesIndex();
  map<int, int> wallmapping;
  vector<int> wallPts;
  region.omeshBoundaryMapping(wallmapping, wallPts, "wallmapping.dat",
                              BLedge0(0.), p["hFirstLayer"] * 0.5);
  cout << "Boundary layer target thickness: " << wallBLThickness0
       << ", actual thickness: " << actualThickness << ", layers: " << layers
       << ", core target size: " << coreMeshSize << endl;
  return 0;
}

int main(int argc, char *argv[]) {
  bool merge = false;
  string mshfilename;
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "merge" && i + 1 < argc && !merge) {
      merge = true;
      mshfilename = argv[++i];
    } else {
      cerr << "Usage: meshIn [merge core.msh]\n";
      return 1;
    }
  }
  InitPts();
  MeshRegions inFoilRegion("RComb_", 1.E-8);
  if (meshingInFoil(inFoilRegion) != 0) return 1;
  if (!merge) {
    outputGeo(inFoilRegion, {1});
    int n = inFoilRegion.m_bndPts.size() / 2;
    ofstream geo("FarField0.geo", ios::app);
    geo.precision(17);
    geo << "Transfinite Line {" << n + 1 << ":" << 2 * n << "} = 2;\n"
        << "Mesh.CharacteristicLengthMin = " << coreMeshSize << ";\n"
        << "Mesh.CharacteristicLengthMax = " << coreMeshSize << ";\n"
        << "Mesh.CharacteristicLengthFromPoints = 0;\n"
        << "Mesh.CharacteristicLengthExtendFromBoundary = 0;\n"
        << "Mesh.RecombinationAlgorithm = 0;\n";
  } else {
    MeshRegions gmshReg("R_gmsh_", 1.E-8);
    if (gmshReg.loadFromMsh(mshfilename, 115. / 180. * M_PI) != 0 || gmshReg.m_cells.empty()) {
      cerr << "Error: could not load core mesh " << mshfilename << endl;
      return 1;
    }
    // The smaller loop is the core interface; check every node before merging.
    auto loops = inFoilRegion.extractBoundaryPoints();
    if (loops.size() != 2) return 1;
    auto loopRadius = [&](int i) {
      const auto &pt = inFoilRegion.m_pts[loops[i][0]];
      return hypot(pt[0] - BLModel->p["Tx0"], pt[1] - BLModel->p["Ty0"]);
    };
    vector<vector<double>> interfacePts;
    for (int id : loops[loopRadius(0) < loopRadius(1) ? 0 : 1])
      interfacePts.push_back(inFoilRegion.m_pts[id]);
    if (!gmshReg.consistancyCheck(interfacePts)) {
      cerr << "Error: core interface mismatch; use the same BL thickness in both stages\n";
      return 1;
    }
    inFoilRegion.AddRegion(gmshReg);
    inFoilRegion.ResetBndPts();
    inFoilRegion.rebuildEdgesIndex();
    BLModel->DefineBCs(inFoilRegion, 0, BLedges);
    inFoilRegion.CheckMesh();
    inFoilRegion.outXml("inFoil.xml");
    inFoilRegion.outCOMPO("inFoil.xml", {0});
  }
  return 0;
}
