#ifndef GUNSHOT_H
#define GUNSHOT_H

#include "model.h"

class Gunshot : public Model {
public:
    Gunshot(QOpenGLWidget* _glWidget, GLuint _vao, GLuint _shaderProgram, unsigned int _numFaces, float _scale, double _invDiag, QVector3D _midPoint, QVector3D _initialPosition);
    ~Gunshot();
};

#endif // GUNSHOT_H
