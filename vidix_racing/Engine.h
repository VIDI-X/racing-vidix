#include <BasicLinearAlgebra.h>

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

    Object(int numOfVerts, Matrix<3> verteces[], int numOfTris, Matrix<3, 1, int> triangles[]){
      this->numOfVerts = numOfVerts;
      this->verteces = verteces;
      this->numOfTris = numOfTris;
      this->triangles = triangles;

      position.Fill(0);
      rotation.Fill(0);
      scale.Fill(0);
    }

    Object(int numOfVerts, Matrix<3> verteces[], int numOfTris, Matrix<3, 1, int> triangles[], Matrix<3> position, Matrix<3> rotation, Matrix<3> scale){
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

      return translationMat * scaleMat * rotateMatX * rotateMatY * rotateMatZ;
    }
};

class Camera{
  public:
    Matrix<3> position;
    Matrix<3> rotation;
    Matrix<3> scale;
    float znear, zfar;

    Camera(){
      position.Fill(0);
      rotation.Fill(0);
      scale.Fill(0);
      znear = 0.1;
      zfar = 1000;
    }

    Camera(Matrix<3> position, Matrix<3> rotation, Matrix<3> scale){
      this->position = position;
      this->rotation = rotation;
      this->scale = scale;
      znear = 0.1;
      zfar = 1000;
    }

    Camera(Matrix<3> position, Matrix<3> rotation, Matrix<3> scale, float znear, float zfar){
      this->position = position;
      this->rotation = rotation;
      this->scale = scale;
      this->znear = znear;
      this->zfar = zfar;
    }

    Matrix<4, 4> getWorldToViewMat() {
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

      return Inverse(translationMat * scaleMat * rotateMatX * rotateMatY * rotateMatZ);
    }

    Matrix<4, 4> getOrthoMat(float width, float height){
      Matrix<4, 4> ortho = {
        1 / width, 0, 0, 0,
        0, 1 / height, 0, 0,
        0, 0, -(2 / (zfar - znear)), -((zfar + znear) / (zfar - znear)),
        0, 0, 0, 1
      };

      return ortho;
    }
};