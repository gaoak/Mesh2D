#include "CAD2D/util.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

void transform(double angle1, double angle2, std::vector<std::vector<std::vector<double>>> &pts) {
  double h = pts[0][2][0] - pts[0][1][0];
  double dx = h * (cos(angle2) - 1.);
  double dy = h * sin(angle2);
  pts[0][2][0] += dx; pts[0][2][1] += dy;
  pts[0][3][0] += dx; pts[0][3][1] += dy;

  dx = h * (cos(angle1) - 1.);
  dy = h * sin(angle1);
  pts[0][6][0] += dx; pts[0][6][1] += dy;
  pts[0][7][0] += dx; pts[0][7][1] += dy;
}

int main(int argc, char *argv[]) {
  double mesh1 = 0.02, mesh2 = 2.;
  std::vector<std::vector<std::vector<double>>> innerpts = {{   
   {1.0000000000000000e+01,  9.5041909293572910e+00,   0, mesh1},
   {1.0937500000000000e+01,  9.5041909293572910e+00,   0, mesh1},
   {1.1562500000000000e+01,  9.5041909293572910e+00,   0, mesh1},//2
   {1.1562500000000000e+01,  9.5397556749681112e+00,   0, mesh1},//3
   {1.0937500000000000e+01,  9.5397556749681112e+00,   0, mesh1},
   {1.0937500000000000e+01,  1.0460244325031887e+01,   0, mesh1},
   {1.1562500000000000e+01,  1.0460244325031887e+01,   0, mesh1},//6
   {1.1562500000000000e+01,  1.0495809070642709e+01,   0, mesh1},//7
   {1.0937500000000000e+01,  1.0495809070642709e+01,   0, mesh1},
   {1.0000000000000000e+01,  1.0495809070642709e+01,   0, mesh1}}};
  std::vector<std::vector<double>> outer = {
    {-20, -20, 0, mesh2}, {40, -20, 0, mesh2}, {40, 40, 0, mesh2}, {-20, 40, 0, mesh2}
  };
  double a1 = 0., a2 = 0.;
  std::string fname("body.geo");
  if(argc>3) {
    a1 = std::stod(argv[1]);
    a2 = std::stod(argv[2]);
    fname = argv[3];
  }
  std::cout << "angles " << a1 << ", " << a2 << std::endl;
  transform(a1, a2, innerpts);
  OutGeo(fname, outer, innerpts);
  return 0;
}