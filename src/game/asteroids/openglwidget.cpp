#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    angle=0.0;
    X=0.0;
    Y=1.0;
    Z=0.0;
}
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug("OpenGL version : %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);
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
    ship->drawModel(angle, X, Y, Z);

    //asteroid->drawModel(angle, x, y, z);
    //guardar info de radius e center(x,y,z)
}

void OpenGLWidget::showFileOpenDialog()
{
    QByteArray fileFormat = "off ";
    QString fileName = QFileDialog::getOpenFileName(this,
        " Open File ", QDir::homePath(),
        QString("\%1 Files (*.\%2) ").arg(QString(fileFormat.toUpper())).arg(QString(fileFormat)), 0
#ifdef Q_OS_LINUX
        ,
        QFileDialog::DontUseNativeDialog
#endif
        );
    if (!fileName.isEmpty()) {
        model = std::make_shared<Model>(this);
        model->readOFFFile(fileName);
        emit statusBarMessage(QString("Vertices: \%1 , Faces : \%2").arg(model->numVertices).arg(model->numFaces));
    }
    update();
}

void OpenGLWidget::loadSampleModel()
{
    QString fileName = "C:\\Repos\\asteroids\\src\\models\\ship.off";

    ship = std::make_shared<Ship>(this);
    ship->readOFFFile(fileName);

    update();
}

void OpenGLWidget::rotateObject(int value)
{
    qDebug("Angle: %d", value);
    angle = value;

    update();
}

void OpenGLWidget::AxisXChecked(bool checked){
    if(checked)
        X=1.0;
    else
        X=0.0;
}

void OpenGLWidget::AxisYChecked(bool checked){
    if(checked)
        Y=1.0;
    else
        Y=0.0;
}

void OpenGLWidget::AxisZChecked(bool checked){
    if(checked)
        Z=1.0;
    else
        Z=0.0;
}
