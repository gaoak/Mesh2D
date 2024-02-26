#include "CAD2D/BLMeshModule.h"
#include "CAD2D/CompositEdge.h"
#include "CAD2D/FlatPlate.h"
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
  std::map<std::string, double> p;
  std::map<std::string, int> q;
  DefineBLParams(p, q);
  BLModel = std::make_shared<FlatPlate>(p, q);
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
  pic0.MeshGen(int((nearBoxRight - nearBoxLeft) / maxLayerh + 0.5),
               int((nearBoxUp - nearBoxDown) / maxLayerh + 0.5));
  nearBodyRegion.AddRegion(pic0);
  nearBodyRegion.transformation(nearAoA, 0., 0.);

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

int outputXML(MeshRegions &combinedReg) {
  // output outer region
  vector<int> comp3;
  comp3.push_back(0);
  // wall

  int offset = BLModel->DefineBCs(combinedReg, 0, BLedges);
  // inlet
  combinedReg.defineBoundary((void *)far08, Cfar08.m_N, offset);
  // outlet
  combinedReg.defineBoundary((void *)far311, Cfar311.m_N, 1 + offset);
  // side
  combinedReg.defineBoundary((void *)far03, Cfar03.m_N, 2 + offset);
  combinedReg.defineBoundary((void *)far811, Cfar811.m_N, 3 + offset);
  // output
  combinedReg.outXml("outerRegion.xml");
  combinedReg.outCOMPO("outerRegion.xml", comp3);
  return 0;
}