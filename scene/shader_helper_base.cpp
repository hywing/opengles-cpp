#include "shader_helper_base.h"

ShaderHelperBase::ShaderHelperBase()
{
    m_program = new OpenglShaderProgram();
}

ShaderHelperBase::~ShaderHelperBase()
{
    delete m_program;
}

int ShaderHelperBase::attributeLocation(const string &name) const
{
    return m_program->attributeLocation(name);
}

void ShaderHelperBase::setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride)
{
    m_program->setAttributeBuffer(location, type, offset, tupleSize, stride);
}

void ShaderHelperBase::enableAttributeArray(int location)
{
    m_program->enableAttributeArray(location);
}

void ShaderHelperBase::disableAttributeArray(int location)
{
    m_program->disableAttributeArray(location);
}

void ShaderHelperBase::setUniformValue(int location, const glm::mat4x4 &value)
{
    m_program->setUniformValue(location, value);
}

void ShaderHelperBase::setUniformValue(int location, GLuint value)
{
    m_program->setUniformValue(location, value);
}

void ShaderHelperBase::setUniformValue(int location, const glm::vec3 &value)
{
    m_program->setUniformValue(location, value);
}

int ShaderHelperBase::uniformLocation(const string &name) const
{
    return m_program->uniformLocation(name);
}

bool ShaderHelperBase::bind()
{
    return m_program->bind();
}

void ShaderHelperBase::release()
{
    m_program->release();
}
