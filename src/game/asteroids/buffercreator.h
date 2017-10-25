#ifndef BUFFERCREATOR_H
#define BUFFERCREATOR_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "offmodel.h"

class BufferCreator: public QOpenGLExtraFunctions {
public:
    BufferCreator(QOpenGLWidget* _glWidget);
    ~BufferCreator();

    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;

    GLuint createBuffer(std::shared_ptr<OffModel> offModel);

protected:
    QOpenGLWidget* glWidget;

    void createVBOs(std::shared_ptr<OffModel> offModel);
    void destroyVBOs();
};

#endif // BUFFERCREATOR_H
