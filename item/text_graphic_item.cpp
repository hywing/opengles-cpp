#include "text_graphic_item.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <map>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <cstdint>

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

static std::map<GLint, Character> g_characters;

#ifndef GL_RED
#define GL_RED 0x1903
#endif

static const int MAX_SHORT_VALUE = 65536;

TextGraphicItem::TextGraphicItem()
{
    init();
}

TextGraphicItem *TextGraphicItem::getInstance()
{
    static TextGraphicItem instance;
    return &instance;
}

void TextGraphicItem::init()
{
    loadFacesByAscii();
}

void TextGraphicItem::draw()
{
    for(auto &iter : m_hashTable)
    {
        string str = iter.second.text;
        auto newStr = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(str);
        drawText(iter.first, newStr.data(), newStr.size(), iter.second.pos.point.x, iter.second.pos.point.y, iter.second.scale, iter.second.color.getColor());
    }
}

void TextGraphicItem::renderText(const Point &pos, const string &text, const Color &color, float scale)
{
    bool result = false;
//    for(auto iter : m_hashTable) {
//        if(iter.second.text == text) {
//            result = true;
//        }
//    }

    if(!result) {
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        TextItem textData(text, pos, color, scale);
        m_hashTable[m_vbo] = textData;
        auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(text);
        loadFacesByUnicode(str.data(), str.size());
    }
}

void TextGraphicItem::setText(const Point &pos, const string &text, const Color &color, float scale)
{
    TextShaderHelper::getInstance()->bind();
    TextGraphicItem::getInstance()->renderText(pos, text, color, scale);
    TextShaderHelper::getInstance()->release();
}

void TextGraphicItem::drawText()
{
    TextShaderHelper::getInstance()->bind();
    TextGraphicItem::getInstance()->draw();
    TextShaderHelper::getInstance()->release();
}

void TextGraphicItem::drawText(int vbo, const std::string &text, float x, float y, float scale, const glm::vec3 &color)
{
    auto shader = TextShaderHelper::getInstance();
    auto matrix = MatrixHelper::getInstance();
    shader->setUniformValue(shader->uniformLocation("u_Matrix"), matrix->getFinalProjectMatrix());
    shader->enableAttributeArray(shader->attributeLocation("a_Position"));

    glUniform3f(shader->uniformLocation("textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = g_characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glVertexAttribPointer(shader->attributeLocation("a_Position"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    shader->disableAttributeArray(shader->attributeLocation("a_Position"));
}

void TextGraphicItem::drawText(int vbo, const wchar_t *text, int textLen, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 &color)
{
    auto shader = TextShaderHelper::getInstance();
    auto matrix = MatrixHelper::getInstance();
    shader->setUniformValue(shader->uniformLocation("u_Matrix"), matrix->getFinalProjectMatrix());
    shader->enableAttributeArray(shader->attributeLocation("a_Position"));

    glUniform3f(shader->uniformLocation("textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    for (int i = 0; i < textLen; ++i)
    {
        Character ch = g_characters[text[i]];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glVertexAttribPointer(shader->attributeLocation("a_Position"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制方块
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * scale; //(2^6 = 64)
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    shader->disableAttributeArray(shader->attributeLocation("a_Position"));
}

void TextGraphicItem::loadFacesByAscii()
{
    // 初始化freetype字体库
    {
        // FreeType
        // --------
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        // find path to font
        std::string font_name = FileSystem::SOURCE_PATH + "res/font/msyh.ttc";
        if (font_name.empty())
        {
            std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                g_characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
}

void TextGraphicItem::loadFacesByUnicode(const wchar_t *text, int size)
{
    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "TextRenderSample::LoadFacesByUnicode FREETYPE: Could not init FreeType Library"  << std::endl;

    // Load font as face
    FT_Face face;
    std::string path(FileSystem::SOURCE_PATH + "res/font/msyh.ttc");
    if (FT_New_Face(ft, path.c_str(), 0, &face))
        std::cout << "TextRenderSample::LoadFacesByUnicode FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 96, 96);
    FT_Select_Charmap(face, ft_encoding_unicode);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (int i = 0; i < size; ++i) {
        // 节约vbo资源，如果找到了，就没必要再创建一副纹理了
        if(g_characters.find(text[i]) != g_characters.end()) {
            continue ;
        }

        //int index =  FT_Get_Char_Index(face,unicodeArr[i]);
        if (FT_Load_Glyph(face, FT_Get_Char_Index(face, text[i]), FT_LOAD_DEFAULT))
        {
            std::cout << "TextRenderSample::LoadFacesByUnicode FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        FT_Glyph glyph;
        FT_Get_Glyph(face->glyph, &glyph );

        //Convert the glyph to a bitmap.
        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

        //This reference will make accessing the bitmap easier
        FT_Bitmap& bitmap = bitmap_glyph->bitmap;

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_LUMINANCE,
                bitmap.width,
                bitmap.rows,
                0,
                GL_LUMINANCE,
                GL_UNSIGNED_BYTE,
                bitmap.buffer
        );

//        printf("TextRenderSample::LoadFacesByUnicode text[i]=%d [w,h,buffer]=[%d, %d, %p], advance.x=%ld\n", text[i], bitmap.width, bitmap.rows, bitmap.buffer, glyph->advance.x / MAX_SHORT_VALUE);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
                texture,
                glm::ivec2(/*face->glyph->*/bitmap.width, /*face->glyph->*/bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>((glyph->advance.x / MAX_SHORT_VALUE) << 6)
        };
        g_characters.insert(std::pair<GLint, Character>(text[i], character));

    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

