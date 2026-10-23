#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

// Explicit element boundaries for the Ha=1000 square duct.
// Change these arrays directly to change the mesh; no growth-rate fitting is used.
const std::array<double, 8> x_nodes = {{
    -1.0, -0.98, -0.94, -0.85, 0.85, 0.94, 0.98, 1.0
}};
const std::array<double, 8> y_nodes = {{
    -1.0, -0.9985, -0.996, -0.99, 0.99, 0.996, 0.9985, 1.0
}};
const int Nx = static_cast<int>(x_nodes.size()) - 1;
const int Ny = static_cast<int>(y_nodes.size()) - 1;

// RectRegion samples s uniformly; map each parameter interval to one element.
template <std::size_t N>
double coordinateFromNodes(const std::array<double, N>& nodes, double s) {
    static_assert(N >= 2, "At least two element-boundary coordinates are required");
    const double index = 0.5 * (std::max(-1.0, std::min(1.0, s)) + 1.0) * (N - 1);
    const std::size_t lower = static_cast<std::size_t>(std::floor(index));
    if (lower >= N - 1) {
        return nodes.back();
    }
    const double weight = index - lower;
    return (1.0 - weight) * nodes[lower] + weight * nodes[lower + 1];
}

// Keep the existing callback names and boundary directions.
inline std::vector<double> edge03(double s) {
    return {coordinateFromNodes(x_nodes, s), y_nodes.front()};  // Bottom.
}
inline std::vector<double> edge69(double s) {
    return {coordinateFromNodes(x_nodes, s), y_nodes.back()};   // Top.
}
inline std::vector<double> edge06(double s) {
    return {x_nodes.front(), coordinateFromNodes(y_nodes, s)};  // Left.
}
inline std::vector<double> edge39(double s) {
    return {x_nodes.back(), coordinateFromNodes(y_nodes, s)};   // Right.
}

#endif
