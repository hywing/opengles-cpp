#ifndef MARKGRAPHICITEM_H
#define MARKGRAPHICITEM_H

#include "graphic_item_base.h"

class MarkGraphicItem : public GraphicItemBase
{
public:
    MarkGraphicItem();

    virtual void init() override;

    virtual void draw() override;

    void setInfo(const Point &pos, const string &type);

private:
    Point m_pos;
    string m_path;
};

#endif // MARKGRAPHICITEM_H
