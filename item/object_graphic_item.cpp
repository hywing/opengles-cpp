#include "object_graphic_item.h"

#include <iostream>
#include <fstream>
#include <sstream>

#ifndef GL_RED
#define GL_RED 0x1903
#endif


ObjectGraphicItem::ObjectGraphicItem()
{
    // 设置纹理属性
    {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        stbi_set_flip_vertically_on_load(false);

        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 加载并生成纹理
        int width, height, nrChannels;
        unsigned char *data = stbi_load(string(FileSystem::SOURCE_PATH + "res/obj/old/10.png").data(), &width, &height, &nrChannels, 0);
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
}

ObjectGraphicItem::~ObjectGraphicItem()
{

}

void ObjectGraphicItem::init()
{
    readObject3D(FileSystem::SOURCE_PATH + string("res/obj/old/tractor.obj"));

    auto &buffer = m_objects.begin()->second->vertex;

    m_mode   = GL_TRIANGLES;
    m_number = buffer.size() / 3;

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_buffer.size() * sizeof (float)), &m_buffer[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjectGraphicItem::draw()
{
    auto shader = ObjectShaderHelper::getInstance();
    auto matrix = MatrixHelper::getInstance();
    int  attr   = -1;
    shader->setUniformValue(shader->uniformLocation("vMatrix"), matrix->getFinalProjectMatrix());
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    attr = shader->attributeLocation("vTexture");
    shader->setUniformValue(attr, 0);
    for (auto &obj : m_objects)
    {
        shader->setUniformValue(shader->uniformLocation("vKa"), glm::vec3(obj.second->mtl.Ka[0], obj.second->mtl.Ka[1], obj.second->mtl.Ka[2]));
        shader->setUniformValue(shader->uniformLocation("vKd"), glm::vec3(obj.second->mtl.Kd[0], obj.second->mtl.Kd[1], obj.second->mtl.Kd[2]));
        shader->setUniformValue(shader->uniformLocation("vKs"), glm::vec3(obj.second->mtl.Ks[0], obj.second->mtl.Ks[1], obj.second->mtl.Ks[2]));

        // TODO 开启深度测试
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        // TODO 面剔除
        // 设置剔除哪些面。参数可以有三个：1、GL_BACK：只剔除背向面；2、GL_FRONT：只剔除正向面；3、GL_FRONT_AND_BACK：剔除正向面和背向面。GL_BACK是默认值
        glCullFace(GL_FRONT);
        // TODO 设置哪种环绕顺序是面向观察点的。默认值是GL_CCW（逆时针），当我们设置为GL_CW（顺时针）
        glFrontFace(GL_CW);

        attr = shader->attributeLocation("vPosition");
        glEnableVertexAttribArray(attr);
        glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), obj.second->vertex.data());

        attr = shader->attributeLocation("vNormal");
        glEnableVertexAttribArray(attr);
        glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), obj.second->normal.data());

        attr = shader->attributeLocation("vCoord");
        glEnableVertexAttribArray(attr);
        glVertexAttribPointer(attr, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), obj.second->texture.data());

        glDrawArrays(m_mode, 0, m_number);

        glDisableVertexAttribArray(shader->attributeLocation("vPosition"));
        glDisableVertexAttribArray(shader->attributeLocation("vNormal"));
        glDisableVertexAttribArray(shader->attributeLocation("vCoord"));

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void ObjectGraphicItem::readObject3D(const string &filePath)
{
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;

    std::vector<float>      oVs;                    // 原始顶点坐标列表
    std::vector<float>      oVNs;                   // 原始顶点法线列表
    std::vector<float>      oVTs;                   // 原始贴图坐标列表
    std::vector<float>      oFVs;                   // 面顶点
    std::vector<float>      oFVNs;
    std::vector<float>      oFVTs;

    std::vector<float>       vertexArray;
    std::vector<float>       normalArray;
    std::vector<float>       textureArray;
    std::map<string, MtlInfo> mTls;                 // 纹理结构填充容器
    std::shared_ptr<Object3D> nowObj;               // 当前对象
    MtlInfo nowMtl;                                 // 当前纹理结构体

    string parent;                                  // 要获取当前文件夹路径
    std::ifstream  br(filePath);
    br.open(filePath);
    std::stringstream fileStream;
    fileStream << br.rdbuf();
    assert(br.is_open());
    string temps;

    while (getline(fileStream, temps))
    {
        if (temps.empty() == false)
        {
            stringList tempsa;
            split(trimmed(temps), tempsa, " ");
            string     option = trimmed(tempsa[0]);
            parent             = FileSystem::SOURCE_PATH + "res/obj/old/";
            if (option == "mtllib")
            {
                // 这里要通过一个路径打开纹理文件读取mtl信息
                parent += tempsa[1];
                mTls = readMtl(trimmed(parent));
            }
            else if (option == "usemtl")
            {
                tempsa[1] = trimmed(tempsa[1]);
                if (mTls.empty() == false)
                {
                    nowMtl = mTls.at(tempsa[1]);
                }
                if (m_objects.find(tempsa[1]) != m_objects.end())
                {
                    nowObj = m_objects.at(tempsa[1]);
                }
                else
                {
                    nowObj = std::shared_ptr<Object3D>(new Object3D());
                    nowObj->mtl = nowMtl;
                    m_objects.insert(std::pair<string, std::shared_ptr<Object3D>>(tempsa[1], nowObj));
                }
            }
            else if (option == "v")
            {
                read(tempsa, oVs, true);
            }
            else if (option == "vn")
            {
                read(tempsa, oVNs);
            }
            else if (option == "vt")
            {
                read(tempsa, oVTs);
            }
            else if (option == "f")
            {
                for (uint i = 1; i < tempsa.size(); i++)
                {
                    stringList fs;
                    split(tempsa[i], fs, "/");
                    int         index;
                    if (fs.size() > 0)
                    {
                        int ovsSize      = oVs.size() / 3;
                        index            = (ovsSize - 1) - (std::abs(atoi(fs[0].data())) - 1);
                        float tempValueX = oVs.at(index * 3);
                        float tempValueY = oVs.at(index * 3 + 1);
                        float tempValueZ = oVs.at(index * 3 + 2);

                        if (tempValueX <= minX)
                        {
                            minX = tempValueX;
                        }
                        if (tempValueX >= maxX)
                        {
                            maxX = tempValueX;
                        }
                        if (tempValueY <= minY)
                        {
                            minY = tempValueY;
                        }
                        if (tempValueY >= maxY)
                        {
                            maxY = tempValueY;
                        }
                        if (tempValueZ <= minZ)
                        {
                            minZ = tempValueZ;
                        }
                        if (tempValueZ >= maxZ)
                        {
                            maxZ = tempValueZ;
                        }

                        nowObj->vertex.push_back(tempValueX);
                        nowObj->vertex.push_back(tempValueY);
                        nowObj->vertex.push_back(tempValueZ);
                    }
                    if (fs.size() > 1)
                    {
                        //贴图
                        int oVTsSize = oVTs.size() / 3;
                        index        = (oVTsSize - 1) - (std::abs(atoi(fs[1].data())) - 1);
                        nowObj->texture.push_back(oVTs.at(index * 3 + 1));
                        nowObj->texture.push_back(oVTs.at(index * 3));
                    }
                    if (fs.size() > 2)
                    {
                        //法线索引
                        int oVNsSize = oVNs.size() / 3;
                        index        = (oVNsSize - 1) - (std::abs(atoi(fs[2].data())) - 1);
                        nowObj->normal.push_back(oVNs.at(index * 3));
                        nowObj->normal.push_back(oVNs.at(index * 3 + 1));
                        nowObj->normal.push_back(oVNs.at(index * 3 + 2));
                    }
                }
            }
        }
    }

    br.close();
}

