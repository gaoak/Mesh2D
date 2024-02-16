#ifndef EDGEFUNCTIONS_H
#define EDGEFUNCTIONS_H
#include "params.h"

double g_ptsC[20][2];

void transform(std::vector<double> &p, double AoA) {
  double x = p[0], y = p[1];
  p[0] = x * cos(AoA) + y * sin(AoA);
  p[1] = -x * sin(AoA) + y * cos(AoA);
}

int InitPts() {
  // airfoil
  g_ptsC[0][0] = radiusL * cos(theta0);
  g_ptsC[0][1] = radiusL * sin(theta0);
  g_ptsC[1][0] = radiusL * cos(theta1);
  g_ptsC[1][1] = radiusL * sin(theta1);
  g_ptsC[2][0] = radiusL * cos(theta2);
  g_ptsC[2][1] = radiusL * sin(theta2);

  g_ptsC[3][0] = 0.;
  g_ptsC[3][1] = 0.;
  g_ptsC[4][0] = 0.;
  g_ptsC[4][1] = 0.;
  return 0;
}

std::vector<double> edge0(double s) {
  double t = -M_PI * s;
  double a = ChordLen * 0.5;
  double b = Thickness * 0.5;
  std::vector<double> res(2, 0.);
  res[0] = a * cos(t);
  res[1] = b * sin(t);
  transform(res, AoA0);
  res[0] += g_ptsC[0][0];
  res[1] += g_ptsC[0][1];
  return res;
}
std::vector<double> edge1(double s) {
  double t = -M_PI * s;
  double a = ChordLen * 0.5;
  double b = Thickness * 0.5;
  std::vector<double> res(2, 0.);
  res[0] = a * cos(t);
  res[1] = b * sin(t);
  transform(res, AoA1);
  res[0] += g_ptsC[1][0];
  res[1] += g_ptsC[1][1];
  return res;
}
std::vector<double> edge2(double s) {
  double t = -M_PI * s;
  double a = ChordLen * 0.5;
  double b = Thickness * 0.5;
  std::vector<double> res(2, 0.);
  res[0] = a * cos(t);
  res[1] = b * sin(t);
  transform(res, AoA2);
  res[0] += g_ptsC[2][0];
  res[1] += g_ptsC[2][1];
  return res;
}

std::vector<double> edge3(double s) {
  double t = -M_PI * s;
  std::vector<double> res(2, 0.);
  res[0] = g_ptsC[3][0] + radiusNeari * cos(t);
  res[1] = g_ptsC[3][1] + radiusNeari * sin(t);
  return res;
}
std::vector<double> edge4(double s) {
  double t = M_PI * s;
  std::vector<double> res(2, 0.);
  res[0] = g_ptsC[4][0] + radiusFar * cos(t);
  res[1] = g_ptsC[4][1] + radiusFar * sin(t);
  return res;
}

std::vector<double> g_boundingbox0;
std::vector<double> g_boundingbox1;
std::vector<double> g_boundingbox2;
bool toremove(std::vector<double> p) {
  double eps = g_boundingbox0[g_boundingbox0.size() - 1];
  bool inside = true;
  for (int i = 0; i < 2; ++i) {
    if (p[i] < g_boundingbox0[2 * i] - eps ||
        p[i] > g_boundingbox0[2 * i + 1] + eps) {
      inside = false;
      break;
    }
  }
  if (inside)
    return true;
  inside = true;
  eps = g_boundingbox1[g_boundingbox1.size() - 1];
  for (int i = 0; i < 2; ++i) {
    if (p[i] < g_boundingbox1[2 * i] - eps ||
        p[i] > g_boundingbox1[2 * i + 1] + eps) {
      inside = false;
      break;
    }
  }
  if (inside)
    return true;
  inside = true;
  eps = g_boundingbox2[g_boundingbox2.size() - 1];
  for (int i = 0; i < 2; ++i) {
    if (p[i] < g_boundingbox2[2 * i] - eps ||
        p[i] > g_boundingbox2[2 * i + 1] + eps) {
      inside = false;
      break;
    }
  }
  return inside;
}

#endif
