#include "modelfactory.h"

ModelFactory::ModelFactory(QOpenGLWidget* _glWidget)
{
    glWidget = _glWidget;
}

ModelFactory::~ModelFactory(){}

void ModelFactory::Build(){
    //Shaders files:
    QString vertexDefaultShaderFile(":/shaders/vshader_default.glsl");
    QString vertexEnergyShaderFile(":/shaders/vshader_energy.glsl");
    QString fragmentDefaultShaderFile(":/shaders/fshader_default.glsl");

    //Pre-compiling shaders:
    std::shared_ptr<ShaderCompiler> shaderCompiler = std::make_shared<ShaderCompiler>(glWidget);
    shaderProgramDefault = shaderCompiler->compileShaderProgram(vertexDefaultShaderFile, fragmentDefaultShaderFile);
    shaderProgramEnergy = shaderCompiler->compileShaderProgram(vertexEnergyShaderFile, fragmentDefaultShaderFile);

    //Pre creating buffers:
    std::shared_ptr<BufferCreator> bufferCreator = std::make_shared<BufferCreator>(glWidget);

    QString shipFile = "C:\\Repos\\asteroids\\src\\models\\ship.off";
    shipOffModel = std::make_shared<OffModel>(shipFile);
    vaoShip = bufferCreator->createBuffer(shipOffModel);

    QString gunshotFile = "C:\\Repos\\asteroids\\src\\models\\sphere.off";
    gunshotOffModel = std::make_shared<OffModel>(gunshotFile);
    vaoGunshot = bufferCreator->createBuffer(gunshotOffModel);

    QString asteroidFile = "C:\\Repos\\asteroids\\src\\models\\sphere.off";
    asteroidOffModel = std::make_shared<OffModel>(asteroidFile);
    vaoAsteroid = bufferCreator->createBuffer(asteroidOffModel);
}

std::shared_ptr<Ship> ModelFactory::GetShipInstance(){
    float size =  Physics::shipSize;
    return GetScaledShipInstance(size);
}

std::shared_ptr<Ship> ModelFactory::GetScaledShipInstance(float size){
    QVector3D position = QVector3D(0,0,0);
    std::shared_ptr<Ship> ship = std::make_shared<Ship>(glWidget, vaoShip, shaderProgramDefault, shipOffModel->numFaces, size, shipOffModel->invDiag, shipOffModel->midPoint, position);
    ship->id = QUuid::createUuid().toString();

    return ship;
}


std::shared_ptr<Gunshot> ModelFactory::GetGunshotInstance(Ship* ship){
    if(!ship)
        return nullptr;

    QString vertexShaderFile(":/shaders/vshader_energy.glsl");
    QString fragmentShaderFile(":/shaders/fshader_default.glsl");

    float size =  Physics::gunshotSize;

    QVector3D position = Physics::GetNextLinearMoviment
            (
                ship->currentPosition.x(),
                ship->currentPosition.y(),
                ship->angle,
                Physics::shipAngleCorrection,
                Physics::shipMovimentFactor
             );

    std::shared_ptr<Gunshot> gunshot = std::make_shared<Gunshot>(glWidget, vaoGunshot, shaderProgramEnergy, gunshotOffModel->numFaces, size, gunshotOffModel->invDiag, gunshotOffModel->midPoint, position);

    gunshot->currentPosition = position;

    gunshot->angle = ship->angle;
    gunshot->id = QUuid::createUuid().toString();

    return gunshot;
}


std::shared_ptr<Asteroid> ModelFactory::GetAsteroidInstance(){
    QString vertexShaderFile(":/shaders/vshader_default.glsl");
    QString fragmentShaderFile(":/shaders/fshader_default.glsl");

    float HI = 2.0f;
    float LO = 0.5f;
    float factor = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    float size = Physics::asteroidMSize * factor;

    int choice = qrand() % 2;
    int AbsSignalChoice = qPow(-1, (qrand() % 2));
    int HVChoise = qPow(-1, (qrand() % 2));
    int AngleSignalChoice = qPow(-1, (qrand() % 2));

    float Var = 1.2;
    float absChoice = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/Var));
    float Ang = 45.0;
    float angleChoice = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/Ang));

    QVector2D initPoint;
    if(choice == 0){ // y fixed
        initPoint = QVector2D(absChoice* AbsSignalChoice, Var*HVChoise);
    }else{ // x fixed
        initPoint = QVector2D(Var*HVChoise,absChoice * AbsSignalChoice);
    }

    QVector2D initVector = initPoint.normalized();
    float angle;
    //Angle definition pointing to origin
    if(initVector.y()>=0){
        QVector2D refVector(1,0);
        float dot = QVector2D::dotProduct(refVector,initVector);
        angle = qRadiansToDegrees(qAcos(dot)) + 180;
    }else{
        QVector2D refVector(-1,0);
        float dot = QVector2D::dotProduct(refVector,initVector);
        angle = qRadiansToDegrees(qAcos(dot));
    }
    //Random angle
    angle += angleChoice * AngleSignalChoice;

    auto asteroid = std::make_shared<Asteroid>(glWidget, vaoAsteroid, shaderProgramDefault, asteroidOffModel->numFaces, size, asteroidOffModel->invDiag, asteroidOffModel->midPoint, initPoint);

    asteroid->currentPosition = initPoint;
    asteroid->angle = angle;
    asteroid->color = 0.5f;

    asteroid->id = QUuid::createUuid().toString();

    return asteroid;
}
