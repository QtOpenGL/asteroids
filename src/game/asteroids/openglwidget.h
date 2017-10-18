#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "memory.h"
#include "model.h"
#include "ship.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = 0);
    std::shared_ptr<Model> model = nullptr;
    std::shared_ptr<Ship> ship = nullptr;

    float angle;
    float X;
    float Y;
    float Z;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent* event);

signals:
    void statusBarMessage(QString);

public slots:
    void showFileOpenDialog();
    void loadSampleModel();
    void rotateObject(int value);
    void AxisXChecked(bool checked);
    void AxisYChecked(bool checked);
    void AxisZChecked(bool checked);
};
#endif // OPENGLWIDGET_H