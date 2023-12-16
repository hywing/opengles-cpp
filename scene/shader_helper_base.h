#ifndef SHADERHELPERBASE_H
#define SHADERHELPERBASE_H

#include "opengl_shader_program.h"

class ShaderHelperBase
{
public:
    explicit ShaderHelperBase();

    ~ShaderHelperBase();

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

protected:
    OpenglShaderProgram *m_program;
};

#ifndef SINGLETON_DECLARE
#define SINGLETON_DECLARE(CLASS)                   \
    class CLASS : public ShaderHelperBase          \
    {                                              \
    private:                                       \
        explicit CLASS(); \
        ~CLASS()                                   \
        {                                          \
        }                                          \
                                                   \
    public:                                        \
        static CLASS *getInstance()                \
        {                                          \
            static CLASS instance;                 \
            return &instance;                      \
        };                                         \
    };
#endif

#ifndef SINGLETON_IMPLEMENT
#define SINGLETON_IMPLEMENT(CLASS)                                                          \
    CLASS::CLASS()                                                                          \
    {                                                                                       \
        m_program->addShaderFromSourceCode(OpenglShaderProgram::Vertex, vertex_shader);     \
        m_program->addShaderFromSourceCode(OpenglShaderProgram::Fragment, fragment_shader); \
        if (!m_program->link())                                                             \
        {                                                                                   \
            std::cout << "shader link error!";                                              \
        }                                                                                   \
    }
#endif

#endif // SHADERHELPERBASE_H
