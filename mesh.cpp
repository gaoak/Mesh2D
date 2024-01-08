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
int meshingWake(MeshRegions &combinedReg);
int outputXML(MeshRegions &combinedReg);
int outputGeo(MeshRegions &combinedReg, MeshRegions &FarFieldReg,
              vector<double> pC);
int meshingOuterBoundary(MeshRegions &combinedReg);
int main(int argc, char *argv[]) {
  bool merge = false;
  string mshfilename;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "merge") == 0) {
      if (argc > i + 1)
        mshfilename = string(argv[i + 1]);
      merge = true;
    }
  }
  /// initialise
  InitPts();
  cout << "start meshing --------" << endl;
  MeshRegions combinedReg("RComb_", 1.E-6);
  MeshRegions FarFieldReg("RFar_", 1.E-6);
  meshingNearBody(combinedReg);
  meshingWake(combinedReg);
  meshingOuterBoundary(FarFieldReg);
  if (!merge) {
    vector<double> pC = {g_ptsF[7][0], g_ptsF[7][1]};
    outputGeo(combinedReg, FarFieldReg, pC);
    cout << "output CAD file" << endl;
    cout << "=======================================" << endl;
  } else {
    combinedReg.AddRegion(FarFieldReg);
    MeshRegions gmshReg("R_gmsh_", 1.E-8);
    gmshReg.loadFromMsh(mshfilename, 135. / 180. * 3.14159);
    cout << "load " << mshfilename << endl;
    vector<int> comp1;
    comp1.push_back(0);
    comp1.push_back(gmshReg.getCellsNumber());
    gmshReg.outXml("FarField.xml");
    gmshReg.outCOMPO("FarField.xml", comp1);
    if (!combinedReg.consistancyCheck(gmshReg)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    if (!gmshReg.consistancyCheck(combinedReg)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    combinedReg.AddRegion(gmshReg);
    outputXML(combinedReg);
    cout << "------------------------------------" << endl;
    cout << "------------------------------------" << endl;
  }
  return 0;
}

int meshingNearBody(MeshRegions &combinedReg) {
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

int outputGeo(MeshRegions &combinedReg, MeshRegions &FarFieldReg,
              vector<double> pC) {
  // generate gmsh geo file
  vector<vector<double>> box;
  std::vector<std::vector<int>> boundary = FarFieldReg.extractBoundaryPoints();
  int wallID = -1;
  for (int i = 0; i < boundary.size(); ++i) {
    for (int j = 0; j < boundary[i].size(); ++j) {
      if (fabs(FarFieldReg.m_pts[boundary[i][j]][0] - pC[0]) +
              fabs(FarFieldReg.m_pts[boundary[i][j]][1] - pC[1]) <
          1E-6) {
        wallID = i;
        break;
      }
    }
    if (wallID != -1)
      break;
  }
  for (int i = 0; i < boundary[wallID].size(); ++i) {
    vector<double> p0 = FarFieldReg.m_pts[boundary[wallID][i]];
    box.push_back(p0);
  }
  vector<double> center;
  center.push_back(0.);
  center.push_back(0.);

  // output outer inner region (no wall) for Omesh
  combinedReg.outOuterRegion("FarField.geo", box, center, .1, true);
  return 0;
}

int outputGeo(MeshRegions &combinedReg, MeshRegions &nearWallRegion,
              vector<vector<double>> &breakpts, MeshRegions &FarFieldReg,
              vector<double> pC) {
  // generate gmsh geo file
  vector<vector<double>> box;
  std::vector<std::vector<int>> boundary = FarFieldReg.extractBoundaryPoints();
  int wallID = -1;
  for (int i = 0; i < boundary.size(); ++i) {
    for (int j = 0; j < boundary[i].size(); ++j) {
      if (fabs(FarFieldReg.m_pts[boundary[i][j]][0] - pC[0]) +
              fabs(FarFieldReg.m_pts[boundary[i][j]][1] - pC[1]) <
          1E-6) {
        wallID = i;
        break;
      }
    }
    if (wallID != -1)
      break;
  }
  for (int i = 0; i < boundary[wallID].size(); ++i) {
    vector<double> p0 = FarFieldReg.m_pts[boundary[wallID][i]];
    box.push_back(p0);
  }
  vector<double> center;
  center.push_back(0.);
  center.push_back(0.);

  // output outer inner region (no wall) for Omesh
  combinedReg.outOuterRegion("FarField.geo", box, center, .1, true);
  combinedReg.omeshBoundaryMapping("wallmapping.dat", center, 0.1);
  vector<vector<double>> nobox;
  // nearWallRegion.outOuterRegion("airfoil.geo",nobox, center, .1, false);
  // nearWallRegion.outOuterRegion("airfoil.geo", nobox, center, .1, false);
  return 0;
}

int outputXML(MeshRegions &combinedReg) {
  // output outer region
  vector<int> comp3;
  comp3.push_back(0);
  // wall
  combinedReg.defineBoundary((void *)edge01, Cedge01.m_N, 0);
  combinedReg.defineBoundary((void *)edge12, Cedge12.m_N, 0);
  combinedReg.defineBoundary((void *)edge23, Cedge23.m_N, 0);
  combinedReg.defineBoundary((void *)edge34, Cedge34.m_N, 0);
  combinedReg.defineBoundary((void *)edge45, Cedge45.m_N, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge56, Cedge56.m_N, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge67, Cedge67.m_N, 0);
  combinedReg.defineBoundary((void *)edge78, Cedge78.m_N, 0);
  combinedReg.defineBoundary((void *)edge89, Cedge89.m_N, 0);
  combinedReg.defineBoundary((void *)edge910, Cedge910.m_N, 0);
  combinedReg.defineBoundary((void *)edge1011, Cedge1011.m_N, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge110, Cedge110.m_N, 0, curvedpts);
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