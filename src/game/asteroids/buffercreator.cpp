#include "buffercreator.h"

BufferCreator::BufferCreator(QOpenGLWidget* _glWidget){
    glWidget = _glWidget;

    glWidget->makeCurrent();

    initializeOpenGLFunctions();
}

BufferCreator::~BufferCreator(){
    destroyVBOs();
}

void BufferCreator::createVBOs(std::shared_ptr<OffModel> offModel){
    glWidget->makeCurrent();
    destroyVBOs();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, offModel->numVertices * sizeof(QVector4D), offModel->vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, offModel->numFaces * 3 * sizeof(unsigned int), offModel->indices.get(), GL_STATIC_DRAW);
}

void BufferCreator::destroyVBOs(){
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboIndices);
    glDeleteVertexArrays(1, &vao);
    vboVertices = 0;
    vboIndices = 0;
    vao = 0;
}

GLuint BufferCreator::createBuffer(std::shared_ptr<OffModel> offModel){
    createVBOs(offModel);
    return vao;
}
