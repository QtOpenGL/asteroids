#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include <QUuid>
#include "ship.h"
#include "gunshot.h"
#include "asteroid.h"
#include "physics.h"
#include "shadercompiler.h"
#include "buffercreator.h"

class ModelFactory {
public:
    ModelFactory(QOpenGLWidget* _glWidget);
    ~ModelFactory();

    QOpenGLWidget* glWidget;

    GLuint shaderProgramDefault = 0;
    GLuint shaderProgramEnergy  = 0;

    GLuint vaoShip = 0;
    GLuint vaoGunshot = 0;
    GLuint vaoAsteroid = 0;

    std::shared_ptr<OffModel> shipOffModel = nullptr;
    std::shared_ptr<OffModel> gunshotOffModel = nullptr;
    std::shared_ptr<OffModel> asteroidOffModel = nullptr;

    void Build();

    std::shared_ptr<Ship> GetShipInstance();
    std::shared_ptr<Ship> GetScaledShipInstance(float size);
    std::shared_ptr<Gunshot> GetGunshotInstance(Ship* ship);
    std::shared_ptr<Asteroid> GetAsteroidInstance();
};


#endif // MODELFACTORY_H
