#include "CAD2D/MeshRegions.h"
#include "CAD2D/RectRegion.h"

#include "edgefunctions.h"
#include "params.h"

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool parametersAreValid() {
  if (innerRadius <= 0.0 || outerRadius <= innerRadius) {
    std::cerr << "Require 0 < innerRadius < outerRadius.\n";
    return false;
  }
  if (coreMeshSize <= 0.0 || innerFirstLayer <= 0.0 ||
      outerFirstLayer <= 0.0 || innerLayerGrowth < 1.0 ||
      outerLayerGrowth < 1.0) {
    std::cerr << "Mesh sizes must be positive and growth ratios at least 1.\n";
    return false;
  }
  if (innerCircumferentialCells < 4 || outerCircumferentialCells < 4) {
    std::cerr << "Each circle needs at least four circumferential cells.\n";
    return false;
  }
  if ((innerCircumferentialCells + outerCircumferentialCells) % 2 != 0) {
    std::cerr << "The total number of core-interface edges must be even for "
                 "full quadrilateral recombination.\n";
    return false;
  }
  if (innerCircumferentialCells % 4 != 0 ||
      outerCircumferentialCells % 4 != 0) {
    std::cerr << "Each circle's circumferential cell count must be divisible "
                 "by four.\n";
    return false;
  }
  if (innerBoundaryLayerThickness <= 0.0 ||
      outerBoundaryLayerThickness <= 0.0 ||
      innerFirstLayer > innerBoundaryLayerThickness ||
      outerFirstLayer > outerBoundaryLayerThickness) {
    std::cerr << "Invalid boundary-layer thickness.\n";
    return false;
  }
  const double centerDistance =
      std::hypot(innerCenterX - outerCenterX, innerCenterY - outerCenterY);
  const double innerCoreRadius =
      innerRadius + innerBoundaryLayerThickness;
  const double outerCoreRadius =
      outerRadius - outerBoundaryLayerThickness;
  if (centerDistance + innerCoreRadius >= outerCoreRadius) {
    std::cerr << "The two boundary layers overlap or leave no core region.\n";
    return false;
  }
  if (curvedPoints < 3) {
    std::cerr << "curvedPoints must be at least 3.\n";
    return false;
  }
  return true;
}

void generateInnerBoundaryLayer(MeshRegions &mesh) {
  initialiseCompositeEdges();
  std::vector<void *> edges = {reinterpret_cast<void *>(innerWallEdge),
                               reinterpret_cast<void *>(innerNormalEdge),
                               nullptr, nullptr};
  RectRegion region(edges, "InnerBL", false, meshTolerance);
  region.MeshGen(CinnerWall.m_N, innerBoundaryLayerCells(),
                 eBoundaryLayer1);
  mesh.AddRegion(region);
  mesh.ResetBndPts();
  mesh.rebuildEdgesIndex();
}

void generateOuterBoundaryLayer(MeshRegions &mesh) {
  initialiseCompositeEdges();
  std::vector<void *> edges = {reinterpret_cast<void *>(outerWallEdge),
                               reinterpret_cast<void *>(outerNormalEdge),
                               nullptr, nullptr};
  RectRegion region(edges, "OuterBL", false, meshTolerance);
  region.MeshGen(CouterWall.m_N, outerBoundaryLayerCells(),
                 eBoundaryLayer1);
  mesh.AddRegion(region);
  mesh.ResetBndPts();
  mesh.rebuildEdgesIndex();
}

void generateBoundaryLayers(MeshRegions &mesh) {
  generateInnerBoundaryLayer(mesh);
  generateOuterBoundaryLayer(mesh);
}

