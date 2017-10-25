#ifndef MODEL_H
#define MODEL_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <limits>
#include <iostream>
#include <memory>
#include "offmodel.h"
#include "physics.h"

class Model : public QOpenGLExtraFunctions {
public:
    Model(QOpenGLWidget* _glWidget, GLuint _vao, unsigned int _numFaces, GLuint _shaderProgram, float _scale, double _invDiag, QVector3D _midPoint, QVector3D _initialPosition);
    ~Model();

    QOpenGLWidget* glWidget;

    GLuint vao = 0;
    GLuint shaderProgram = 0;

    void drawModel();
    void drawModel(float arg);

    QMatrix4x4 modelMatrix;

    QVector3D initialPosition;
    QVector3D currentPosition;

    QString id;

    unsigned int numFaces;
    float scale;
    double invDiag;
    QVector3D midPoint;

    float hitBoxRadius;
    float angle;
    float color;

    bool CalculateColision(Model* other);
};

#endif // MODEL_H
