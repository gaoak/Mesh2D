#include <iostream>
#include <vector>

#include "CAD2D/MeshRegions.h"
#include "CAD2D/RectRegion.h"

#include "edgefunctions.h"

int main() {
    if (!validateParameters()) {
        return 1;
    }
    initialiseEdges();

    std::vector<void *> edges;
    edges.push_back(reinterpret_cast<void *>(edgeBottom));
    edges.push_back(reinterpret_cast<void *>(edgeRight));
    edges.push_back(reinterpret_cast<void *>(edgeTop));
    edges.push_back(reinterpret_cast<void *>(edgeLeft));

    RectRegion channel(edges, "HartmannChannel", true, meshTolerance);
    channel.MeshGen(Nx, leftEdge.m_N, eIsoparametric);

    MeshRegions mesh("HartmannChannel", meshTolerance);
    mesh.AddRegion(channel);
    mesh.ResetBndPts();
    mesh.rebuildEdgesIndex();

    // XML composites: C0 inlet, C1 outlet, C2 lower wall, C3 upper wall.
    mesh.defineBoundary(reinterpret_cast<void *>(edgeLeft), leftEdge.m_N, 0);
    mesh.defineBoundary(reinterpret_cast<void *>(edgeRight), rightEdge.m_N, 1);
    mesh.defineBoundary(reinterpret_cast<void *>(edgeBottom), Nx, 2);
    mesh.defineBoundary(reinterpret_cast<void *>(edgeTop), Nx, 3);

    mesh.CheckMesh();
    mesh.outXml("hartmann.xml");
    mesh.outCOMPO("hartmann.xml", std::vector<int>(1, 0));

    std::cout << "Generated hartmann.xml: " << Nx << " x "
              << leftEdge.m_N << " quadrilateral elements\n"
              << "  y partitions: " << NyLower << " + " << NyCore
              << " + " << NyUpper << '\n';
    return 0;
}