int outputGeo(MeshRegions &innerLayer, MeshRegions &outerLayer,
              const std::string &filename) {
  const std::vector<double> interfacePoint = outerCoreBoundary(-1.0);
  const std::vector<std::vector<int>> boundaries =
      outerLayer.extractBoundaryPoints();
  int interfaceId = -1;
  for (size_t i = 0; i < boundaries.size(); ++i) {
    for (const int pointId : boundaries[i]) {
      const std::vector<double> &p = outerLayer.m_pts[pointId];
      if (std::fabs(p[0] - interfacePoint[0]) +
              std::fabs(p[1] - interfacePoint[1]) <
          meshTolerance) {
        interfaceId = i;
        break;
      }
    }
    if (interfaceId >= 0) {
      break;
    }
  }
  if (interfaceId < 0) {
    std::cerr << "Cannot find the outer boundary-layer/core interface.\n";
    return 1;
  }

  std::vector<std::vector<double>> box;
  for (const int pointId : boundaries[interfaceId]) {
    box.push_back(outerLayer.m_pts[pointId]);
  }
  const std::vector<double> innerCenter = {innerCenterX, innerCenterY};
  const double boundarySelector =
      innerRadius + 0.5 * innerBoundaryLayerThickness;
  innerLayer.outOuterRegion(filename, box, innerCenter, boundarySelector,
                            true);

  std::ofstream geo(filename.c_str(), std::ofstream::app);
  if (!geo.is_open()) {
    std::cerr << "Cannot write " << filename << ".\n";
    return 1;
  }

  const int pointCount =
      box.size() + static_cast<size_t>(innerCircumferentialCells);
  const int firstLine = pointCount + 1;
  const int lastLine = pointCount * 2;
  geo << "Transfinite Line {" << firstLine << ":" << lastLine
      << "} = 2;\n"
      << "Mesh.CharacteristicLengthMin = " << coreMeshSize << ";\n"
      << "Mesh.CharacteristicLengthMax = " << coreMeshSize << ";\n"
      << "Mesh.Algorithm = 5;\n"
      << "Mesh.RecombinationAlgorithm = 1;\n"
      << "Mesh.MshFileVersion = 2.2;\n";
  return 0;
}

bool interfacesMatch(MeshRegions &core, MeshRegions &boundaryLayers) {
  const size_t expectedPoints =
      innerCircumferentialCells + outerCircumferentialCells;
  if (core.m_bndPts.size() != expectedPoints) {
    std::cerr << "Gmsh changed the core-interface discretisation: expected "
              << expectedPoints << " boundary points, got "
              << core.m_bndPts.size() << ".\n";
    return false;
  }
  int pointId = -1;
  for (const int corePointId : core.m_bndPts) {
    if (!boundaryLayers.pointIsExist(core.m_pts[corePointId], pointId)) {
      std::cerr << "Boundary-layer/core interface point mismatch.\n";
      return false;
    }
  }
  return true;
}

int writeFinalMesh(const std::string &coreMesh,
                   const std::string &outputFile) {
  MeshRegions combined("Ring", meshTolerance);
  generateBoundaryLayers(combined);

  MeshRegions core("Core", meshTolerance);
  const double maxCoreElementAngle =
      120.0 / 180.0 * std::acos(-1.0);
  const int status = core.loadFromMsh(coreMesh, maxCoreElementAngle);
  if (status != 0) {
    std::cerr << "Failed to load " << coreMesh << " (error " << status
              << ").\n";
    return 1;
  }
  if (!interfacesMatch(core, combined)) {
    return 1;
  }
  combined.AddRegion(core);
  combined.ResetBndPts();
  combined.rebuildEdgesIndex();

  combined.defineBoundary(reinterpret_cast<void *>(innerBoundary),
                          innerCircumferentialCells, 0, curvedPoints);
  combined.defineBoundary(reinterpret_cast<void *>(outerBoundary),
                          outerCircumferentialCells, 1, curvedPoints);
  combined.CheckMesh();
  combined.outXml(outputFile);
  combined.outCOMPO(outputFile, {0});
  std::cout << "Generated " << outputFile << " with "
            << combined.getCellsNumber() << " elements.\n";
  return 0;
}

void printTopology() {
  std::cout << "Boundary-layer topology: inner="
            << innerCircumferentialCells << " x "
            << innerBoundaryLayerCells() << ", outer="
            << outerCircumferentialCells << " x "
            << outerBoundaryLayerCells() << ".\n";
}

} // namespace

int main(int argc, char *argv[]) {
  if (!parametersAreValid()) {
    return 1;
  }
  if (argc == 1 || (argc >= 2 && std::strcmp(argv[1], "geometry") == 0)) {
    const std::string geoFile = argc >= 3 ? argv[2] : "core.geo";
    printTopology();
    MeshRegions innerLayer("InnerBoundaryLayer", meshTolerance);
    MeshRegions outerLayer("OuterBoundaryLayer", meshTolerance);
    generateInnerBoundaryLayer(innerLayer);
    generateOuterBoundaryLayer(outerLayer);
    return outputGeo(innerLayer, outerLayer, geoFile);
  }
  if (std::strcmp(argv[1], "merge") == 0) {
    if (argc < 3 || argc > 4) {
      std::cerr << "Usage: " << argv[0]
                << " merge core.msh [output.xml]\n";
      return 1;
    }
    printTopology();
    return writeFinalMesh(argv[2], argc == 4 ? argv[3] : "ring.xml");
  }
  std::cerr << "Usage: " << argv[0]
            << " [geometry [core.geo] | merge core.msh [output.xml]]\n";
  return 1;
}
