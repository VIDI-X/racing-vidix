#include <BasicLinearAlgebra.h>

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
  { -2, 2},
  { 8, 2},
  { 13.0893, 11.4921},
  { 10.8865, 22.0347},
  { 2.4224, 28.695},
  { -8.3426, 28.356},
  { -16.3714, 21-1774},
  { -17.0973, 10.5172},
  { -12.2316, 1.3637},
};
Matrix<2> tockeInner[] = {
  {-2, 0},
  { 6.4628, 3.3141},
  { 10.825, 11.2961},
  { 9.0317, 20.2111},
  { 1.9275, 25.8878},
  { -7.1635, 25.6699},
  { -13.9875, 19.6594},
  {-15.3515, 10.6686},
  { -10.6173, 2.9045}
};
Polygon stazaInner = Polygon(9, tockeInner);
Polygon stazaOuter = Polygon(9, tockeOuter);

