#include "model.h"

Model::Model(QOpenGLWidget* _glWidget, GLuint _vao, GLuint _shaderProgram, unsigned int _numFaces, float _scale, double _invDiag, QVector3D _midPoint, QVector3D _initialPosition)
{
    glWidget = _glWidget;
    vao = _vao;
    shaderProgram = _shaderProgram;
    numFaces = _numFaces;

    scale = _scale;
    invDiag = _invDiag;
    midPoint = _midPoint;

    this->hitBoxRadius = invDiag*scale;

    this->initialPosition = _initialPosition;
    this->currentPosition = QVector3D(0.0, 0.0, 0.0);

    this->angle=0.0;
    this->color=1.0;

    //glWidget->makeCurrent();

    initializeOpenGLFunctions();
}

Model::~Model(){ }


void Model::drawModel()
{
    modelMatrix.setToIdentity(); //M=I
    modelMatrix.translate(currentPosition);
    modelMatrix.rotate(angle, 0.0, 0.0, 1.0); // Rotação apenas em Z (plano XY)
    modelMatrix.scale(invDiag*scale, invDiag*scale, invDiag*scale); //M=I*S
    modelMatrix.translate(-midPoint); //M=I*S*T

    glBindVertexArray(vao);
    glUseProgram(shaderProgram);

    GLuint locModelMatrix = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(locModelMatrix, 1, GL_FALSE, modelMatrix.data());

    GLuint locColor = glGetUniformLocation(shaderProgram, "color");
    glUniform1f(locColor, color);

    glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
}


//Extra Functions:

bool Model::CalculateColision(Model* other)
{   
    float distance = this->currentPosition.distanceToPoint(other->currentPosition);
    return distance < (this->hitBoxRadius + other->hitBoxRadius);
}
