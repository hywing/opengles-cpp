#include "line_graphic_item.h"

LineGraphicItem::LineGraphicItem()
{

}

void LineGraphicItem::init()
{
    auto color = m_color.getColor();
    GLfloat vertices[] = {
         m_a.point.x,  m_a.point.y, m_a.point.z,
         color.r, color.g, color.b,
         m_b.point.x,  m_b.point.y, m_b.point.z,
         color.r, color.g, color.b,
    };

    m_mode = GL_LINE_STRIP;
    m_number = 2;

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineGraphicItem::setPoints(const Point &a, const Point &b)
{
    m_a = a;
    m_b = b;

    init();
}
