#ifndef GRAPHICITEMBASE_H
#define GRAPHICITEMBASE_H

#include <cmath>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GLES2/gl2.h>

#include "matrix_helper.h"
#include "normal_shader_helper.h"
#include "texture_shader_helper.h"
#include "text_shader_helper.h"
#include "sky_shader_helper.h"
#include "object_shader_helper.h"
#include "model_shader_helper.h"
#include "stb_image.h"
#include "file_system.h"

using string = std::string;

using uint = unsigned int;

using ushort = unsigned short;

using uchar = unsigned char;

struct Point
{
    glm::vec4 point;

    Point()
    {
        point.x = 0.0f;
        point.y = 0.0f;
        point.z = 0.0f;
        point.w = 0.0f;
    }

    Point(const Point &cp)
    {
        point = cp.point;
    }

    Point(float x, float y, float z = 0.0f, float angle = 0.0f)
    {
        point.x = x;
        point.y = y;
        point.z = z;
        point.w = angle;
    }

    float angle() const
    {
        return point.w;
    }

    Point &operator =(const Point &cp)
    {
        if (this != &cp)
        {
            point = cp.point;
        }
        return *this;
    }

    bool operator !=(const Point &cp)
    {
        return !(point == cp.point);
    }

    friend Point operator -(const Point &a, const Point &b)
    {
        Point c;
        c.point = a.point - b.point;
        return c;
    }

    friend Point operator +(const Point &a, const Point &b)
    {
        Point c;
        c.point = a.point + b.point;
        return c;
    }

    friend Point operator +=(const Point &a, const Point &b)
    {
        Point c;
        c.point = a.point + b.point;
        return c;
    }

    friend Point operator /(const Point &a, double div)
    {
        Point c;
        c.point.x = a.point.x * div;
        c.point.y = a.point.y * div;
        c.point.z = a.point.z * div;
        c.point.w = a.point.w;
        return c;
    }

    friend Point operator *(const Point &a, double div)
    {
        Point c;
        c.point.x = a.point.x * div;
        c.point.y = a.point.y * div;
        c.point.z = a.point.z * div;
        c.point.w = a.point.w;
        return c;
    }

    friend std::ostream &operator <<(std::ostream & os,const Point &c)
    {
        return os << "Point(" << c.point.x << "，" << c.point.y << ", " << c.point.z << "), angle : " << c.angle();
    }
};

struct Color
{
    Color() {

    }

    Color(int color) {
        m_color = color;
    }

    Color(const glm::vec3 &vec) {
        uchar red = (uchar)(vec.r * 255);
        uchar green = (uchar)(vec.g * 255);
        uchar blue = (uchar)(vec.b * 255);
        uchar alpha = 1;
        m_color = (alpha << 24) | (red << 16) | (green << 8) | blue;
    }

    Color(uchar red, uchar green, uchar blue, uchar alpha = 1) {
        m_color = (alpha << 24) | (red << 16) | (green << 8) | blue;
    }

    Color(uint color) {
        this->m_color = color;
    }

    glm::vec3 getColor() {
        glm::vec3 vec;
        vec.r = ((m_color & 0xff0000) >> 16) / 255.0;
        vec.g = ((m_color & 0xff00) >> 8) / 255.0;
        vec.b = (m_color & 0xff) / 255.0;
        return vec;
    }

private:
    uint m_color = (1 << 31);
};

class GraphicItemBase
{
public:
    explicit GraphicItemBase();

    virtual ~GraphicItemBase();

    virtual void init() = 0;

    virtual void draw();

    void setMode(GLenum mode);

    void setName(const string &name);

    string name() const;

    void translate(float x, float y, float z);

    void rotate(float angle, float x, float y, float z);

    void scale(float factor);

    string type() const;

    void setColor(const Color &color);

protected:
    // 图元vao
    GLint m_vao;
    // 图元vbo
    GLuint m_vbo;
    // 图元ebo
    GLint m_ebo;
    // 图元buffer
    std::vector<glm::vec3> m_buffer;
    // 图元纹理
    GLuint m_texture;
    // 顶点数目
    int m_number;
    // 绘制模式
    GLenum m_mode;
    // 图元名称
    string m_name;
    // 圆周率
    const double PI = 3.1415926535898;
    // 类型
    string m_type;
    // 颜色
    Color m_color;
};

#endif // GRAPHICITEMBASE_H
