#include "CAD2D/CompositEdge.h"
#include "CAD2D/LineEdge.h"
#include "CAD2D/MeshRegions.h"
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
int meshingVortexPassway(MeshRegions &combinedReg);
int meshingWake(MeshRegions &combinedReg);
int outputXML(MeshRegions &combinedReg, MeshRegions &inFoilRegion);
int outputGeo(MeshRegions &combinedReg, MeshRegions &nearWallRegion,
              MeshRegions &FarFieldReg, vector<double> pC);
int meshingOuterBoundary(MeshRegions &combinedReg, vector<double> &p);
int main(int argc, char *argv[]) {
  bool merge = false;
  string mshfilename, mshfilename2;
  string mshinfoilfilename1;
  string mshinfoilfilename2;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "merge") == 0) {
      if (argc > i + 1)
        mshfilename = string(argv[i + 1]);
      if (argc > i + 2)
        mshfilename2 = string(argv[i + 2]);
      if (argc > i + 3)
        mshinfoilfilename1 = string(argv[i + 3]);
      if (argc > i + 4)
        mshinfoilfilename2 = string(argv[i + 4]);
      merge = true;
    }
  }
  /// initialize
  MeshRegions combinedReg("R_Comb_", 1.E-6);
  MeshRegions nearFieldReg("R_NearField_", 1.E-6);
  MeshRegions nearWallRegion("R_NearField_", 1.E-6);
  MeshRegions inFoilRegion("R_inFoil_", 1.E-6);
  InitPts();
  cout << "start meshing --------" << endl;
  meshingNearBody(combinedReg);
  nearWallRegion.AddRegion(combinedReg);
  MeshRegions FarFieldReg("RFar", 1.E-6);
  vector<double> pC;
  meshingOuterBoundary(FarFieldReg, pC);
  meshingVortexPassway(combinedReg);
  meshingWake(combinedReg);

  if (!merge) {
    outputGeo(combinedReg, nearWallRegion, FarFieldReg, pC);
    cout << "output CAD file" << endl;
    cout << "=======================================" << endl;
  }
  /////////////////////////////////////////////////////
  //////////////gmsh region, far field region//////////
  // step 2 import mesh
  if (merge) {
    combinedReg.AddRegion(FarFieldReg);
    // load gmsh1
    MeshRegions gmshReg("R_gmsh_", 1.E-8);
    gmshReg.loadFromMsh(mshfilename, 135. / 180. * 3.14159);
    cout << "load " << mshfilename << endl;
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
    gmshReg2.loadFromMsh(mshfilename2, 110. / 180. * 3.14159);
    cout << "load " << mshfilename2 << endl;
    if (!combinedReg.consistancyCheck(gmshReg2)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    if (!gmshReg2.consistancyCheck(combinedReg)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    combinedReg.AddRegion(gmshReg2);
    // in airfoil mesh
    MeshRegions gmshInFoil1("R_gmsh1_", 1.E-8);
    gmshInFoil1.loadFromMsh(mshinfoilfilename1, 120. / 180. * 3.14159);
    cout << "load " << mshinfoilfilename1 << endl;
    if (!nearWallRegion.consistancyCheck(gmshInFoil1)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    inFoilRegion.AddRegion(gmshInFoil1);
    MeshRegions gmshInFoil2("R_gmsh2_", 1.E-8);
    gmshInFoil2.loadFromMsh(mshinfoilfilename2, 135. / 180. * 3.14159);
    cout << "load " << mshinfoilfilename2 << endl;
    if (!nearWallRegion.consistancyCheck(gmshInFoil2)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    if (!gmshInFoil2.consistancyCheck(nearWallRegion)) {
      cout << "Error: node mismatch, exit" << endl;
      return -1;
    }
    inFoilRegion.AddRegion(gmshInFoil2);
    outputXML(combinedReg, inFoilRegion);
    cout << "------------------------------------" << endl;
    cout << "------------------------------------" << endl;
  }
  return 0;
}

int meshingNearBody(MeshRegions &combinedReg) {
  /////////near body region////////////////
  /////////near body region////////////////
  std::vector<RectRegion> Rects;
  // boundary layer region 0
  std::vector<void *> edges0;
  void *edge0;
  // edge 2
  edges0.push_back((void *)edge2);
  edges0.push_back((void *)radiusEdge);
  edges0.push_back((void *)edge0);
  edges0.push_back((void *)edge0);
  Rects.push_back(RectRegion(edges0, "Rrear", false));
  setRadiusLayers(nLayers2);
  Rects[Rects.size() - 1].MeshGen(Ncyl2, nLayers2, eBoundaryLayer1, false);
  Rects[Rects.size() - 1].Tec360Pts("test2.dat");
  // edge 3
  edges0[0] = (void *)edge3;
  Rects.push_back(RectRegion(edges0, "Rsidedown", false));
  setRadiusLayers(nLayers3);
  Rects[Rects.size() - 1].MeshGen(Ncyl3, nLayers3, eBoundaryLayer1, false);
  Rects[Rects.size() - 1].Tec360Pts("test3.dat");
  // edge 4
  edges0[0] = (void *)edge4;
  Rects.push_back(RectRegion(edges0, "Rfront", false));
  setRadiusLayers(nLayers4);
  Rects[Rects.size() - 1].MeshGen(Ncyl4, nLayers4, eBoundaryLayer1, false);
  Rects[Rects.size() - 1].Tec360Pts("test4.dat");
  // edge 5
  edges0[0] = (void *)edge5;
  Rects.push_back(RectRegion(edges0, "Rsideup", false));
  setRadiusLayers(nLayers5);
  Rects[Rects.size() - 1].MeshGen(Ncyl5, nLayers5, eBoundaryLayer1, false);
  Rects[Rects.size() - 1].Tec360Pts("test5.dat");

  ///////////// combine the near field mesh
  for (unsigned int i = 0; i < Rects.size(); ++i) {
    combinedReg.AddRegion(Rects[i]);
  }
  return 0;
}

int meshingOuterBoundary(MeshRegions &combinedReg, vector<double> &p) {
  vector<double> pD = edge10(-1. + 1 * 2. / Cedge10.m_N);
  vector<double> pH = edge10(1. - 1 * 2. / Cedge10.m_N);
  vector<double> pE = edge12(-1. + 1 * 2. / Cedge12.m_N);
  vector<double> pA = edge12(1. - 1 * 2. / Cedge12.m_N);
  vector<double> p8 = edge9(-1.);
  vector<double> p9 = edge9(1.);
  vector<double> p10 = edge11(-1.);
  vector<double> p11 = edge11(1.);
  vector<vector<vector<double>>> edges;
  // right edge
  vector<vector<double>> EA8;
  vector<vector<double>> E89;
  vector<vector<double>> ED9;
  vector<vector<double>> EAD;
  EA8.push_back(pA);
  EA8.push_back(p8);
  ED9.push_back(pD);
  ED9.push_back(p9);
  for (int i = 0; i <= Cedge9.m_N; ++i) {
    double tmps = -1. + i * 2. / Cedge9.m_N;
    vector<double> p0 = edge9(tmps);
    E89.push_back(p0);
    p0[0] = pD[0] * (p0[1] - pA[1]) / (pD[1] - pA[1]) +
            pA[0] * (p0[1] - pD[1]) / (pA[1] - pD[1]);
    EAD.push_back(p0);
  }
  edges.clear();
  edges.push_back(EA8);
  edges.push_back(E89);
  edges.push_back(ED9);
  edges.push_back(EAD);
  RectRegion pic9 = RectRegion(edges, "pic");
  pic9.MeshGen(1, Cedge9.m_N);
  pic9.Tec360Pts("pic9.dat");
  combinedReg.AddRegion(pic9);
  // left  edge
  vector<vector<double>> EH10;
  vector<vector<double>> E1011;
  vector<vector<double>> EE11;
  vector<vector<double>> EHE;
  EH10.push_back(pH);
  EH10.push_back(p10);
  EE11.push_back(pE);
  EE11.push_back(p11);
  for (int i = 0; i <= Cedge11.m_N; ++i) {
    double tmps = -1. + i * 2. / Cedge11.m_N;
    vector<double> p0 = edge11(tmps);
    E1011.push_back(p0);
    p0[0] = pH[0] * (p0[1] - pE[1]) / (pH[1] - pE[1]) +
            pE[0] * (p0[1] - pH[1]) / (pE[1] - pH[1]);
    EHE.push_back(p0);
  }
  edges.clear();
  edges.push_back(EH10);
  edges.push_back(E1011);
  edges.push_back(EE11);
  edges.push_back(EHE);
  RectRegion pic11 = RectRegion(edges, "pic");
  pic11.MeshGen(1, Cedge11.m_N);
  pic11.Tec360Pts("pic11.dat");
  combinedReg.AddRegion(pic11);
  // upper edge
  vector<double> pB = EAD[1];
  vector<double> pC = EAD[EAD.size() - 2];
  vector<double> pG = EHE[1];
  vector<double> pF = EHE[EHE.size() - 2];
  vector<vector<double>> EDH;
  vector<vector<double>> ECG;
  vector<vector<double>> ECD;
  vector<vector<double>> EHG;
  ECD.push_back(pD);
  ECD.push_back(pC);
  EHG.push_back(pH);
  EHG.push_back(pG);
  for (int i = 1; i < Cedge10.m_N; ++i) {
    double tmps = -1. + i * 2. / Cedge10.m_N;
    vector<double> p0 = edge10(tmps);
    EDH.push_back(p0);
    p0[1] = pC[1] * (p0[0] - pG[0]) / (pC[0] - pG[0]) +
            pG[1] * (p0[0] - pC[0]) / (pG[0] - pC[0]);
    if (i == 1) {
      ECG.push_back(pC);
    } else if (i == Cedge10.m_N - 1) {
      ECG.push_back(pG);
    } else {
      ECG.push_back(p0);
    }
  }
  edges.clear();
  edges.push_back(ECD);
  edges.push_back(EDH);
  edges.push_back(EHG);
  edges.push_back(ECG);
  RectRegion pic10 = RectRegion(edges, "pic");
  pic10.MeshGen(1, Cedge10.m_N - 2);
  pic10.Tec360Pts("pic10.dat");
  combinedReg.AddRegion(pic10);
  // lower edge
  vector<vector<double>> EAE;
  vector<vector<double>> EBF;
  vector<vector<double>> EAB;
  vector<vector<double>> EEF;
  EAB.push_back(pA);
  EAB.push_back(pB);
  EEF.push_back(pE);
  EEF.push_back(pF);
  for (int i = 1; i < Cedge12.m_N; ++i) {
    double tmps = -1. + i * 2. / Cedge12.m_N;
    vector<double> p0 = edge12(tmps);
    EAE.push_back(p0);
    p0[1] = pB[1] * (p0[0] - pF[0]) / (pB[0] - pF[0]) +
            pF[1] * (p0[0] - pB[0]) / (pF[0] - pB[0]);
    if (i == 1) {
      EBF.push_back(pF);
    } else if (i == Cedge12.m_N - 1) {
      EBF.push_back(pB);
    } else {
      EBF.push_back(p0);
    }
  }
  edges.clear();
  edges.push_back(EAB);
  edges.push_back(EBF);
  edges.push_back(EEF);
  edges.push_back(EAE);
  RectRegion pic12 = RectRegion(edges, "pic");
  pic12.MeshGen(1, Cedge12.m_N - 2);
  pic12.Tec360Pts("pic12.dat");
  combinedReg.AddRegion(pic12);
  //
  p = pC;
  //
  // vector<int> comp4;
  // comp4.push_back(0);
  // combinedReg.outXml("manout.xml");
  // combinedReg.outCOMPO("manout.xml", comp4);
  return 0;
}

int meshingWake(MeshRegions &combinedReg) {
  pts[16][0] = farWakeLeft;
  pts[19][0] = farWakeLeft;
  pts[16][1] = farWakeDown;
  pts[19][1] = farWakeUp;
  pts[17][0] = farWakeRight;
  pts[18][0] = farWakeRight;
  pts[17][1] = pts[16][1] - (pts[17][0] - pts[16][0]) * tan(wakeDiffuseAngle);
  pts[18][1] = pts[19][1] + (pts[18][0] - pts[19][0]) * tan(wakeDiffuseAngle);
  std::vector<void *> edges4;
  edges4.push_back((void *)edge19);
  edges4.push_back((void *)edge20);
  edges4.push_back((void *)edge21);
  edges4.push_back((void *)edge22);
  RectRegion farwakeRegion = RectRegion(edges4, "R_FarWake");
  farwakeRegion.MeshGen(Cedge19.m_N, Cedge20.m_N);
  farwakeRegion.transformation(farWakeAoA);
  farwakeRegion.Tec360Pts("testfarwake.dat");

  //////////////combine region//////////
  combinedReg.AddRegion(farwakeRegion);
  vector<int> comp2;
  comp2.push_back(0);
  combinedReg.outXml("testmeshwake.xml");
  combinedReg.outCOMPO("testmeshwake.xml", comp2);
  return 0;
}

int meshingVortexPassway(MeshRegions &combinedReg) {
  squarePts[0][0] = SquareLeft;
  squarePts[3][0] = SquareLeft;
  squarePts[0][1] = SquareDown;
  squarePts[3][1] = SquareUp;
  squarePts[1][0] = SquareRight;
  squarePts[2][0] = SquareRight;
  squarePts[1][1] = squarePts[0][1] -
                    (squarePts[1][0] - squarePts[0][0]) * tan(wakeDiffuseAngle);
  squarePts[2][1] = squarePts[3][1] +
                    (squarePts[2][0] - squarePts[3][0]) * tan(wakeDiffuseAngle);
  std::vector<void *> edges4;
  edges4.push_back((void *)edgeA);
  edges4.push_back((void *)edgeB);
  edges4.push_back((void *)edgeC);
  edges4.push_back((void *)edgeD);
  RectRegion farwakeRegion = RectRegion(edges4, "R_FarWake");
  farwakeRegion.MeshGen(CedgeA.m_N, CedgeB.m_N);
  farwakeRegion.transformation(farWakeAoA);
  farwakeRegion.Tec360Pts("testfarwake.dat");

  combinedReg.GetBoundBox(boundingbox);
  boundingbox.push_back((SquareRight - SquareLeft) / CedgeA.m_N);
  farwakeRegion.RemoveElements((void *)toremove);
  //////////////combine region//////////
  combinedReg.AddRegion(farwakeRegion);
  vector<int> comp2;
  comp2.push_back(0);
  combinedReg.outXml("testmesh.xml");
  combinedReg.outCOMPO("testmesh.xml", comp2);
  return 0;
}

int outputXML(MeshRegions &combinedReg, MeshRegions &inFoilRegion) {
  // output outer region
  vector<int> comp3;
  comp3.push_back(0);
  // wall
  combinedReg.defineBoundary((void *)edge2, Ncyl2, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge3, Ncyl3, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge4, Ncyl4, 0, curvedpts);
  combinedReg.defineBoundary((void *)edge5, Ncyl5, 0, curvedpts);
  // inlet
  combinedReg.defineBoundary((void *)edge11, Cedge11.m_N, 1);
  // outlet
  combinedReg.defineBoundary((void *)edge9, Cedge9.m_N, 2);
  // side
  combinedReg.defineBoundary((void *)edge10, Cedge10.m_N, 3);
  combinedReg.defineBoundary((void *)edge12, Cedge12.m_N, 4, 2, 0., -1);
  // output
  combinedReg.outXml("outerRegion.xml");
  combinedReg.outCOMPO("outerRegion.xml", comp3);

  return 0;
}

int outputGeo(MeshRegions &combinedReg, MeshRegions &nearWallRegion,
              MeshRegions &FarFieldReg, vector<double> pC) {
  // outer layer
  vector<vector<double>> box;
  std::vector<std::vector<int>> boundary = FarFieldReg.extractBoundaryPoints();
  int wallID = -1;
  for (int i = 0; i < boundary.size(); ++i) {
    for (int j = 0; j < boundary[i].size(); ++j) {
      if (fabs(FarFieldReg.m_pts[boundary[i][j]][0] - pC[0]) +
              fabs(FarFieldReg.m_pts[boundary[i][j]][1] - pC[1]) <
          1E-1) {
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
  boundary = combinedReg.extractBoundaryPoints();
  std::vector<double> minx(boundary.size(), 1E20);
  for (size_t i = 0; i < boundary.size(); ++i) {
    for (int j = 0; j < boundary[i].size(); ++j) {
      if (combinedReg.m_pts[boundary[i][j]][0] < minx[i]) {
        minx[i] = combinedReg.m_pts[boundary[i][j]][0];
      }
    }
  }
  // box outer layer
  vector<vector<vector<double>>> innbox(minx.size());
  for (size_t b = 0; b < innbox.size(); ++b) {
    double minvalue = minx[0];
    wallID = 0;
    for (size_t i = 1; i < minx.size(); ++i) {
      if (minx[i] < minvalue) {
        minvalue = minx[i];
        wallID = i;
      }
    }
    for (int i = 0; i < boundary[wallID].size(); ++i) {
      vector<double> p0 = combinedReg.m_pts[boundary[wallID][i]];
      innbox[b].push_back(p0);
    }
    minx[wallID] = 1E20;
  }
  // box inner layer
  // airfoil outer layer

  std::vector<std::vector<std::vector<double>>> tmparray;
  tmparray.push_back(innbox[0]);
  tmparray.push_back(innbox[4]);
  OutGeo("FarField1.geo", box, tmparray);
  tmparray.clear();
  tmparray.push_back(innbox[2]);
  OutGeo("FarField2.geo", innbox[1], tmparray);
  return 0;
}
