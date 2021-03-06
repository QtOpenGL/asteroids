#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    factory = std::make_unique<ModelFactory>(this);

    lifeManager = std::make_unique<LifeManager>(factory.get());

    shipPlayer = getMediaPlayer("..\\..\\sounds\\bangLarge.wav");
    shotPlayer = getMediaPlayer("..\\..\\sounds\\fire.wav");
    asteroidPlayer = getMediaPlayer("..\\..\\sounds\\bangSmall.wav");

    currentScore = 0;
    topPoints = 0;
    level = 0;
    tempTime = 0.0f;

    playing = false;
}

OpenGLWidget::~OpenGLWidget()
{
    delete shipPlayer;
    delete shotPlayer;
    delete asteroidPlayer;
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

    if (!playing)
        return;

    //Nave do jogador
    ship->drawModel();

    //Vidas do jogador
    for (std::shared_ptr<Ship> &life : lifeManager->ships)
        life->drawModel();

    //Tiros da nave
    QHashIterator<QString, std::shared_ptr<Gunshot>> i(gunshots);
    while (i.hasNext())
    {
        i.next();
        i.value()->drawModel();
    }

    //Asteroids
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext())
    {
        i_ast.next();
        i_ast.value()->drawModel();
    }
}

void OpenGLWidget::startGame()
{
    //Resource Pool
    factory->LoadInstances();

    //Audio enable
    shipPlayer->setVolume(Physics::gameVolumeShipPlayer);
    shotPlayer->setVolume(Physics::gameVolumeShotPlayer);
    asteroidPlayer->setVolume(Physics::gameVolumeAsteroidPlayer);

    //Enable state handling
    emit updateButtonEnable(false);
    emit updateGameText(QString(""));

    //Current score handling
    currentScore = 0;
    emit updateCurrentScore(currentScore);

    //Level handling
    level = 1;
    emit updateLevel(QString("Fase: %1").arg(level));

    //5 lifes
    lifeManager->SetLifeCount(Physics::gameInitialLifes);

    //Create Ship
    ship = factory->GetShipInstance();

    //Game starts
    playing = true;

    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        ship->MoveLeft();
        break;
    case Qt::Key_Right:
        ship->MoveRight();
        break;
    case Qt::Key_Up:
        ship->MoveUp();
        break;
    case Qt::Key_Space:
        triggerGunshot();
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
    if (!playing)
        return;

    float elapsedTime = time.restart() / 1000.0f;

    //Gunshots Colisions and Limits
    checkGunshotsColisions(elapsedTime);

    //Asteroids Colisions and Limits
    checkAsteroidsColisions(elapsedTime);

    //New Asteroids
    insertNewAsteroids(elapsedTime);

    update();
}

void OpenGLWidget::checkGunshotsColisions(float elapsedTime)
{
    QHashIterator<QString, std::shared_ptr<Gunshot>> i(gunshots);
    while (i.hasNext())
    {
        i.next();
        if (i.value())
        {
            auto gunshot = i.value();

            //Position
            gunshot->currentPosition = Physics::GetNextLinearMoviment(
                gunshot->currentPosition.x(),
                gunshot->currentPosition.y(),
                gunshot->angle,
                Physics::gunshotAngleCorrection,
                Physics::gunshotMovimentFactor * elapsedTime);

            //Color
            float dist = (gunshot->currentPosition.distanceToPoint(gunshot->initialPosition)) / 2.0f;
            gunshot->color = Physics::modelInitialColor - dist;

            //Limits:
            if (qAbs(gunshot->currentPosition.x()) > Physics::gameOutboundGunshotPosition || qAbs(gunshot->currentPosition.y()) > Physics::gameOutboundGunshotPosition)
            {
                gunshots.remove(gunshot->id);
                gunshot.reset();
            }
            else
            {
                //Score:
                QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
                while (i_ast.hasNext())
                {
                    i_ast.next();
                    auto asteroid = i_ast.value();
                    if (asteroid && !asteroid->isFragment)
                    {
                        if (gunshot->CalculateColision(asteroid.get()))
                        {
                            asteroidPlayer->play();
                            increasePlayerScore();

                            //Create Fragments from father
                            auto fragments = factory->GetFragmentInstance(asteroid->currentPosition, asteroid->scale);
                            for (uint j = 0; j < fragments.size(); ++j)
                            {
                                asteroids[fragments[j]->id] = fragments[j];
                            }

                            //Remove gunshot
                            gunshots.remove(gunshot->id);
                            factory->RemoveGunshotInstance(gunshot);

                            //Remove father
                            asteroids.remove(asteroid->id);
                            factory->RemoveAsteroidInstance(asteroid);

                            break;
                        }
                    }
                }
            }
        }
    }
}