std::map<string, ObjectGraphicItem::MtlInfo> ObjectGraphicItem::readMtl(const string &stream)
{
    std::map<string, MtlInfo> map;
    std::ifstream br(stream.data());
    br.open(stream);
    string temps;
    MtlInfo *mtlInfo = nullptr;

    std::stringstream fileStream;
    fileStream << br.rdbuf();
    assert(br.is_open());

    while (getline(fileStream, temps))
    {
        stringList tempsa;

        split(trimmed(temps), tempsa, " ");

        if (trimmed(tempsa[0]) == "newmtl")
        {
            MtlInfo mtl;
            mtl.newmtl = trimmed(tempsa[1]);
            map.insert(std::pair<string, MtlInfo>(trimmed(tempsa[1]), mtl));
            mtlInfo = &map[trimmed(tempsa[1])];
        }
        else if (trimmed(tempsa[0]) == "illum")
        {
            mtlInfo->illum = atoi(tempsa[1].data());
        }
        else if (trimmed(tempsa[0]) == "Kd")
        {
            read(tempsa, mtlInfo->Kd);
        }
        else if (trimmed(tempsa[0]) == "Ka")
        {
            read(tempsa, mtlInfo->Ka);
        }
        else if (trimmed(tempsa[0]) == "Ke")
        {
            read(tempsa, mtlInfo->Ke);
        }
        else if (trimmed(tempsa[0]) == "Ks")
        {
            read(tempsa, mtlInfo->Ks);
        }
        else if (trimmed(tempsa[0]) == "Ns")
        {
            mtlInfo->Ns = atof(tempsa[1].data());
        }
        else if (trimmed(tempsa[0]) == "map_Kd")
        {
            mtlInfo->map_Kd = trimmed(tempsa[1]);
        }
    }
    br.close();
    return map;
}

void ObjectGraphicItem::read(const stringList &value, std::vector<float> &list, bool boost)
{
    if (boost)
    {
        for (uint i = 2; i < value.size(); i++)
        {
            list.push_back(atof(value[i].data()) * 100.0f);
        }
    }
    else
    {
        for (uint i = 1; i < value.size(); i++)
        {
            list.push_back(atof(value[i].data()));
        }
    }
}

void ObjectGraphicItem::read(const stringList &value, float *fv)
{
    for (uint i = 1; i < value.size() && i < 4; i++)
    {
        fv[i - 1] = atof(value[i].data());
    }
}

string ObjectGraphicItem::trimmed(const string &str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }

    pos = str.find_first_not_of('\t');
    if (pos == string::npos)
    {
        return str;
    }

    string::size_type pos2;
    pos2 = str.find_last_not_of('\r');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }

    pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }

    pos2 = str.find_last_not_of('\t');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }

    return str.substr(pos);
}

void ObjectGraphicItem::split(string str, ObjectGraphicItem::stringList &v, string spacer)
{
    uint pos1;
    auto pos2 = string::npos;
    pos2 = 0;
    int len=spacer.length();     //记录分隔符的长度
    pos1=0;
    pos2=str.find(spacer);
    while( pos2 != string::npos )
    {
        v.push_back( str.substr(pos1,pos2-pos1) );
        pos1 = pos2 + len;
        pos2 = str.find(spacer,pos1);    // 从str的pos1位置开始搜寻spacer
    }
    if(pos1 != str.length()) //分割最后一个部分
       v.push_back(str.substr(pos1));
}
