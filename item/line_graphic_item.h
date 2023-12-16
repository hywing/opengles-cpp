#ifndef LINEGRAPHICITEM_H
#define LINEGRAPHICITEM_H

#include "graphic_item_base.h"

class LineGraphicItem : public GraphicItemBase
{
public:
    LineGraphicItem();

    virtual void init() override;

    void setPoints(const Point &a, const Point &b);

private:
    Point m_a;
    Point m_b;
};

#endif // LINEGRAPHICITEM_H
