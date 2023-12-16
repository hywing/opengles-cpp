#include "sky_graphic_item.h"

unsigned int loadCubemap(const std::vector<std::string> &faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

SkyGraphicItem::SkyGraphicItem()
{

}

void SkyGraphicItem::init()
{
    float MULTIPLE = 10000;

    Point a = Point(-MULTIPLE, -MULTIPLE, 0);
    Point b = Point(MULTIPLE, MULTIPLE, 0);

    m_xBegin = a.point.x;
    m_xEnd   = b.point.x;
    m_yBegin = a.point.y;
    m_yEnd   = b.point.y;

    createCubeTexture();

    // 顶点
    GLfloat buffer[] = {// positions
                       -MULTIPLE, MULTIPLE,  -MULTIPLE, -MULTIPLE, -MULTIPLE, -MULTIPLE, MULTIPLE,  -MULTIPLE, -MULTIPLE,
                       MULTIPLE,  -MULTIPLE, -MULTIPLE, MULTIPLE,  MULTIPLE,  -MULTIPLE, -MULTIPLE, MULTIPLE,  -MULTIPLE,

                       -MULTIPLE, -MULTIPLE, MULTIPLE,  -MULTIPLE, -MULTIPLE, -MULTIPLE, -MULTIPLE, MULTIPLE,  -MULTIPLE,
                       -MULTIPLE, MULTIPLE,  -MULTIPLE, -MULTIPLE, MULTIPLE,  MULTIPLE,  -MULTIPLE, -MULTIPLE, MULTIPLE,

                       MULTIPLE,  -MULTIPLE, -MULTIPLE, MULTIPLE,  -MULTIPLE, MULTIPLE,  MULTIPLE,  MULTIPLE,  MULTIPLE,
                       MULTIPLE,  MULTIPLE,  MULTIPLE,  MULTIPLE,  MULTIPLE,  -MULTIPLE, MULTIPLE,  -MULTIPLE, -MULTIPLE,

                       -MULTIPLE, -MULTIPLE, MULTIPLE,  -MULTIPLE, MULTIPLE,  MULTIPLE,  MULTIPLE,  MULTIPLE,  MULTIPLE,
                       MULTIPLE,  MULTIPLE,  MULTIPLE,  MULTIPLE,  -MULTIPLE, MULTIPLE,  -MULTIPLE, -MULTIPLE, MULTIPLE,

                       -MULTIPLE, MULTIPLE,  -MULTIPLE, MULTIPLE,  MULTIPLE,  -MULTIPLE, MULTIPLE,  MULTIPLE,  MULTIPLE,
                       MULTIPLE,  MULTIPLE,  MULTIPLE,  -MULTIPLE, MULTIPLE,  MULTIPLE,  -MULTIPLE, MULTIPLE,  -MULTIPLE,

                       -MULTIPLE, -MULTIPLE, -MULTIPLE, -MULTIPLE, -MULTIPLE, MULTIPLE,  MULTIPLE,  -MULTIPLE, -MULTIPLE,
                       MULTIPLE,  -MULTIPLE, -MULTIPLE, -MULTIPLE, -MULTIPLE, MULTIPLE,  MULTIPLE,  -MULTIPLE, MULTIPLE};


    m_mode   = GL_TRIANGLES;
    m_number = 36;

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SkyGraphicItem::draw()
{
    auto shader = SkyShaderHelper::getInstance();
    auto matrix = MatrixHelper::getInstance();
    shader->setUniformValue(shader->uniformLocation("u_Matrix"), matrix->getFinalProjectMatrix());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    shader->setUniformValue(shader->uniformLocation("u_TextureUnit"), 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    shader->setAttributeBuffer(shader->attributeLocation("a_Position"), GL_FLOAT, 0, 3, sizeof(float) * 3);
    shader->enableAttributeArray(shader->attributeLocation("a_Position"));

    glDrawArrays(m_mode, 0, m_number);
    glDepthFunc(GL_LESS);
    shader->disableAttributeArray(shader->attributeLocation("a_Position"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyGraphicItem::createCubeTexture()
{
    std::vector<std::string> faces =
    {
        FileSystem::SOURCE_PATH + "res/pix/right.jpg",
        FileSystem::SOURCE_PATH + "res/pix/left.jpg",
        FileSystem::SOURCE_PATH + "res/pix/top.jpg",
        FileSystem::SOURCE_PATH + "res/pix/bottom.jpg",
        FileSystem::SOURCE_PATH + "res/pix/front.jpg",
        FileSystem::SOURCE_PATH + "res/pix/back.jpg"
    };

    m_texture = loadCubemap(faces);
}
