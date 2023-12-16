#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H

#include <glm/glm.hpp>
#include <GLES2/gl2.h>
#include <string>
#include <iostream>

using string = std::string;

class OpenglShaderProgram
{
public:
    enum ShaderType : int {
        Vertex,
        Fragment
    };

    OpenglShaderProgram();

    bool addShaderFromSourceCode(ShaderType type, const char *source);

    bool link();

    int attributeLocation(const string &name) const;

    void setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0);

    void enableAttributeArray(int location);

    void disableAttributeArray(int location);

    void setUniformValue(int location, const glm::mat4x4 &value);

    void setUniformValue(int location, GLuint value);

    void setUniformValue(int location, const glm::vec3 &value);

    int uniformLocation(const string &name) const;

    bool bind();

    void release();

    GLuint program();

private:
    GLuint m_program;
    GLint vertex_shader;
    GLint fragment_shader;
};

#endif // OPENGLSHADERPROGRAM_H
