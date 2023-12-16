#ifndef TRACEGRAPHICITEM_H
#define TRACEGRAPHICITEM_H

#include "graphic_item_base.h"

class TraceGraphicItem : public GraphicItemBase
{
public:
    TraceGraphicItem();

    virtual void init() override;

    virtual void draw() override;

    void addPoints(const std::vector<Point> &p);
};

#endif // TRACEGRAPHICITEM_H
