#ifndef GRIDGRAPHICITEM_H
#define GRIDGRAPHICITEM_H

#include "graphic_item_base.h"

class GridGraphicItem : public GraphicItemBase
{
public:
    GridGraphicItem();

    virtual void init();

private:
    double m_width = 4000.0f;
    double m_pos   = 0.0f;
    float  m_xBegin;
    float  m_xEnd;
    float  m_yBegin;
    float  m_yEnd;
};

#endif // GRIDGRAPHICITEM_H
