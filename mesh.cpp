#include "CAD2D/CompositEdge.h"
#include "CAD2D/LineEdge.h"
#include "CAD2D/MeshRegions.h"
#include "CAD2D/MeshTool.h"
#include "CAD2D/RectRegion.h"
#include "CAD2D/airfoil.h"
#include "CAD2D/util.h"
#include "edgefunctions.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

int geoFarDomain(std::string filename) {
  std::vector<std::vector<double>> pts;
  double ds;
  ds = 2. / Cedge43.m_N;
  for (int i = 0; i <= Cedge43.m_N; ++i) {
    pts.push_back(edge43(-1. + i * ds));
  }
  ds = 2. / Cedge36.m_N;
  for (int i = 1; i < Cedge36.m_N; ++i) {
    pts.push_back(edge36(-1. + i * ds));
  }
  ds = 2. / Cedge76.m_N;
  for (int i = 0; i <= Cedge76.m_N; ++i) {
    pts.push_back(edge76(1. - i * ds));
  }
  ds = 2. / Cedge47.m_N;
  for (int i = 1; i < Cedge47.m_N; ++i) {
    pts.push_back(edge47(1. - i * ds));
  }
  OutGeo(filename, pts, std::vector<std::vector<std::vector<double>>>());
  return 0;
}
int geoCentreDomain(std::string filename, MeshRegion &upperDomain,
                    MeshRegion &lowerDomain) {
  double angle = 45. / 180. * M_PI;
  std::vector<std::vector<int>> bndupper = upperDomain.splitBoundaryPts(angle);
  int idUpper = -1;
  for (size_t i = 0; i < bndupper.size(); ++i) {
    if (fabs(upperDomain.m_pts[bndupper[i][0]][1] -
             upperDomain.m_pts[bndupper[i][1]][1]) < 1E-6 &&
        fabs(upperDomain.m_pts[bndupper[i][0]][1] - G_pts[5][1]) < 1E-6) {
      idUpper = i;
      break;
    } else {
      continue;
    }
  }
  if (fabs(upperDomain.m_pts[bndupper[idUpper][0]][0] - G_pts[5][0]) < 1E-6) {
    std::reverse(bndupper[idUpper].begin(), bndupper[idUpper].end());
  }
  std::vector<std::vector<int>> bndlower = lowerDomain.splitBoundaryPts(angle);
  int idLower = -1;
  for (size_t i = 0; i < bndlower.size(); ++i) {
    if (fabs(lowerDomain.m_pts[bndlower[i][0]][0] -
             lowerDomain.m_pts[bndlower[i][1]][0]) < 1E-6)
      continue;
    if (fabs(lowerDomain.m_pts[bndlower[i][0]][1] - G_ptsA[0][1]) < 1E-6) {
      idLower = i;
      break;
    } else {
      continue;
    }
  }
  if (fabs(lowerDomain.m_pts[bndlower[idLower][0]][0] - G_ptsA[0][0]) > 1E-6) {
    std::reverse(bndlower[idLower].begin(), bndlower[idLower].end());
  }
  std::vector<std::vector<double>> pts;
  for (auto p : bndlower[idLower]) {
    pts.push_back(lowerDomain.m_pts[p]);
  }
  double ds = 2. / CedgeA5.m_N;
  for (int i = 1; i < CedgeB2.m_N; ++i) {
    pts.push_back(edgeB2(-1. + i * ds));
  }
  for (auto p : bndupper[idUpper]) {
    pts.push_back(upperDomain.m_pts[p]);
  }
  for (int i = 1; i < CedgeA5.m_N; ++i) {
    pts.push_back(edgeA5(1. - i * ds));
  }
  OutGeo(filename, pts, std::vector<std::vector<std::vector<double>>>());
  return 0;
}
int mergeGmshDomain(std::string gmshfilename, MeshRegions &combined) {
  MeshRegions gmshReg("gmsh", 1.E-8);
  gmshReg.loadFromMsh(gmshfilename, 125. / 180. * M_PI);
  cout << "load " << gmshfilename << endl;
  if (!combined.consistancyCheck(gmshReg)) {
    cout << "Error: node mismatch, exit" << endl;
    return -1;
  }
  if (!gmshReg.consistancyCheck(combined)) {
    cout << "Error: node mismatch, exit" << endl;
    return -1;
  }
  combined.AddRegion(gmshReg);
  return 0;
}
int main(int argc, char *argv[]) {
  bool merge = false;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "merge") == 0) {
      merge = true;
    }
  }
  InitPts();
  cout << "start meshing --------" << endl;
  // meshing bottom boundary layer
  MeshRegions lowerdomain("Boundarylayer", 1E-5);
  setRadiusMesh(hFirstLayerBottom, progressBottom, maxLayerhBottom);
  std::vector<std::vector<double>> trimNorms;
  std::vector<double> tmpnorm = {0., 1., 0.};
  trimNorms.push_back(tmpnorm);
  trimNorms.push_back(tmpnorm);
  meshingBoundayLayer(lowerdomain, Cedge01.m_N, (void *)thickFunc01,
                      (void *)edge01, "Bottom", trimNorms);
  // meshingBoundayLayer(lowerdomain, (void *)thickFuncOval, (void *)edgeinner,
  // "Oval", std::vector<std::vector<double>>()); meshing upper domain
  std::vector<double> box;
  lowerdomain.GetBoundBox(box);
  G_ptsA[0][1] = box[3];
  G_ptsA[1][1] = box[3];
  G_pts[5][1] = G_ptsA[0][1] + centralGap;
  G_pts[2][1] = G_ptsA[1][1] + centralGap;
  std::vector<void *> edges;
  edges.push_back((void *)edge52);
  edges.push_back((void *)edge23);
  edges.push_back((void *)edge43);
  edges.push_back((void *)edge54);
  RectRegion upperdomain(edges, "upperdomain");
  upperdomain.MeshGen(Cedge52.m_N, Cedge23.m_N);
  // upperdomain.Tec360Pts("upperdomain.dat");
  // meshing centre domain
  geoCentreDomain("Centre.geo", upperdomain, lowerdomain);
  geoFarDomain("UpperFar.geo");
  if (!merge)
    return 0;

  MeshRegions combinedReg("globle", 1E-6);
  combinedReg.AddRegion(upperdomain);
  combinedReg.AddRegion(lowerdomain);
  mergeGmshDomain("Centre.msh", combinedReg);
  mergeGmshDomain("UpperFar.msh", combinedReg);
  // define boundary conditions
  std::map<int, void *> conditions;
  vector<int> comp3;
  comp3.push_back(0);
  // wall
  combinedReg.defineBoundary((void *)edge01, Cedge01.m_N, 0, curvedpts, 0., 1);
  // left
  conditions[1] = (void *)leftBnd;
  conditions[2] = (void *)rightBnd;
  conditions[3] = (void *)upperBnd;
  combinedReg.defineBoundary(conditions, 45. / 180. * M_PI);
  // output
  combinedReg.outXml("outerRegion.xml");
  combinedReg.outCOMPO("outerRegion.xml", comp3);
  return 0;
}
