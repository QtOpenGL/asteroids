#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    angle=0.0;

    factory = std::make_shared<ModelFactory>(this);

    player = new QMediaPlayer;

    player->setMedia(QUrl::fromLocalFile("C:\\Repos\\asteroids\\src\\sounds\\fire.wav"));
    player->setVolume(100);
}
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug("OpenGL version : %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);

    time.start();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!ship)
        return;

    //Nave do jogador
    ship->drawModel(angle, 0.1);

    if (!gunshot)
        return;

    //Tiros do jogador
    gunshot->drawModel(angle, 0.02);

    //Tiros do jogador
    //gunshot->drawModel(angle, X, Y, Z);

    //asteroid->drawModel(angle, x, y, z);
    //guardar info de radius e center(x,y,z)
}

void OpenGLWidget::loadSampleModel()
{
    //ship = std::make_shared<Ship>(this, shipOffModel);
    //ship->Create();

    ship = factory->GetShipInstance();

    //gunshot = std::make_shared<Gunshot>(this, shipOffModel);
    //gunshot->Create();

    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        angle+=5.0;
        qDebug("Angle: %f", angle);
        break;
    case Qt::Key_Right:
        angle-=5.0;
        qDebug("Angle: %f", angle);
        break;
    case Qt::Key_Up:
        float xPos, yPos;
        xPos= ship->atualPoint.x() + 0.05*cos((angle + 90)* (3.1416/180));
        yPos= ship->atualPoint.y() + 0.05*sin((angle + 90)* (3.1416/180));
        ship->atualPoint = QVector3D(xPos, yPos, 0);
        break;
    case Qt::Key_Space:
        float xShipPos, yShipPos;
        xShipPos= ship->atualPoint.x() + 0.05*cos((angle + 90)* (3.1416/180));
        yShipPos= ship->atualPoint.y() + 0.05*sin((angle + 90)* (3.1416/180));

        gunshot = factory->GetGunshotInstance();
        gunshot->atualPoint = QVector3D(xShipPos, yShipPos, 0);

        player->play();
        break;
    case Qt::Key_Escape:
        qApp->quit();
        break;
    default:
        break;
    }
    update();
}

void OpenGLWidget::animate()
{
    float elapsedTime = time.restart() / 1000.0f;

    if (!gunshot)
        return;

    float xPos, yPos;
    xPos= gunshot->atualPoint.x() + elapsedTime * 2 * cos((angle + 90)* (3.1416/180));
    yPos= gunshot->atualPoint.y() + elapsedTime * 2 * sin((angle + 90)* (3.1416/180));
    gunshot->atualPoint =  QVector3D(xPos, yPos, 0);

    //Limits:
    if(qAbs(gunshot->atualPoint.x())>1.2 || qAbs(gunshot->atualPoint.y()) >1.2){
        gunshot.reset();
    }

    ////10 shots:
    //for (int i = 0; i < 10; ++i) {
    //    if(ponteiro[i]){ //if exists
    //        //animate
    //    }
    //}

    //during delete, insert

    update();
}
