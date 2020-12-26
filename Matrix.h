#pragma once
#include <math.h> // remember to add _USE_MATH_DEFINES to the project settings
#include <memory.h>

/*
  All 4x4 matrices are assumed to be stored as a 1D array as illustrated below:
  m[0] m[4] m[8]  m[12]
  m[1] m[5] m[9]  m[13]
  m[2] m[6] m[10] m[14]
  m[3] m[7] m[11] m[15]
  */
class Matrix
{
public:
  // Sets up the indentify matrix
  static void setIdentity(float* matrix);

  // Multiplies the lhs matrix by the rhs matrix and stores the result in result: result = lhs x rhs
  static void multiplyMatrix(float* result, const float* lhs, const float* rhs);

  // Applys a translation matrix to the matrix provided.  The result is stored back in the same block of memory)
  // e.g. matrix = matrix x translation (where the translation matrix is created using tx, ty, and tz to represent the translation vector
  static void translate(float* matrix, const float tx, const float ty, const float tz);

  // Applys a scale matrix to the matrix provided.  The result is stored back in the same block of memory
  // e.g. matrix = matrix x scale (where the scale matrix is created using sx, sy, and sz to represent the axis scaling values)
  static void scale(float* matrix, const float sx, const float sy, const float sz);

  // Applys a rotation about the x-axis matrix to the matrix provided.  The result is stored back in the same block of memory
  // e.g. matrix = matrix x rotationX (where the rotation matrix is created by the angle provided in degrees)
  static void rotateX(float* matrix, const float degs);

  // Applys a rotation about the y-axis matrix to the matrix provided.  The result is stored back in the same block of memory
  // e.g. matrix = matrix x rotationY (where the rotation matrix is created by the angle provided in degrees)
  static void rotateY(float* matrix, const float degs);

  // Applys a rotation about the z-axis matrix to the matrix provided.  The result is stored back in the same block of memory
  // e.g. matrix = matrix x rotationZ (where the rotation matrix is created by the angle provided in degrees)
  static void rotateZ(float* matrix, const float degs);

  static void setOrtho(float* matrix, float left, float right, float bottom, float top, float near, float far);

private:
  // helper method to convert between degrees and radians
    static float degreesToRadians(const float degrees);
  
};

inline float Matrix::degreesToRadians(const float degrees)
{
  return degrees*(float) (M_PI/180.0);
}

inline void Matrix::setIdentity(float* matrix)
{
  memset(matrix, 0, sizeof(float)*16);
  matrix[0]=matrix[5]=matrix[10]=matrix[15]=1;
}


