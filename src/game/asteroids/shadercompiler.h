#ifndef SHADERCOMPILER_H
#define SHADERCOMPILER_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QString>

class ShaderCompiler: public QOpenGLExtraFunctions {
public:
    ShaderCompiler(QOpenGLWidget* _glWidget);
    ~ShaderCompiler();

    GLuint compileShaderProgram(QString vertexShaderFile, QString fragmentShaderFile);

protected:
    QOpenGLWidget* glWidget;

    GLuint shaderProgram = 0;

    void createShaders(QString vertexShaderFile, QString fragmentShaderFile);
    void destroyShaders();
};

#endif // SHADERCOMPILER_H
