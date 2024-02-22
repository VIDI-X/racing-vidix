#include <BasicLinearAlgebra.h>
using namespace BLA;

class Polygon {
public:
  int numOfPoints;
  Matrix<2> *points;

  Polygon(int numOfPoints, Matrix<2> points[]) {
    this->numOfPoints = numOfPoints;
    this->points = points;
  }

  bool inside(Matrix<2> point) {
    float x = point(0);
    float y = point(1);
    Matrix<2> infPoint = { point(0), 100000 };
    bool inside = true;

    Matrix<2> p1 = points[0], p2;
    for (int i = 1; i <= numOfPoints; i++) {
      p2 = points[i % numOfPoints];

      if (y > min(p1(2), p2(2))) {
        if (y <= max(p1(2), p2(2))) {
          if (x <= max(p1(1), p2(1))) {
            float x_intersection = (y - p1(2)) * (p2(1) - p1(1)) / (p2(2) - p1(2)) + p1(1);

            if (p1(1) == p2(1)
                || x <= x_intersection) {
              inside = !inside;
            }
          }
        }
      }
    }

    return inside;
  }
};

Matrix<2> tockeOuter[] = {
  { -20, -20 },
  { 80, 20 },
  { 130.893, 114.921 },
  { 108.865, 220.347 },
  { 24.224, 286.95 },
  { -83.426, 283.56 },
  { -163.714, 211.774 },
  { -170.973, 105.172 },
  { -122.316, 13.637 },
};
Matrix<2> tockeInner[] = {
  { -20, 0 },
  { 64.628, 33.141 },
  { 108.25, 112.961 },
  { 90.317, 202.111 },
  { 19.275, 258.878 },
  { -71.635, 256.699 },
  { -139.875, 196.594 },
  { -153.515, 106.686 },
  { -106.173, 29.045 }
};
Polygon stazaInner = Polygon(9, tockeInner);
Polygon stazaOuter = Polygon(9, tockeOuter);