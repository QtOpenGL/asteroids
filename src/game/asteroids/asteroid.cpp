#include "asteroid.h"

Asteroid::Asteroid(QOpenGLWidget* _glWidget, GLuint _vao, GLuint _shaderProgram, unsigned int _numFaces, float _scale, double _invDiag, QVector3D  _midPoint, QVector3D _initialPosition): Model::Model(_glWidget, _vao, _shaderProgram, _numFaces, _scale, _invDiag, _midPoint, _initialPosition)
{

}
Asteroid::~Asteroid()
{

}
