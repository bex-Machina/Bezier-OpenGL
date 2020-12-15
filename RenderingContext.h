#pragma once
#include "Matrix.h"
#include <assert.h>


class RenderingContext
{
public:
  RenderingContext();

  void initModelMatrix(bool resetdepth);
  void pushModelMatrix();
  void popModelMatrix();

  void updateMVPs();

  // These all assume the transformation happens to the top-most matrix in the stack (model view)
  void translate(const float x, const float y, const float z);
  void scale(const float x, const float y, const float z);
  void rotateX(const float degs);
  void rotateY(const float degs);
  void rotateZ(const float degs);


  int glprogram;
  int diffusehandle, poshandle, colhandle, model;

  int mvhandle, mvphandle;
  float viewmatrix[16], projectionmatrix[16];
  float mvmatrix[16], mvpmatrix[16]; // combines the model veiw and projection matrix

  //diffusehandle = glGetUniformLocation(glprogram, "u_m_diffuse");

private:
  static const int MAX_MODEL_DEPTH=32;
  int depth;
  float modelmatrix[16*MAX_MODEL_DEPTH];
};

// This constructor is done for you
inline RenderingContext::RenderingContext() : mvhandle(-1), mvphandle(-1), depth(0)
{
  Matrix::setIdentity(viewmatrix);
  Matrix::setIdentity(projectionmatrix);
  Matrix::setIdentity(modelmatrix);
  Matrix::setIdentity(mvmatrix);
  Matrix::setIdentity(mvpmatrix);
}

inline void RenderingContext::initModelMatrix(bool resetdepth)
{
  if (resetdepth)
    depth=0;
// TODO: Set the top model view matrix to the identify
  Matrix::setIdentity(modelmatrix+depth);
}

inline void RenderingContext::pushModelMatrix()
{
  // TODO: copy the current model matrix into the next slot of memory and update depth
  // e.g. "photocopy the top piece of paper and place the copy on top of the original"
    assert((depth / 16) < (MAX_MODEL_DEPTH - 1));
    memcpy(modelmatrix + depth +16, modelmatrix + depth, sizeof(float) * 16);
    depth += 16;
}

inline void RenderingContext::popModelMatrix()
{
  // TODO: decrease the model matrix stack depth by one matrix
    assert(depth > 0);
    depth -= 16;
    
}

inline void RenderingContext::translate(const float x, const float y, const float z)
{
  // TODO: translate the top model view matrix by [x, y, z]
    Matrix::translate(modelmatrix + depth, x, y, z);
}

inline void RenderingContext::scale(const float x, const float y, const float z)
{
  // TODO: scale the top model view matrix by x, y, z along their respective axis
    Matrix::scale(modelmatrix + depth, x, y, z);
}

inline void RenderingContext::rotateX(const float degs)
{
  // TODO: rotate the top model view matrix by degs along the x-axis
    Matrix::rotateX(modelmatrix + depth, degs);
}

inline void RenderingContext::rotateY(const float degs)
{
  // TODO: rotate the top model view matrix by degs along the y-axis
    Matrix::rotateY(modelmatrix + depth, degs);
}

inline void RenderingContext::rotateZ(const float degs)
{
  // TODO: rotate the top model view matrix by degs along the z-axis
    Matrix::rotateZ(modelmatrix + depth, degs);
}


// This method is done for you
inline void RenderingContext::updateMVPs()
{
  Matrix::multiplyMatrix(mvmatrix, viewmatrix, modelmatrix+depth);
  Matrix::multiplyMatrix(mvpmatrix, projectionmatrix, mvmatrix);
}