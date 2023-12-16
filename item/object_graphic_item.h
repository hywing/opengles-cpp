#ifndef OBJECTGRAPHICITEM_H
#define OBJECTGRAPHICITEM_H

#include "graphic_item_base.h"

#include <map>
#include <memory>

class ObjectGraphicItem : public GraphicItemBase
{
    using stringList = std::vector<string>;

    struct MtlInfo
    {
        string newmtl;
        float   Ka[3];  //阴影色
        float   Kd[3];  //固有色
        float   Ks[3];  //高光色
        float   Ke[3];  //
        float   Ns;     // shininess
        string map_Kd; //固有纹理贴图
        string map_Ks; //高光纹理贴图
        string map_Ka; //阴影纹理贴图
        int     illum;

        MtlInfo()
        {
        }

        MtlInfo(const MtlInfo &mi)
        {
            this->newmtl = mi.newmtl;
            this->Ka[0]  = mi.Ka[0];
            this->Ka[1]  = mi.Ka[1];
            this->Ka[2]  = mi.Ka[2];

            this->Kd[0] = mi.Kd[0];
            this->Kd[1] = mi.Kd[1];
            this->Kd[2] = mi.Kd[2];

            this->Ks[0] = mi.Ks[0];
            this->Ks[1] = mi.Ks[1];
            this->Ks[2] = mi.Ks[2];

            this->Ke[0] = mi.Ke[0];
            this->Ke[1] = mi.Ke[1];
            this->Ke[2] = mi.Ke[2];

            this->Ns = mi.Ns;

            this->map_Kd = mi.map_Kd;
            this->map_Ks = mi.map_Ks;
            this->map_Ka = mi.map_Ka;
            this->illum  = mi.illum;
        }

        MtlInfo &operator =(const MtlInfo &mi)
        {
            if (this != &mi)
            {
                this->newmtl = mi.newmtl;
                this->Ka[0]  = mi.Ka[0];
                this->Ka[1]  = mi.Ka[1];
                this->Ka[2]  = mi.Ka[2];

                this->Kd[0] = mi.Kd[0];
                this->Kd[1] = mi.Kd[1];
                this->Kd[2] = mi.Kd[2];

                this->Ks[0] = mi.Ks[0];
                this->Ks[1] = mi.Ks[1];
                this->Ks[2] = mi.Ks[2];

                this->Ke[0] = mi.Ke[0];
                this->Ke[1] = mi.Ke[1];
                this->Ke[2] = mi.Ke[2];

                this->Ns = mi.Ns;

                this->map_Kd = mi.map_Kd;
                this->map_Ks = mi.map_Ks;
                this->map_Ka = mi.map_Ka;
                this->illum  = mi.illum;
            }
            return *this;
        }

        bool operator ==(const MtlInfo &mi)
        {
            bool ret = true;
            ret  &= (this->newmtl == mi.newmtl);
            ret  &= (this->Ka[0] == mi.Ka[0]);
            ret  &= (this->Ka[1] == mi.Ka[1]);
            ret  &= (this->Ka[2] == mi.Ka[2]);

            ret  &= (this->Kd[0] == mi.Kd[0]);
            ret  &= (this->Kd[1] == mi.Kd[1]);
            ret  &= (this->Kd[2] == mi.Kd[2]);

            ret  &= (this->Ks[0] == mi.Ks[0]);
            ret  &= (this->Ks[1] == mi.Ks[1]);
            ret  &= (this->Ks[2] == mi.Ks[2]);

            ret  &= (this->Ke[0] == mi.Ke[0]);
            ret  &= (this->Ke[1] == mi.Ke[1]);
            ret  &= (this->Ke[2] == mi.Ke[2]);

            ret  &= (this->Ns == mi.Ns);

            ret  &= (this->map_Kd == mi.map_Kd);
            ret  &= (this->map_Ks == mi.map_Ks);
            ret  &= (this->map_Ka == mi.map_Ka);
            ret  &= (this->illum  == mi.illum);
            return ret;
        }

        bool operator !=(const MtlInfo &mi) {
            return !(*this == mi);
        }
    };

    struct Object3D
    {
        std::vector<float> vertex;
        std::vector<float> normal;
        std::vector<float> texture;
        MtlInfo mtl;

        Object3D()
        {
        }

        ~Object3D()
        {
//            std::cout << "delete ... " << std::endl;
        }

        Object3D(const Object3D &obj)
        {
            this->vertex  = obj.vertex;
            this->normal  = obj.normal;
            this->texture = obj.texture;
            this->mtl = obj.mtl;
        }

        Object3D &operator=(const Object3D &obj)
        {
            if (this != &obj)
            {
                this->vertex  = obj.vertex;
                this->normal  = obj.normal;
                this->texture = obj.texture;
                this->mtl     = obj.mtl;
            }

            return *this;
        }
    };

public:
    ObjectGraphicItem();

    ~ObjectGraphicItem();

    virtual void init() override;

    virtual void draw() override;

protected:
    void readObject3D(const string &filePath);

    std::map<string, MtlInfo> readMtl(const string &stream);

    void read(const stringList &value, std::vector<float> &list, bool boost = false);

    void read(const stringList &value, float *fv);

    string trimmed(const string& str);

    void split(string str, stringList &v, string spacer);

private:
    std::map<std::string, std::shared_ptr<Object3D>> m_objects;
};

#endif // OBJECTGRAPHICITEM_H
