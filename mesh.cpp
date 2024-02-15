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
int outputGeo(MeshRegions &combinedReg, std::vector<int> OutLevels);
int meshingOuterBoundary(MeshRegions &combinedReg);
int main(int argc, char *argv[]) {
  bool merge = false;
  string mshfilename0, mshfilename1;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "merge") == 0) {
      if (argc > i + 1)
        mshfilename0 = string(argv[i + 1]);
      if (argc > i + 2)
        mshfilename1 = string(argv[i + 2]);
      merge = true;
    }
  }
  /// initialise
  InitPts();
  cout << "start meshing --------" << endl;
  MeshRegions combinedReg("RComb_", 1.E-6);
  meshingBoundaryLayer(combinedReg);
  meshingNearBody(combinedReg);
  meshingOuterBoundary(combinedReg);
  if (!merge) {
    std::vector<int> OutLevels = {1, 3};
    outputGeo(combinedReg, OutLevels);
    cout << "output CAD file" << endl;
    cout << "=======================================" << endl;
  } else {
    // load gmsh1
    MeshRegions gmshReg("R_gmsh_", 1.E-8);
    gmshReg.loadFromMsh(mshfilename0, 130. / 180. * 3.14159);
    cout << "load " << mshfilename0 << endl;
    if (!combinedReg.consistancyCheck(gmshReg)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    if (!gmshReg.consistancyCheck(combinedReg)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    combinedReg.AddRegion(gmshReg);
    // load gmsh2
    MeshRegions gmshReg2("R_gmsh_", 1.E-8);
    gmshReg2.loadFromMsh(mshfilename1, 120. / 180. * 3.14159);
    cout << "load " << mshfilename1 << endl;
    if (!combinedReg.consistancyCheck(gmshReg2)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    if (!gmshReg2.consistancyCheck(combinedReg)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    combinedReg.AddRegion(gmshReg2);
    outputXML(combinedReg);
    cout << "------------------------------------" << endl;
    cout << "------------------------------------" << endl;
  }
  return 0;
}

int meshingNearBody(MeshRegions &combinedReg) {
  setRadiusMesh(2.*M_PI*radiusNeari/Nnear, 1.01, 2.*M_PI*radiusNeari/Nnear);
  std::vector<void *> edges0;
  void *edgetmp;
  // edge 11-0
  edges0.push_back((void *)edge3);
  edges0.push_back((void *)radiusEdge);
  edges0.push_back(edgetmp);
  edges0.push_back(edgetmp);
  RectRegion Rect(RectRegion(edges0, "cyl3", false));
  setRadiusLayers(1);
  Rect.MeshGen(Nnear, 1, eBoundaryLayer1);
  Rect.Tec360Pts("cyl3.dat");
  combinedReg.AddRegion(Rect);
  return 0;
}

int meshingBoundaryLayer(MeshRegions &combinedReg) {
  setRadiusMesh(hFirstLayer, progress, maxBLSize);
  /////////near body region////////////////
  std::vector<RectRegion> Rects;
  // boundary layer region 0
  std::vector<void *> edges0;
  void *edgetmp;
  // edge 11-0
  edges0.push_back((void *)edge0);
  edges0.push_back((void *)radiusEdge);
  edges0.push_back(edgetmp);
  edges0.push_back(edgetmp);
  Rects.push_back(RectRegion(edges0, "cyl0", false));
  setRadiusLayers(nBLayers);
  Rects[Rects.size() - 1].MeshGen(Ncylinder, nBLayers, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("cyl0.dat");
  // edge 01
  edges0[0] = (void *)edge1;
  Rects.push_back(RectRegion(edges0, "cyl1", false));
  setRadiusLayers(nBLayers);
  Rects[Rects.size() - 1].MeshGen(Ncylinder, nBLayers, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("cyl1.dat");
  // edge 12
  edges0[0] = (void *)edge2;
  Rects.push_back(RectRegion(edges0, "cyl2", false));
  setRadiusLayers(nBLayers);
  Rects[Rects.size() - 1].MeshGen(Ncylinder, nBLayers, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("cyl2.dat");
  ///////////// combine the near field mesh
  for (unsigned int i = 0; i < Rects.size(); ++i) {
    combinedReg.AddRegion(Rects[i]);
  }
  return 0;
}

int meshingOuterBoundary(MeshRegions &combinedReg) {
  setRadiusMesh(2.*M_PI*radiusFar/Nfar, 1.1, 4.*M_PI*radiusFar/Nfar);
  std::vector<void *> edges0;
  void *edgetmp;
  // edge 11-0
  edges0.push_back((void *)edge4);
  edges0.push_back((void *)radiusEdge);
  edges0.push_back(edgetmp);
  edges0.push_back(edgetmp);
  RectRegion Rect(RectRegion(edges0, "cyl4", false));
  setRadiusLayers(1);
  Rect.MeshGen(Nfar, 1, eBoundaryLayer1);
  Rect.Tec360Pts("cyl4.dat");
  combinedReg.AddRegion(Rect);
  return 0;
}

int outputGeo(MeshRegions &combinedReg, std::vector<int> OutLevels) {
  // outer layer
  std::vector<std::vector<int>> boundary = combinedReg.extractBoundaryPoints();
  vector<vector<vector<double>>> boxes(boundary.size());
  for (int i = 0; i < boundary.size(); ++i) {
    for (int j = 0; j < boundary[i].size(); ++j) {
      boxes[i].push_back(combinedReg.m_pts[boundary[i][j]]);
    }
  }
  std::map<int, std::set<int>> trees;
  std::set<int> roots;
  BuildTopoTree(boxes, trees, roots);
  std::vector<int> levels(boxes.size(), 0);
  int r0 = *roots.begin();
  FindTreesDepths(r0, 0, trees, levels);
  int filen = 0;
  for (auto l : OutLevels) {
    for (size_t i = 0; i < levels.size(); ++i) {
      if (l == levels[i]) {
        std::vector<std::vector<std::vector<double>>> tmparray;
        for (auto p : trees[i]) {
          tmparray.push_back(boxes[p]);
        }
        OutGeo("FarField" + to_string(filen) + ".geo", boxes[i], tmparray);
        ++filen;
      }
    }
  }
  return 0;
}

int outputXML(MeshRegions &combinedReg) {
  // output outer region
  vector<int> comp3;
  comp3.push_back(0);
  // wall
  combinedReg.defineBoundary((void *)edge0, Ncylinder, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge1, Ncylinder, 1, curvedpts);
  combinedReg.defineBoundary((void *)edge2, Ncylinder, 2, curvedpts);
  combinedReg.defineBoundary((void *)edge4, Nfar, 3);
  // output
  combinedReg.outXml("outerRegion.xml");
  combinedReg.outCOMPO("outerRegion.xml", comp3);
  return 0;
}