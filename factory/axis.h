#ifndef AXIS_H
#define AXIS_H

#include <string>
#include <vector>
#include <memory>

class LineGraphicItem;

class Axis
{
    struct TickItem {
        std::shared_ptr<LineGraphicItem> line;
        std::string text;
    };

public:
    Axis();

    ~Axis();

    void setInfo(const std::string &name, float begin = -2000, float end = 2000);

    void draw();

protected:
    void initAxis();

private:
    // 刻度：每100个单位标注
    std::vector<TickItem> m_markTicks;

    // 直线：-4000 - 4000
    std::shared_ptr<LineGraphicItem> m_line;

    // 轴标记：x or y
    std::string m_name = "x";

    // 起点
    float m_begin = -2000;

    // 终点
    float m_end = 2000;
};

#endif // AXIS_H
