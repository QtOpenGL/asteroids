#include "ship.h"

Ship::Ship(QOpenGLWidget* _glWidget, GLuint _vao, GLuint _shaderProgram, unsigned int _numFaces, float _scale, double _invDiag, QVector3D _midPoint, QVector3D _initialPosition): Model::Model(_glWidget, _vao, _shaderProgram, _numFaces, _scale, _invDiag, _midPoint, _initialPosition)
{
    this->hitBoxRadius = (invDiag*scale)/2;
}

Ship::~Ship()
{
}

void Ship::MoveLeft(){
    if(this)
        this->angle+= Physics::shipMoveAngle;
}

void Ship::MoveRight(){
    if(this)
        this->angle-= Physics::shipMoveAngle;
}

void Ship::MoveUp(){
    if(this){
        this->currentPosition = Physics::GetNextLinearMoviment
                (
                    this->currentPosition.x(),
                    this->currentPosition.y(),
                    this->angle,
                    Physics::shipAngleCorrection,
                    Physics::shipMovimentFactor
                 );
    }
}
