#ifndef SKYGRAPHICITEM_H
#define SKYGRAPHICITEM_H

#include "graphic_item_base.h"

class SkyGraphicItem : public GraphicItemBase
{
public:
    SkyGraphicItem();

    virtual void init() override;

    virtual void draw() override;

protected:
    void createCubeTexture();

private:
    float m_xBegin;
    float m_xEnd;
    float m_yBegin;
    float m_yEnd;
};

#endif // SKYGRAPHICITEM_H
