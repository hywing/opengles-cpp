#ifndef TEXTGRAPHICITEM_H
#define TEXTGRAPHICITEM_H

#include "graphic_item_base.h"

#include <unordered_map>

class TextGraphicItem : public GraphicItemBase
{
    struct TextItem {
        string text;
        Point pos;
        Color color;
        float scale;

        TextItem() {

        }

        TextItem(const string &text, const Point &pos, const Color &color, float scale) {
            this->text = text;
            this->pos = pos;
            this->color = color;
            this->scale = scale;
        }

        TextItem(const TextItem &textData) {
            this->text = textData.text;
            this->pos = textData.pos;
            this->color = textData.color;
            this->scale = textData.scale;
        }

        TextItem &operator =(const TextItem &textData) {
            if(this != &textData) {
                this->text = textData.text;
                this->pos = textData.pos;
                this->color = textData.color;
                this->scale = textData.scale;
            }
            return *this;
        }
    };

public:
    TextGraphicItem();

    static TextGraphicItem *getInstance();

    virtual void init() override;

    virtual void draw() override;

    void renderText(const Point &pos, const string &text, const Color &color = glm::vec3(0.5, 0.8f, 0.2f), float scale = 1.0);

    static void setText(const Point &pos, const string &text, const Color &color = glm::vec3(0.5, 0.8f, 0.2f), float scale = 1.0);

    static void drawText();

protected:
    void drawText(int vbo, const std::string &text, float x, float y, float scale, const glm::vec3 &color);

    void drawText(int vbo, const wchar_t* text, int textLen, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 &color);

    void loadFacesByAscii();

    void loadFacesByUnicode(const wchar_t* text, int size);

private:
    std::unordered_map<uint, TextItem> m_hashTable;
};

#endif // TEXTGRAPHICITEM_H