void OpenGLWidget::checkAsteroidsColisions(float elapsedTime)
{
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext())
    {
        i_ast.next();
        auto asteroid = i_ast.value();
        if (asteroid)
        {
            asteroid->currentPosition = Physics::GetNextLinearMoviment(
                asteroid->currentPosition.x(),
                asteroid->currentPosition.y(),
                asteroid->angle,
                Physics::asteroidAngleCorrection,
                asteroid->speed * elapsedTime);

            //Color:
            if (!asteroid->isFragment)
            {
                asteroid->color = Physics::modelInitialColor;
            }
            else
            {
                float dist = (asteroid->currentPosition.distanceToPoint(asteroid->initialPosition)) / 2.0f;
                asteroid->color = Physics::modelInitialColor - dist * Physics::fragmentColorFactor;
            }

            //Limits:
            if (qAbs(asteroid->currentPosition.x()) > Physics::gameOutboundAsteroidPosition || qAbs(asteroid->currentPosition.y()) > Physics::gameOutboundAsteroidPosition)
            {
                asteroids.remove(asteroid->id);
                asteroid.reset();
            }
            else
            {
                //Crash:
                if (!asteroid->isFragment)
                {
                    if (ship->CalculateColision(asteroid.get()))
                    {
                        shipPlayer->play();

                        asteroids.remove(asteroid->id);
                        factory->RemoveAsteroidInstance(asteroid);

                        lifeManager->DecreaseLifeCount();
                        if (lifeManager->IsZero())
                        {
                            setGameOver();
                            update();
                            return;
                        }
                    }
                }
            }
        }
    }
}

void OpenGLWidget::insertNewAsteroids(float elapsedTime)
{
    tempTime += elapsedTime;
    float asteroidTime = Physics::gameInitialAsteroidTimeRelease / level;
    float launchTime = tempTime / asteroidTime;
    if (launchTime > 1.0f)
    {
        tempTime = 0.0f;
        auto asteroid = factory->GetAsteroidInstance();
        asteroids[asteroid->id] = asteroid;
    }
}

void OpenGLWidget::increasePlayerScore()
{
    //Current Score
    currentScore++;
    emit updateCurrentScore(currentScore);

    //Top Score
    if (currentScore > topPoints)
    {
        topPoints = currentScore;
        emit updateTopPoints(topPoints);
    }

    //Level
    level = currentScore / Physics::gameNextLevelFactor + 1;
    emit updateLevel(QString("Fase: %1").arg(level));
}

void OpenGLWidget::setGameOver()
{
    playing = false;

    emit updateGameText(QString("GAME OVER"));
    emit updateButtonEnable(true);
    emit updateLevel("");

    removeAllAsteroids();
    removeAllGunshots();
}

void OpenGLWidget::removeAllAsteroids()
{
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext())
    {
        i_ast.next();
        auto asteroid = i_ast.value();
        if (asteroid)
        {
            asteroids.remove(asteroid->id);
            factory->RemoveAsteroidInstance(asteroid);
        }
    }
}

void OpenGLWidget::removeAllGunshots()
{
    QHashIterator<QString, std::shared_ptr<Gunshot>> i_ast(gunshots);
    while (i_ast.hasNext())
    {
        i_ast.next();
        auto gunshot = i_ast.value();
        if (gunshot)
        {
            gunshots.remove(gunshot->id);
            factory->RemoveGunshotInstance(gunshot);
        }
    }
}

void OpenGLWidget::triggerGunshot()
{
    if (!playing)
        return;

    auto gunshot = factory->GetGunshotInstance(ship.get());
    gunshots[gunshot->id] = gunshot;

    shotPlayer->play();
}

QMediaPlayer *OpenGLWidget::getMediaPlayer(const QString &file)
{
    QMediaPlayer *player = new QMediaPlayer;
    player->setVolume(0);
    player->setMedia(QUrl::fromLocalFile(file));
    player->play();
    return player;
}
