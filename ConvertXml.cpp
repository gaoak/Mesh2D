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
  vector<int> comp3;
  comp3.push_back(0);
  region.outXml(outfilename);
  region.outCOMPO(outfilename, comp3);
  return 0;
}
