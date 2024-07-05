#include "CAD2D/util.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

void transform(double angle1, double angle2, std::vector<std::vector<std::vector<double>>> &pts) {
  double h = pts[0][2][1] - pts[0][1][1];
  double dx = -h * sin(angle1);
  double dy = h * (cos(angle1) - 1.);
  pts[0][2][0] += dx; pts[0][2][1] += dy;
  pts[0][3][0] += dx; pts[0][3][1] += dy;

  dx = -h * sin(angle2);
  dy = h * (cos(angle2) - 1.);
  pts[0][6][0] += dx; pts[0][6][1] += dy;
  pts[0][7][0] += dx; pts[0][7][1] += dy;
}

int main(int argc, char *argv[]) {
  double mesh1 = 0.02, mesh2 = 1.;
  std::vector<std::vector<std::vector<double>>> innerpts = {{   
   {7.6235621662431381e+00, 1.6093750000000000e+01,   0, mesh1},
   {8.1016168823007799e+00, 1.6093750000000000e+01,   0, mesh1},
   {8.4085955103574754e+00, 1.6093750000000000e+01,   0, mesh1},//2
   {8.4085955103574754e+00, 1.6015625000000000e+01,   0, mesh1},//3
   {8.1016168823007799e+00, 1.6015625000000000e+01,   0, mesh1},
   {8.1016168823007799e+00, 1.3984375000000000e+01,   0, mesh1},
   {8.4085955103574754e+00, 1.3984375000000000e+01,   0, mesh1},//6
   {8.4085955103574754e+00, 1.3906250000000000e+01,   0, mesh1},//7
   {8.1016168823007799e+00, 1.3906250000000000e+01,   0, mesh1},
   {7.6235621662431381e+00, 1.3906250000000000e+01,   0, mesh1}}};
  std::vector<std::vector<double>> outer = {
    {-6, -12, 0, mesh2}, {32, -12, 0, mesh2}, {32, 26, 0, mesh2}, {-6, 26, 0, mesh2}
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