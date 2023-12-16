#include "ground_graphic_item.h"

GroundGrahicItem::GroundGrahicItem()
{

}

void GroundGrahicItem::init()
{
    float landColorX = 220.0 / 256;
    float landColorY = 229.0 / 256;
    float landColorZ = 244.0 / 256;

    m_xBegin = -m_width + m_pos;
    m_xEnd   = m_width + m_pos;
    m_yBegin = -m_width + m_pos;
    m_yEnd   = m_width + m_pos;

    float arrVertex[] = {
        //  position                color
        0.0f + m_pos, 0.0f + m_pos, 0.0f,       landColorX, landColorY, landColorZ, m_xBegin,   m_yBegin,   0.0f,
        landColorX,   landColorY,   landColorZ, m_xEnd,     m_yBegin,   0.0f,       landColorX, landColorY, landColorZ,
        m_xEnd,       m_yEnd,       0.0f,       landColorX, landColorY, landColorZ, m_xBegin,   m_yEnd,     0.0f,
        landColorX,   landColorY,   landColorZ, m_xBegin,   m_yBegin,   0.0f,       landColorX, landColorY, landColorZ,
    };

    m_mode = GL_TRIANGLE_FAN;

    m_number = 6;

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrVertex), &arrVertex[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
