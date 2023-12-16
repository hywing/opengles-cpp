#include "axis.h"
#include "line_graphic_item.h"
#include "text_graphic_item.h"

Axis::Axis()
{

}

Axis::~Axis()
{

}

void Axis::setInfo(const std::string &name, float begin, float end)
{
    m_name = name;
    m_begin = begin;
    m_end = end;

    initAxis();
}

void Axis::draw()
{
    NormalShaderHelper::getInstance()->bind();
    m_line->draw();
    for(auto &iter : m_markTicks) {
        iter.line->draw();
    }
    NormalShaderHelper::getInstance()->release();
}

void Axis::initAxis()
{
    m_line = std::shared_ptr<LineGraphicItem>(new LineGraphicItem);

    int width = 5;

    if(m_name == "x") {
        // 轴的范围
        m_line->setPoints(Point(m_begin, 0), Point(m_end, 0));

        // 每100个刻度画一根线段
        for(auto i = m_begin; i <= m_end; i += 100) {
            TickItem item;
            Point p(i, 0);
            item.line = std::shared_ptr<LineGraphicItem>(new LineGraphicItem);
            item.line->setPoints(p, Point(i, width));
            item.text = std::to_string(int(i));
            m_markTicks.push_back(item);
            TextGraphicItem::setText(Point(i - 10, width - 15), item.text, glm::vec3(1, 0, 0), 0.2);
        }

        // 末尾画上x轴的标记
        TextGraphicItem::setText(Point(m_end + 50, 10), m_name);
    }
    else if(m_name == "y") {
        // 轴的范围
        m_line->setPoints(Point(0, m_begin), Point(0, m_end));

        // 每100个刻度画一根线段
        for(auto i = m_begin; i <= m_end; i += 100) {
            if(i != 0) {
                TickItem item;
                Point p(0, i);
                item.line = std::shared_ptr<LineGraphicItem>(new LineGraphicItem);
                item.line->setPoints(p, Point(width, i));
                item.text = std::to_string(int(i));
                m_markTicks.push_back(item);
                TextGraphicItem::setText(Point(-width - 20, i - 10), item.text, glm::vec3(0, 1, 0), 0.2);
            }
        }

        // 末尾画上y轴的标记
        TextGraphicItem::setText(Point(-width - 20, m_end + 50), m_name);
    }
}
