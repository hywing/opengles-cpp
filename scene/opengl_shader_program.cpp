#include "opengl_shader_program.h"

OpenglShaderProgram::OpenglShaderProgram()
{

}

bool OpenglShaderProgram::addShaderFromSourceCode(OpenglShaderProgram::ShaderType type, const char *source)
{
    enum Consts {INFOLOG_LEN = 512};
    GLchar infoLog[INFOLOG_LEN];
    GLint success;
    if(type == Vertex) {
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &source, NULL);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, INFOLOG_LEN, NULL, infoLog);
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        }
    }
    else if(type == Fragment) {
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &source, NULL);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
        }
    }
    return true;
}

int OpenglShaderProgram::attributeLocation(const string &name) const
{
    return glGetAttribLocation(m_program, name.data());
}

void OpenglShaderProgram::setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride)
{
    glVertexAttribPointer(location, tupleSize, type, GL_TRUE, stride, reinterpret_cast<const void *>(offset));
}

void OpenglShaderProgram::enableAttributeArray(int location)
{
    glEnableVertexAttribArray(location);
}

void OpenglShaderProgram::disableAttributeArray(int location)
{
    glDisableVertexAttribArray(location);
}

void OpenglShaderProgram::setUniformValue(int location, const glm::mat4x4 &value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void OpenglShaderProgram::setUniformValue(int location, GLuint value)
{
    glUniform1i(location, value);
}

void OpenglShaderProgram::setUniformValue(int location, const glm::vec3 &value)
{
    glUniform3fv(location, 1, &value[0]);
}

int OpenglShaderProgram::uniformLocation(const string &name) const
{
    return glGetUniformLocation(m_program, name.data());
}

bool OpenglShaderProgram::bind()
{
    glUseProgram(m_program);
    return true;
}

void OpenglShaderProgram::release()
{
    glUseProgram(0);
}

bool OpenglShaderProgram::link()
{
    GLint value;

    m_program = glCreateProgram();
    glAttachShader(m_program, vertex_shader);
    glAttachShader(m_program, fragment_shader);

    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &value);
    return static_cast<bool>(value);
}

GLuint OpenglShaderProgram::program()
{
    return m_program;
}
