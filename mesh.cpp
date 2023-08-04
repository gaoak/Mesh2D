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

int geoCentreDomain(std::string filename, MeshRegion &upperDomain,
                    MeshRegion &lowerDomain) {
  double angle = 80. / 180. * M_PI;
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
int meshCentreDomain(std::string gmshfilename, MeshRegion &upperDomain,
                     MeshRegion &lowerDomain, MeshRegions &combined) {
  combined.AddRegion(upperDomain);
  combined.AddRegion(lowerDomain);
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
  bool withwake = false;
  string mshfilename;
  string mshinfoilfilename1;
  string mshinfoilfilename2;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "merge") == 0) {
      merge = true;
    }
  }
  InitPts();
  cout << "start meshing --------" << endl;
  // meshing bottom boundary layer
  MeshRegions lowerdomain("Boundarylayer", 1E-5);
  setRadiusMesh(hFirstLayer, progress, maxLayerh);
  meshingBoundayLayer(lowerdomain, (void *)thickFunc01, (void *)edge01,
                      "Bottom", std::vector<std::vector<double>>());
  // meshingBoundayLayer(lowerdomain, (void *)thickFuncOval, (void *)edgeinner,
  // "Oval", std::vector<std::vector<double>>()); meshing upper domain
  std::vector<double> box;
  lowerdomain.GetBoundBox(box);
  double height = 5, gap = 0.1;
  G_ptsA[0][0] = box[0];
  G_ptsA[0][1] = box[3];
  G_ptsA[1][0] = box[1];
  G_ptsA[1][1] = box[3];
  G_pts[5][0] = G_ptsA[0][0];
  G_pts[5][1] = G_ptsA[0][1] + gap;
  G_pts[2][0] = G_ptsA[1][0];
  G_pts[2][1] = G_ptsA[1][1] + gap;
  double hFirstLayerup = 1. / Cedge52.m_N, progressup = 1.2, maxLayerhup = 1.;
  setRadiusMesh(hFirstLayerup, progressup, maxLayerhup);
  int nLayers = findNlayers(hFirstLayerup, progressup, height, maxLayerhup);
  setRadiusLayers(nLayers);
  std::vector<void *> edges;
  edges.push_back((void *)edge52);
  edges.push_back((void *)radiusEdge);
  edges.push_back(nullptr);
  edges.push_back(nullptr);
  RectRegion upperdomain(edges, "upperdomain.dat", false);
  upperdomain.MeshGen(Cedge52.m_N, nLayers, eBoundaryLayer1);
  upperdomain.Tec360Pts("upperdomain.dat");
  upperdomain.GetBoundBox(box);
  G_pts[4][0] = box[0];
  G_pts[4][1] = box[3];
  G_pts[3][0] = box[1];
  G_pts[3][1] = box[3];
  // meshing centre domain
  geoCentreDomain("Centre.geo", upperdomain, lowerdomain);
  if (!merge)
    return 0;

  MeshRegions combinedReg("globle", 1E-6);
  meshCentreDomain("Centre.msh", upperdomain, lowerdomain, combinedReg);
  // define boundary conditions
  std::vector<void *> conditions(1);
  vector<int> comp3;
  comp3.push_back(0);
  // wall
  combinedReg.defineBoundary((void *)edge01, Cedge01.m_N, 0, curvedpts, AoA, 1);
  // left
  conditions[0] = (void *)leftBnd;
  combinedReg.defineBoundary(conditions, 45. / 180. * M_PI);
  // right
  conditions[0] = (void *)rightBnd;
  combinedReg.defineBoundary(conditions, 45. / 180. * M_PI);
  // upper
  conditions[0] = (void *)upperBnd;
  combinedReg.defineBoundary(conditions, 45. / 180. * M_PI);
  // output
  combinedReg.outXml("outerRegion.xml");
  combinedReg.outCOMPO("outerRegion.xml", comp3);
  return 0;
}
