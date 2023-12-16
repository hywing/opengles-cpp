#include "mark_graphic_item.h"

MarkGraphicItem::MarkGraphicItem()
{

}

void MarkGraphicItem::init()
{
    float buffer[20];
    int i = 0;

    int pixSize = 25;
    {
        // top right
        buffer[i++] = (m_pos.point.x + pixSize);
        buffer[i++] = (m_pos.point.y + pixSize);
        buffer[i++] = (m_pos.point.z);

        buffer[i++] = (1.0f);
        buffer[i++] = (1.0f);

        // bottom right
        buffer[i++] = (m_pos.point.x + pixSize);
        buffer[i++] = (m_pos.point.y - pixSize);
        buffer[i++] = (m_pos.point.z);

        buffer[i++] = (1.0f);
        buffer[i++] = (0.0f);

        // bottom left
        buffer[i++] = (m_pos.point.x - pixSize);
        buffer[i++] = (m_pos.point.y - pixSize);
        buffer[i++] = (m_pos.point.z);

        buffer[i++] = (0.0f);
        buffer[i++] = (0.0f);

        // top left
        buffer[i++] = (m_pos.point.x - pixSize);
        buffer[i++] = (m_pos.point.y + pixSize);
        buffer[i++] = (m_pos.point.z);

        buffer[i++] = (0.0f);
        buffer[i++] = (1.0f);
    }

    m_mode   = GL_TRIANGLE_FAN;
    m_number = 4;

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MarkGraphicItem::draw()
{
    auto shader = TextureShaderHelper::getInstance();
    auto matrix = MatrixHelper::getInstance();
    shader->setUniformValue(shader->uniformLocation("u_Matrix"), matrix->getFinalProjectMatrix());
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    shader->setUniformValue(shader->uniformLocation("u_TextureUnit"), 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    shader->setAttributeBuffer(shader->attributeLocation("a_Position"), GL_FLOAT, 0, 3, sizeof(float) * 5);
    shader->enableAttributeArray(shader->attributeLocation("a_Position"));
    shader->setAttributeBuffer(shader->attributeLocation("a_TextureCoordinates"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);
    shader->enableAttributeArray(shader->attributeLocation("a_TextureCoordinates"));

    glDrawArrays(m_mode, 0, m_number);
    shader->disableAttributeArray(shader->attributeLocation("a_Position"));
    shader->disableAttributeArray(shader->attributeLocation("a_TextureCoordinates"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void MarkGraphicItem::setInfo(const Point &pos, const string &path)
{
    m_pos = pos;

    m_path = path;

    // 设置纹理属性
    {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 加载并生成纹理
        int width, height, nrChannels;
        unsigned char *data = stbi_load(m_path.data(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    init();
}
