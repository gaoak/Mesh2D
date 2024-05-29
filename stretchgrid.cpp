#include "CAD2D/LineEdge.h"
#include "CAD2D/util.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;
// griddefine.dat
// N -15 -1 0.05 0.01 1.1
// U -1 6 0.01
// N 6 20 0.01 0.05 1.05
int main() {
  ifstream ifile("griddefine.dat");
  if (!ifile.is_open()) {
    cout << "Failed in openning file griddefine.dat." << endl;
  }
  char buffer[1000];
  double globalpts[2][3] = {{0., 0., 0.}, {0., 0., 0.}};
  LineEdge edge;
  ifile.getline(buffer, sizeof(buffer));
  vector<double> pts;
  int count = 0;
  while (!ifile.eof()) {
    std::vector<double> value;
    parserDouble(buffer + 1, value);
    globalpts[0][0] = value[0];
    globalpts[1][0] = value[1];
    if (buffer[0] = 'U' && value.size() == 3) {
      int N = (value[1] - value[0]) / value[2] + 0.5;
      edge = LineEdge(globalpts[0], globalpts[1], N, UNIFORM, 0., 0.);
    } else if (buffer[0] = 'N' && value.size() == 5) {
      int refinetype;
      if (value[2] < value[3]) {
        refinetype = SMALLSTRETCH0;
      } else {
        refinetype = SMALLSTRETCH1;
      }
      edge = LineEdge(globalpts[0], globalpts[1], refinetype, value[2],
                      value[3], value[4]);
    }
    double ds = 2. / edge.m_N;
    for (int i = 0; i < edge.m_N; ++i) {
      pts.push_back(edge.Evaluate(ds * i - 1.)[0]);
    }
    ifile.getline(buffer, sizeof(buffer));
  }
  pts.push_back(edge.Evaluate(1.)[0]);
  ifile.close();
  // output grid.dat
  ofstream ofile("xgrid.dat");
  double maxratio = -1;
  if (!ofile.is_open()) {
    cout << "Failed in openning file xgrid.dat" << endl;
  }
  for (size_t i = 0; i < pts.size(); ++i) {
    ofile << i + 1 << " " << std::scientific << std::setprecision(17) << pts[i]
          << " ";
    if (i == 0 || i + 1 == pts.size()) {
      ofile << 1. << " " << 0. << endl;
    } else {
      double ds0 = pts[i + 1] - pts[i];
      double ds1 = pts[i] - pts[i - 1];
      double ratio = ds0 / ds1;
      if(ds0<=1E-6 || ds1<=1E-6) {
        cout << "error: too small segment at " << pts[i-1] << ", " << pts[i] << ", " << pts[i+1] << endl;
      }
      if (ratio < 1.) {
        ratio = ds1 / ds0;
      }
      if (maxratio < ratio) {
        maxratio = ratio;
      }
      ofile << ratio << " " << ratio - 1. << endl;
    }
  }
  ofile.close();
  cout << "Mesh generation finishes with " << pts.size()
       << " points, max stretch ratio " << maxratio << endl;
  return 0;
}