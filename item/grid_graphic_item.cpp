#include "grid_graphic_item.h"

GridGraphicItem::GridGraphicItem()
{

}

void GridGraphicItem::init()
{
    float landLineColorX = 0.72f;
    float landLineColorY = 0.77f;
    float landLineColorZ = 0.84f;

    float buffer[3864];

    m_xBegin = -m_width + m_pos;
    m_xEnd   = m_width + m_pos;
    m_yBegin = -m_width + m_pos;
    m_yEnd   = m_width + m_pos;

    int j = 0;

    for (float i = m_xBegin; i <= m_xEnd; i += 50.0)
    {
        // 顶点
        buffer[j++] = i;
        buffer[j++] = (m_yEnd);
        buffer[j++] = (0.0f);

        // 颜色
        buffer[j++] = (landLineColorX);
        buffer[j++] = (landLineColorY);
        buffer[j++] = (landLineColorZ);

        // 顶点
        buffer[j++] = (i);
        buffer[j++] = (m_yBegin);
        buffer[j++] = (0.0f);

        // 颜色
        buffer[j++] = (landLineColorX);
        buffer[j++] = (landLineColorY);
        buffer[j++] = (landLineColorZ);
    }

    for (float i = m_yBegin; i <= m_yEnd; i += 50.0)
    {
        // 顶点
        buffer[j++] = (m_xBegin);
        buffer[j++] = (i);
        buffer[j++] = (0.0f);

        // 颜色
        buffer[j++] = (landLineColorX);
        buffer[j++] = (landLineColorY);
        buffer[j++] = (landLineColorZ);

        // 顶点
        buffer[j++] = (m_xEnd);
        buffer[j++] = (i);
        buffer[j++] = (0.0f);

        // 颜色
        buffer[j++] = (landLineColorX);
        buffer[j++] = (landLineColorY);
        buffer[j++] = (landLineColorZ);
    }

    m_mode = GL_LINES;
    m_number = 3864 / 6;

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
