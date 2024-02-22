#include "Engine.h"

Matrix<3> TRIANGLE_VERTS[] = { { -1, -1, 0 }, { 1, -1, 0 }, { 0, 1, 0 } };
Matrix<3, 1, int> TRINAGLE_TRIS[] = { { 0, 1, 2 } };
Object TRIANGLE = Object(3, TRIANGLE_VERTS, 1, TRINAGLE_TRIS);

Matrix<3> CUBE_VERTS[] = {
  { -0.5, -0.5, -0.5 },
  { 0.5, -0.5, -0.5 },
  { -0.5, 0.5, -0.5 },
  { 0.5, 0.5, -0.5 },
  { -0.5, -0.5, 0.5 },
  { 0.5, -0.5, 0.5 },
  { -0.5, 0.5, 0.5 },
  { 0.5, 0.5, 0.5 }
};
Matrix<3, 1, int> CUBE_TRIS[] = {
  { 1, 3, 2 },
  { 0, 5, 1 },
  { 1, 7, 3 },
  { 2, 3, 6 },
  { 0, 2, 4 },
  { 4, 7, 5 },
  { 4, 6, 7 },
  { 0, 4, 5 },
  { 2, 6, 4 },
  { 3, 7, 6 },
  { 1, 5, 7 }
};
Object CUBE = Object(8,
                     CUBE_VERTS,
                     12,
                     CUBE_TRIS);