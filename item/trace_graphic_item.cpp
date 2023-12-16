#include "trace_graphic_item.h"

TraceGraphicItem::TraceGraphicItem()
{

}

void TraceGraphicItem::init()
{
    m_mode = GL_LINE_STRIP;
    m_number = m_buffer.size() / 2;

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_buffer.size() * sizeof (glm::vec3), &m_buffer[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TraceGraphicItem::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    auto shader = NormalShaderHelper::getInstance();
    auto matrix = MatrixHelper::getInstance();
    shader->setUniformValue(shader->uniformLocation("matrix"), matrix->getFinalProjectMatrix());
    glVertexAttribPointer(shader->attributeLocation("posAttr"), 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
    glVertexAttribPointer(shader->attributeLocation("colAttr"), 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
    shader->enableAttributeArray(shader->attributeLocation("posAttr"));
    shader->enableAttributeArray(shader->attributeLocation("colAttr"));
    glLineWidth(1.0);
    glDrawArrays(m_mode, 0, m_number);
    shader->disableAttributeArray(shader->attributeLocation("posAttr"));
    shader->disableAttributeArray(shader->attributeLocation("colAttr"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TraceGraphicItem::addPoints(const std::vector<Point> &points)
{
    for(auto &p : points) {
        m_buffer.push_back(glm::vec3(p.point.x, p.point.y, p.point.z));
        m_buffer.push_back(m_color.getColor());
    }

    init();
}
