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
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
double AoA = 15. / 180. * M_PI;
int nCurved = 4;

bool leftbnd(vector<double> &p) { return p[0] < -10. && fabs(p[1]) < 5.; }

bool rightbnd(vector<double> &p) { return p[0] > 10. && fabs(p[1]) < 5.; }

bool upbnd(vector<double> &p) { return p[1] > 10. && fabs(p[0]) < 5.; }

bool downbnd(vector<double> &p) { return p[1] < -10. && fabs(p[0]) < 5.; }

bool wallbnd(vector<double> &p) { return fabs(p[0]) < 3. && fabs(p[1]) < 3.; }

NACAmpxx airfoil(0., 0., 0.12, false);

std::vector<double> Transform(std::vector<double> pt, double aoa) {
  double x = pt[0], y = pt[1];
  pt[0] = x * cos(aoa) + y * sin(aoa);
  pt[1] = -x * sin(aoa) + y * cos(aoa);
  return pt;
}

std::vector<double> FixPoint(double &s, std::vector<double> pt) {
  if (pt[0] > 0.995)
    pt[0] = 1.;
  if (pt[1] > 0.) {
    pt = airfoil.Upper(airfoil.Findx(s));
  } else {
    pt = airfoil.Lower(airfoil.Findx(s));
    s = -s;
  }
  return pt;
}

std::vector<std::vector<double>> wallmapping;
int findpoints(std::vector<double> &p) {
  for (size_t i = 0; i < wallmapping.size(); ++i) {
    if (0.5 * (fabs(wallmapping[i][0] - p[0]) +
               fabs(wallmapping[i][1] - p[1])) <
            1e-8 ||
        0.5 * (fabs(wallmapping[i][2] - p[0]) +
               fabs(wallmapping[i][3] - p[1])) <
            1e-8) {
      return (int)i;
    }
  }
  return -1;
}

std::vector<std::vector<double>> wallFunction(std::vector<double> &p0,
                                              std::vector<double> &p1) {
  int pid0 = findpoints(p0);
  int pid1 = findpoints(p1);
  int nw0, nw1;
  if (pid0 == -1) {
    wallmapping.push_back(std::vector<double>(4));
    nw0 = wallmapping.size() - 1;
    wallmapping[nw0][0] = p0[0];
    wallmapping[nw0][1] = p0[1];
  }
  if (pid1 == -1) {
    wallmapping.push_back(std::vector<double>(4));
    nw1 = wallmapping.size() - 1;
    wallmapping[nw1][0] = p1[0];
    wallmapping[nw1][1] = p1[1];
  }

  p0 = Transform(p0, -AoA);
  p1 = Transform(p1, -AoA);
  double s0 = airfoil.Finds(p0[0]);
  double s1 = airfoil.Finds(p1[0]);
  p0 = FixPoint(s0, p0);
  p1 = FixPoint(s1, p1);
  std::vector<std::vector<double>> ps;
  ps.push_back(Transform(p0, AoA));
  // curved points
  if (nCurved > 2 && (p0[0] < 0.99 || p1[0] < 0.99)) {
    double ds = (s1 - s0) / (nCurved - 1);
    for (int i = 1; i < nCurved - 1; ++i) {
      double ts = s0 + i * ds;
      if (ts < 0)
        ps.push_back(Transform(airfoil.Lower(airfoil.Findx(fabs(ts))), AoA));
      else
        ps.push_back(Transform(airfoil.Upper(airfoil.Findx(fabs(ts))), AoA));
    }
  }
  // curved points
  p0 = Transform(p0, AoA);
  p1 = Transform(p1, AoA);
  if (pid0 == -1) {
    wallmapping[nw0][2] = p0[0];
    wallmapping[nw0][3] = p0[1];
  }
  if (pid1 == -1) {
    wallmapping[nw1][2] = p1[0];
    wallmapping[nw1][3] = p1[1];
  }
  ps.push_back(p1);
  return ps;
}

void OutputWallmapping(std::string filename) {
  std::ofstream file(filename);
  char buffer[1000];
  file << wallmapping.size() << " 2\n";
  for (size_t i = 0; i < wallmapping.size(); ++i) {
    sprintf(buffer, "%25.18lf %25.18lf %25.18lf %25.18lf\n", wallmapping[i][0],
            wallmapping[i][1], wallmapping[i][2], wallmapping[i][3]);
    file << buffer;
  }
}

int main(int argc, char *argv[]) {
  bool merge = false;
  std::string infilename, outfilename;
  if (argc >= 3) {
    infilename = argv[1];
    outfilename = argv[2];
  } else {
    cout << " usage: ConvertXml infile.xml outfile.xml" << endl;
    exit(-1);
  }
  /// initialise
  cout << "start meshing --------" << endl;
  MeshRegions combinedReg("Rxml:", 1e-6);
  MeshRegions region("Star", 1.E-6);
  region.loadFromXml(infilename, "");
  combinedReg.AddRegion(region);
  std::map<int, void *> edgeFuns;
  edgeFuns[0] = (void *)wallbnd;
  edgeFuns[1] = (void *)leftbnd;
  edgeFuns[2] = (void *)rightbnd;
  edgeFuns[3] = (void *)upbnd;
  edgeFuns[4] = (void *)downbnd;
  combinedReg.defineBoundary(edgeFuns, 0.9 * M_PI / 2.);
  combinedReg.defineCurvedBoundary(0, (void *)wallFunction);
  vector<int> comp3;
  comp3.push_back(0);
  combinedReg.outXml(outfilename);
  combinedReg.outCOMPO(outfilename, comp3);
  OutputWallmapping("wallmapping.dat");
  return 0;
}
