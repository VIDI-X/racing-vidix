#include <BasicLinearAlgebra.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#define TFT_WIDTH 320
#define TFT_HEIGHT 240
using namespace BLA;

class Object {
public:
  int numOfVerts;
  Matrix<3> *verteces;
  int numOfTris;
  Matrix<3, 1, int> *triangles;

  Matrix<3> position;
  Matrix<3> rotation;
  Matrix<3> scale;

  Object(Object *other){
    this->numOfVerts = other->numOfVerts;
    this->verteces = (Matrix<3>*)malloc(this->numOfVerts * sizeof(Matrix<3>));
    for (int i = 0; i < this->numOfVerts; i++){
      this->verteces[i] = other->verteces[i];
    }
    this->numOfTris = other->numOfTris;
    this->triangles = (Matrix<3, 1, int>*)malloc(this->numOfTris * sizeof(Matrix<3, 1, int>));
    for (int i = 0; i < this->numOfTris; i++){
      this->triangles[i] = other->triangles[i];
    }
    this->position = other->position;
    this->rotation = other->rotation;
    this->scale = other->scale;
  }

  Object(int numOfVerts, Matrix<3> verteces[], int numOfTris, Matrix<3, 1, int> triangles[]) {
    this->numOfVerts = numOfVerts;
    this->verteces = verteces;
    this->numOfTris = numOfTris;
    this->triangles = triangles;

    position.Fill(0);
    rotation.Fill(0);
    scale.Fill(1);
  }

  Object(int numOfVerts, Matrix<3> verteces[], int numOfTris, Matrix<3, 1, int> triangles[], Matrix<3> position, Matrix<3> rotation, Matrix<3> scale) {
    this->numOfVerts = numOfVerts;
    this->verteces = verteces;
    this->numOfTris = numOfTris;
    this->triangles = triangles;

    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
  }

  Matrix<4, 4> getObjectToWorldMatrix() {
    Matrix<4, 4> translationMat = {
      1, 0, 0, position(0),
      0, 1, 0, position(1),
      0, 0, 1, position(2),
      0, 0, 0, 1
    };
    Matrix<4, 4> scaleMat = {
      scale(0), 0, 0, 0,
      0, scale(1), 0, 0,
      0, 0, scale(2), 0,
      0, 0, 0, 1
    };
    Matrix<4, 4> rotateMatX = {
      1, 0, 0, 0,
      0, cos(rotation(0)), -sin(rotation(0)), 0,
      0, sin(rotation(0)), cos(rotation(0)), 0,
      0, 0, 0, 1
    };
    Matrix<4, 4> rotateMatY = {
      cos(rotation(1)), 0, sin(rotation(1)), 0,
      0, 1, 0, 0,
      -sin(rotation(1)), 0, cos(rotation(1)), 0,
      0, 0, 0, 1
    };
    Matrix<4, 4> rotateMatZ = {
      cos(rotation(2)), -sin(rotation(2)), 0, 0,
      sin(rotation(2)), cos(rotation(2)), 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };

    return translationMat * rotateMatX * rotateMatY * rotateMatZ * scaleMat;
  }
};

class Camera {
public:
  Matrix<3> position;
  Matrix<3> rotation;
  float znear, zfar;
  float fov;
  const float aspect = TFT_WIDTH / TFT_HEIGHT;

  Camera() {
    position.Fill(0);
    rotation.Fill(0);
    znear = 0.1;
    zfar = 1000;
    fov = PI / 2;
  }

  Camera(Matrix<3> position, Matrix<3> rotation) {
    this->position = position;
    this->rotation = rotation;
    znear = 0.1;
    zfar = 1000;
    fov = PI / 2;
  }

  Camera(Matrix<3> position, Matrix<3> rotation, float znear, float zfar, float fov) {
    this->position = position;
    this->rotation = rotation;
    this->znear = znear;
    this->zfar = zfar;
    this->fov = fov;
  }

  Matrix<4, 4> getWorldToViewMat() {
    Matrix<4, 4> translationMat = {
      1, 0, 0, -position(0),
      0, 1, 0, -position(1),
      0, 0, 1, -position(2),
      0, 0, 0, 1
    };
    Matrix<4, 4> rotateMatX = {
      1, 0, 0, 0,
      0, cos(-rotation(0)), -sin(-rotation(0)), 0,
      0, sin(-rotation(0)), cos(-rotation(0)), 0,
      0, 0, 0, 1
    };
    Matrix<4, 4> rotateMatY = {
      cos(-rotation(1)), 0, sin(-rotation(1)), 0,
      0, 1, 0, 0,
      -sin(-rotation(1)), 0, cos(-rotation(1)), 0,
      0, 0, 0, 1
    };
    Matrix<4, 4> rotateMatZ = {
      cos(-rotation(2)), -sin(-rotation(2)), 0, 0,
      sin(-rotation(2)), cos(-rotation(2)), 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };

    return rotateMatZ * rotateMatY * rotateMatX * translationMat;
  }

  Matrix<4, 4> getOrthoMat(float width, float height) {
    Matrix<4, 4> ortho = {
      1 / width, 0, 0, 0,
      0, 1 / height, 0, 0,
      0, 0, -(2 / (zfar - znear)), -((zfar + znear) / (zfar - znear)),
      0, 0, 0, 1
    };

    return ortho;
  }

  Matrix<4, 4> getPerspectiveMat() {
    float fovY = fov;
    float fovX = aspect * fov;

    Matrix<4, 4> perspective = {
      1 / tan(fovX / 2), 0, 0, 0,
      0, 1 / tan(fovY / 2), 0, 0,
      0, 0, -(zfar + znear) / (zfar - znear), -2 * (zfar * znear) / (zfar - znear),
      0, 0, -1, 0
    };

    return perspective;
  }

  void drawObject(Object &object, Adafruit_ILI9341 &tft, int color) {
    Matrix<4, 4> objectToWorldMat = object.getObjectToWorldMatrix();
    Matrix<4, 4> worldToViewMat = getWorldToViewMat();
    Matrix<4, 4> perspectiveMat = getPerspectiveMat();
    Matrix<4, 4> objectToScreenMat = perspectiveMat * worldToViewMat * objectToWorldMat;

    Matrix<4> verteces[object.numOfVerts];
    for (int i = 0; i < object.numOfVerts; i++) {
      verteces[i] = { object.verteces[i](0), object.verteces[i](1), object.verteces[i](2), 1 };
      verteces[i] = objectToScreenMat * verteces[i];

      if (verteces[i](3) != 0)
        verteces[i] /= verteces[i](3);
      verteces[i](0) = (verteces[i](0) + 1) / 2;
      verteces[i](0) = verteces[i](0) * TFT_WIDTH;
      verteces[i](1) = (verteces[i](1) + 1) / 2;
      verteces[i](1) = (verteces[i](1) * TFT_HEIGHT);
    }

    for (int i = 0; i < object.numOfTris; i++){
      tft.drawTriangle(
        verteces[object.triangles[i](0)](0), verteces[object.triangles[i](0)](1),
        verteces[object.triangles[i](1)](0), verteces[object.triangles[i](1)](1),
        verteces[object.triangles[i](2)](0), verteces[object.triangles[i](2)](1),
        color
      );
    }
  }
};