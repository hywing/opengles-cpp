#include "graphic_item_base.h"

GraphicItemBase::GraphicItemBase()
    : m_vbo(-1), m_ebo(-1), m_texture(-1)
{

}

GraphicItemBase::~GraphicItemBase()
{

}

void GraphicItemBase::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    auto shader = NormalShaderHelper::getInstance();
    auto matrix = MatrixHelper::getInstance();
    shader->setUniformValue(shader->uniformLocation("matrix"), matrix->getFinalProjectMatrix());
    shader->setAttributeBuffer(shader->attributeLocation("posAttr"), GL_FLOAT, 0, 3, sizeof(float) * 6);
    shader->enableAttributeArray(shader->attributeLocation("posAttr"));
    shader->setAttributeBuffer(shader->attributeLocation("colAttr"), GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 6);
    shader->enableAttributeArray(shader->attributeLocation("colAttr"));
    glLineWidth(1.0);
    glDrawArrays(m_mode, 0, m_number);
    shader->disableAttributeArray(shader->attributeLocation("posAttr"));
    shader->disableAttributeArray(shader->attributeLocation("colAttr"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GraphicItemBase::setMode(GLenum mode)
{
    m_mode = mode;
}

void GraphicItemBase::setName(const string &name)
{
    m_name = name;
}

string GraphicItemBase::name() const
{
    return m_name;
}

void GraphicItemBase::translate(float x, float y, float z)
{
    MatrixHelper::getInstance()->translate(x, y, z);
}

void GraphicItemBase::rotate(float angle, float x, float y, float z)
{
    MatrixHelper::getInstance()->rotate(angle, x, y, z);
}

void GraphicItemBase::scale(float factor)
{
    MatrixHelper::getInstance()->scale(factor);
}

string GraphicItemBase::type() const
{
    return m_type;
}

void GraphicItemBase::setColor(const Color &color)
{
    m_color = color;
}
