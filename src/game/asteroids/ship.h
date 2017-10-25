#ifndef SHIP_H
#define SHIP_H

#include "model.h"

class Ship : public Model {
public:
    Ship(QOpenGLWidget* _glWidget, GLuint _vao, GLuint _shaderProgram, unsigned int _numFaces, float _scale, double _invDiag, QVector3D _midPoint, QVector3D _initialPosition);
    ~Ship();

    void MoveLeft();
    void MoveRight();
    void MoveUp();
};

#endif // SHIP_H
