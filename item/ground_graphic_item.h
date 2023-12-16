#ifndef GROUNDGRAHICITEM_H
#define GROUNDGRAHICITEM_H

#include "graphic_item_base.h"

class GroundGrahicItem : public GraphicItemBase
{
public:
    GroundGrahicItem();

    virtual void init();

private:
    double m_width = 4000.0f;
    float  m_pos   = 0.0f;
    float  m_xBegin;
    float  m_xEnd;
    float  m_yBegin;
    float  m_yEnd;
};

#endif // GROUNDGRAHICITEM_H
