#ifndef ASTEROID_H
#define ASTEROID_H

#include "model.h"

class Asteroid : public Model {
public:
    Asteroid(QOpenGLWidget* _glWidget, GLuint _vao, GLuint _shaderProgram, unsigned int _numFaces, float _scale, double _invDiag, QVector3D _midPoint, QVector3D _initialPosition);
    ~Asteroid();
};


#endif // ASTEROID_H
