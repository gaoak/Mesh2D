#include "CAD2D/MeshRegions.h"

#include "edgefunctions.h"
#include "params.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool parametersAreValid() {
  if (innerRadius <= 0.0) {
    std::cerr << "innerRadius must be positive.\n";
    return false;
  }
  if (outerRadius <= innerRadius) {
    std::cerr << "outerRadius must be greater than innerRadius.\n";
    return false;
  }
  if (circumferentialCells < 4 || radialCells < 1) {
    std::cerr << "circumferentialCells must be at least 4 and radialCells "
                 "must be positive.\n";
    return false;
  }
  if (radialGrowth <= 0.0) {
    std::cerr << "radialGrowth must be positive.\n";
    return false;
  }
  if (curvedPoints < 3) {
    std::cerr << "curvedPoints must be at least 3.\n";
    return false;
  }
  return true;
}

void generateRing(MeshRegions &mesh) {
  for (int j = 0; j < radialCells; ++j) {
    for (int i = 0; i < circumferentialCells; ++i) {
      mesh.AddElement(ringElementVertices(i, j));
    }
  }

  mesh.ResetBndPts();
  mesh.rebuildEdgesIndex();
}

} // namespace

int main(int argc, char *argv[]) {
  if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
    std::cout << "Usage: " << argv[0] << " [output.xml]\n"
              << "Mesh parameters are configured in params.h.\n";
    return 0;
  }
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " [output.xml]\n";
    return 1;
  }
  if (!parametersAreValid()) {
    return 1;
  }

  const std::string output = argc == 2 ? argv[1] : "ring.xml";
  MeshRegions mesh("Ring", meshTolerance);
  generateRing(mesh);

  // C[0]: inner circle, C[1]: outer circle.
  mesh.defineBoundary(reinterpret_cast<void *>(innerBoundary),
                      circumferentialCells, 0, curvedPoints);
  mesh.defineBoundary(reinterpret_cast<void *>(outerBoundary),
                      circumferentialCells, 1, curvedPoints);
  mesh.CheckMesh();
  mesh.outXml(output);
  mesh.outCOMPO(output, {0});

  std::cout << "Generated " << output << " with " << mesh.getCellsNumber()
            << " quadrilateral elements.\n";
  return 0;
}
