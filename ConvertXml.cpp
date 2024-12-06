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

bool leftbnd(vector<double>&p) {
  return p[0] < -10. && fabs(p[1])<5.;
}

bool rightbnd(vector<double>&p) {
  return p[0] > 10. && fabs(p[1])<5.;
}

bool upbnd(vector<double>&p) {
  return p[1] > 10. && fabs(p[0])<5.;
}

bool downbnd(vector<double>&p) {
  return p[1] < -10. && fabs(p[0])<5.;
}

bool wallbnd(vector<double>&p) {
  return fabs(p[0]) < 3. && fabs(p[1])<3.;
}

int main(int argc, char *argv[]) {
  bool merge = false;
  std::string infilename, outfilename;
  if(argc>=3) {
    infilename = argv[1];
    outfilename = argv[2];
  } else {
    cout << " usage: ConvertXml infile.xml outfile.xml" << endl;
    exit(-1);
  }
  /// initialise
  cout << "start meshing --------" << endl;
  MeshRegions region("Rxmk:", 1.E-6);
  region.loadFromXml(infilename, "");
  std::map<int, void *> edgeFuns;
  edgeFuns[0] = (void*) wallbnd;
  edgeFuns[1] = (void*) leftbnd;
  edgeFuns[2] = (void*) rightbnd;
  edgeFuns[3] = (void*) upbnd;
  edgeFuns[4] = (void*) downbnd;
  region.defineBoundary(edgeFuns, 0.9*M_PI/2.);
  vector<int> comp3;
  comp3.push_back(0);
  region.outXml(outfilename);
  region.outCOMPO(outfilename, comp3);
  return 0;
}
