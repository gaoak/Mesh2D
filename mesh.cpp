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
int meshingWake(MeshRegions &combinedReg);
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
  meshingWake(combinedReg);
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

int meshingBoundaryLayer(MeshRegions &combinedReg) {
  /////////near body region////////////////
  std::vector<RectRegion> Rects;
  // boundary layer region 0
  std::vector<void *> edges0;
  void *edge0;
  // edge 11-0
  edges0.push_back((void *)edge110);
  edges0.push_back((void *)radiusEdge);
  edges0.push_back(edge0);
  edges0.push_back(edge0);
  Rects.push_back(RectRegion(edges0, "up0", false));
  setRadiusLayers(nLayersU0);
  Rects[Rects.size() - 1].MeshGen(Cedge110.m_N, nLayersU0, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("up0.dat");
  // edge 01
  edges0[0] = (void *)edge01;
  Rects.push_back(RectRegion(edges0, "up1", false));
  setRadiusLayers(nLayersU1);
  Rects[Rects.size() - 1].MeshGen(Cedge01.m_N, nLayersU1, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("up1.dat");
  // edge 12
  edges0[0] = (void *)edge12;
  Rects.push_back(RectRegion(edges0, "up2", false));
  setRadiusLayers(nLayersU2);
  Rects[Rects.size() - 1].MeshGen(Cedge12.m_N, nLayersU2, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("up2.dat");
  // edge 23
  edges0[0] = (void *)edge23;
  Rects.push_back(RectRegion(edges0, "up3", false));
  setRadiusLayers(nLayersU3);
  Rects[Rects.size() - 1].MeshGen(Cedge23.m_N, nLayersU3, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("up3.dat");
  // edge 34
  edges0[0] = (void *)edge34;
  Rects.push_back(RectRegion(edges0, "up4", false));
  setRadiusLayers(nLayersU4);
  Rects[Rects.size() - 1].MeshGen(Cedge34.m_N, nLayersU4, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("up4.dat");
  // edge 45
  edges0[0] = (void *)edge45;
  Rects.push_back(RectRegion(edges0, "up5", false));
  setRadiusLayers(nLayersU5);
  Rects[Rects.size() - 1].MeshGen(Cedge45.m_N, nLayersU5, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("up5.dat");

  // edge 5-6
  edges0[0] = (void *)edge56;
  Rects.push_back(RectRegion(edges0, "low5", false));
  setRadiusLayers(nLayersL5);
  Rects[Rects.size() - 1].MeshGen(Cedge56.m_N, nLayersL5, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("low5.dat");
  // edge 6-7
  edges0[0] = (void *)edge67;
  Rects.push_back(RectRegion(edges0, "low4", false));
  setRadiusLayers(nLayersL4);
  Rects[Rects.size() - 1].MeshGen(Cedge67.m_N, nLayersL4, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("low4.dat");
  // edge 7-8
  edges0[0] = (void *)edge78;
  Rects.push_back(RectRegion(edges0, "low3", false));
  setRadiusLayers(nLayersL3);
  Rects[Rects.size() - 1].MeshGen(Cedge78.m_N, nLayersL3, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("low3.dat");
  // edge 8-9
  edges0[0] = (void *)edge89;
  Rects.push_back(RectRegion(edges0, "low2", false));
  setRadiusLayers(nLayersL2);
  Rects[Rects.size() - 1].MeshGen(Cedge89.m_N, nLayersL2, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("low2.dat");
  // edge 9-10
  edges0[0] = (void *)edge910;
  Rects.push_back(RectRegion(edges0, "low1", false));
  setRadiusLayers(nLayersL1);
  Rects[Rects.size() - 1].MeshGen(Cedge910.m_N, nLayersL1, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("low1.dat");
  // edge 10-11
  edges0[0] = (void *)edge1011;
  Rects.push_back(RectRegion(edges0, "low0", false));
  setRadiusLayers(nLayersL0);
  Rects[Rects.size() - 1].MeshGen(Cedge1011.m_N, nLayersL0, eBoundaryLayer1);
  Rects[Rects.size() - 1].Tec360Pts("low0.dat");

  ///////////// combine the near field mesh
  for (unsigned int i = 0; i < Rects.size(); ++i) {
    combinedReg.AddRegion(Rects[i]);
  }
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
  pic0.MeshGen(int((nearBoxRight - nearBoxLeft) / maxLayerh + 0.5),
               int((nearBoxUp - nearBoxDown) / maxLayerh + 0.5));
  nearBodyRegion.AddRegion(pic0);
  nearBodyRegion.transformation(nearAoA);

  combinedReg.GetBoundBox(g_boundingbox);
  g_boundingbox.push_back(maxLayerh * 2.);
  nearBodyRegion.RemoveElements((void *)toremove);
  //////////////combine region//////////
  combinedReg.AddRegion(nearBodyRegion);
  return 0;
}

int meshingWake(MeshRegions &combinedReg) {
  std::vector<void *> edges4;
  edges4.push_back((void *)wake01);
  edges4.push_back((void *)wake12);
  edges4.push_back((void *)wake23);
  edges4.push_back((void *)wake30);
  RectRegion farwakeRegion = RectRegion(edges4, "R_FarWake");
  farwakeRegion.MeshGen(Cwake01.m_N, Cwake12.m_N);
  farwakeRegion.Tec360Pts("farwake.dat");
  //////////////combine region//////////
  combinedReg.AddRegion(farwakeRegion);
  return 0;
}

int meshingOuterBoundary(MeshRegions &combinedReg) {
  vector<int> pts;
  vector<vector<vector<double>>> edges;
  // left
  pts = {0, 1, 9, 8, 0};
  edges.clear();
  for (int i = 0; i < 4; ++i) {
    vector<double> p0 = {g_ptsF[pts[i]][0], g_ptsF[pts[i]][1]};
    vector<double> p1 = {g_ptsF[pts[i + 1]][0], g_ptsF[pts[i + 1]][1]};
    vector<vector<double>> edge;
    edge.push_back(p0);
    edge.push_back(p1);
    edges.push_back(edge);
  }
  RectRegion pic0 = RectRegion(edges, "pic");
  pic0.MeshGen(1, nBoxLeft);
  pic0.Tec360Pts("pic0.dat");
  combinedReg.AddRegion(pic0);
  // right
  pts = {2, 3, 11, 10, 2};
  edges.clear();
  for (int i = 0; i < 4; ++i) {
    vector<double> p0 = {g_ptsF[pts[i]][0], g_ptsF[pts[i]][1]};
    vector<double> p1 = {g_ptsF[pts[i + 1]][0], g_ptsF[pts[i + 1]][1]};
    vector<vector<double>> edge;
    edge.push_back(p0);
    edge.push_back(p1);
    edges.push_back(edge);
  }
  RectRegion pic1 = RectRegion(edges, "pic");
  pic1.MeshGen(1, nBoxLeft);
  pic1.Tec360Pts("pic1.dat");
  combinedReg.AddRegion(pic1);
  // upper
  pts = {6, 7, 10, 9, 6};
  edges.clear();
  for (int i = 0; i < 4; ++i) {
    vector<double> p0 = {g_ptsF[pts[i]][0], g_ptsF[pts[i]][1]};
    vector<double> p1 = {g_ptsF[pts[i + 1]][0], g_ptsF[pts[i + 1]][1]};
    vector<vector<double>> edge;
    edge.push_back(p0);
    edge.push_back(p1);
    edges.push_back(edge);
  }
  RectRegion pic2 = RectRegion(edges, "pic");
  pic2.MeshGen(nBoxDown - 2, 1);
  pic2.Tec360Pts("pic2.dat");
  combinedReg.AddRegion(pic2);
  // lower
  pts = {1, 2, 5, 4, 1};
  edges.clear();
  for (int i = 0; i < 4; ++i) {
    vector<double> p0 = {g_ptsF[pts[i]][0], g_ptsF[pts[i]][1]};
    vector<double> p1 = {g_ptsF[pts[i + 1]][0], g_ptsF[pts[i + 1]][1]};
    vector<vector<double>> edge;
    edge.push_back(p0);
    edge.push_back(p1);
    edges.push_back(edge);
  }
  RectRegion pic3 = RectRegion(edges, "pic");
  pic3.MeshGen(nBoxDown - 2, 1);
  pic3.Tec360Pts("pic3.dat");
  combinedReg.AddRegion(pic3);
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
  combinedReg.defineBoundary((void *)edge45, Cedge45.m_N, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge56, Cedge56.m_N, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge1011, Cedge1011.m_N, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge110, Cedge110.m_N, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge01, Cedge01.m_N, 5);
  combinedReg.defineBoundary((void *)edge12, Cedge12.m_N, 5);
  combinedReg.defineBoundary((void *)edge23, Cedge23.m_N, 5);
  combinedReg.defineBoundary((void *)edge34, Cedge34.m_N, 5);
  combinedReg.defineBoundary((void *)edge67, Cedge67.m_N, 6);
  combinedReg.defineBoundary((void *)edge78, Cedge78.m_N, 6);
  combinedReg.defineBoundary((void *)edge89, Cedge89.m_N, 6);
  combinedReg.defineBoundary((void *)edge910, Cedge910.m_N, 6);
  // inlet
  combinedReg.defineBoundary((void *)far08, Cfar08.m_N, 1);
  // outlet
  combinedReg.defineBoundary((void *)far311, Cfar311.m_N, 2);
  // side
  combinedReg.defineBoundary((void *)far03, Cfar03.m_N, 3);
  combinedReg.defineBoundary((void *)far811, Cfar811.m_N, 4);
  // output
  combinedReg.outXml("outerRegion.xml");
  combinedReg.outCOMPO("outerRegion.xml", comp3);
  return 0;
}