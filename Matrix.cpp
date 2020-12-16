#include "Matrix.h"



// Multiplies the lhs matrix by the rhs matrix and stores the result in result: result = lhs x rhs
void Matrix::multiplyMatrix(float* result, const float* lhs, const float* rhs)
{
    //memset(result, 0, sizeof(float) * 16);
 /*   for (size_t row = 0; row < 4; row++) {
        for (size_t col = 0; col < 4; col++) {
            for (size_t traverse = 0; traverse < 4; traverse++) {
                result[row + 4 * col] += lhs[row + 4 * traverse] * rhs[traverse + 4 * col];
            }
        }
    }*/

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float n = 0; 
            for (int k = 0; k < 4; k++)
            {
                n += lhs[i + (k * 4)] * rhs[k + (j * 4)];
            }
            result[i + (j * 4)] = n; 
        }
    }
}

// Applys a translation matrix to the matrix provided.  The result is stored back in the same block of memory)
// e.g. matrix = matrix x translation (where the translation matrix is created using tx, ty, and tz to represent the translation vector
void  Matrix::translate(float* matrix, const float tx, const float ty, const float tz)
{
    matrix[12] += (matrix[0] * tx) + (matrix[4] * ty) + (matrix[8] * tz);
    matrix[13] += (matrix[1] * tx) + (matrix[5] * ty) + (matrix[9] * tz);
    matrix[14] += (matrix[2] * tx) + (matrix[6] * ty) + (matrix[10] * tz);
    matrix[15] += (matrix[3] * tx) + (matrix[7] * ty) + (matrix[11] * tz);

}




// Applys a scale matrix to the matrix provided.  The result is stored back in the same block of memory
// e.g. matrix = matrix x scale (where the scale matrix is created using sx, sy, and sz to represent the axis scaling values)
void  Matrix::scale(float* matrix, const float sx, const float sy, const float sz)
{
    for (int i = 0; i < 4; i++)
    {
        matrix[i] *= sx;
        matrix[i + 4] *= sy;
        matrix[i + 8] *= sz;
    }
}


/*
  All 4x4 matrices are assumed to be stored as a 1D array as illustrated below:
  m[0] m[4] m[8]  m[12]
  m[1] m[5] m[9]  m[13]
  m[2] m[6] m[10] m[14]
  m[3] m[7] m[11] m[15]
  */
// Applys a rotation about the x-axis matrix to the matrix provided.  The result is stored back in the same block of memory
// e.g. matrix = matrix x rotationX (where the rotation matrix is created by the angle provided in degrees)
void  Matrix::rotateX(float* matrix, const float degs)
{
    float rad;
    rad = Matrix::degreesToRadians(degs);
    float rotate[16];
    float tmp[16];
    memcpy(tmp, matrix, sizeof(float) * 16);
    Matrix::setIdentity(rotate);
    rotate[5] = cosf(rad);
    rotate[6] = sinf(rad);
    rotate[9] = sinf(rad) * -1.0f;
    rotate[10] = cosf(rad);

    multiplyMatrix(matrix, tmp, rotate);

}

// Applys a rotation about the y-axis matrix to the matrix provided.  The result is stored back in the same block of memory
// e.g. matrix = matrix x rotationY (where the rotation matrix is created by the angle provided in degrees)
void  Matrix::rotateY(float* matrix, const float degs)
{

    float rad = Matrix::degreesToRadians(degs);
    float rotate[16];
    float tmp[16];
    memcpy(tmp, matrix, sizeof(float) * 16);
    Matrix::setIdentity(rotate);
    rotate[0] = cosf(rad);
    rotate[8] = sinf(rad);
    rotate[2] = sinf(rad) * -1.0f;
    rotate[10] = cosf(rad);

    multiplyMatrix(matrix, tmp, rotate);

}

/*
  All 4x4 matrices are assumed to be stored as a 1D array as illustrated below:
  m[0] m[4] m[8]  m[12]
  m[1] m[5] m[9]  m[13]
  m[2] m[6] m[10] m[14]
  m[3] m[7] m[11] m[15]
  */
// Applys a rotation about the z-axis matrix to the matrix provided.  The result is stored back in the same block of memory
// e.g. matrix = matrix x rotationZ (where the rotation matrix is created by the angle provided in degrees)
void  Matrix::rotateZ(float* matrix, const float degs)
{


    float rad;
    rad = Matrix::degreesToRadians(degs);
    float rotate[16];
    float tmp[16];
    memcpy(tmp, matrix, sizeof(float) * 16);
    Matrix::setIdentity(rotate);
    rotate[0] = cosf(rad);
    rotate[4] = sinf(rad);
    rotate[1] = sinf(rad)*-1.0f;
    rotate[5] = cosf(rad);

    multiplyMatrix(matrix, tmp,rotate);
}


/*
  All 4x4 matrices are assumed to be stored as a 1D array as illustrated below:
  m[0] m[4] m[8]  m[12]
  m[1] m[5] m[9]  m[13]
  m[2] m[6] m[10] m[14]
  m[3] m[7] m[11] m[15]
  */
void Matrix::setOrtho(float* matrix, float left, float right, float bottom, float top, float near, float far)
{
    memset(matrix, 0, sizeof(float) * 16);
    matrix[0] = 2 / (right - left);
    matrix[5] = 2 / (top - bottom);
    matrix[10] = -(2 / (far - near));
    matrix[12] = -((right + left) / (right - left));
    matrix[13] = -((top + bottom) / (top - bottom));
    matrix[14] = -((far + near) / (far - near));
    matrix[15] = 1;
}