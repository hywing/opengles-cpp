#ifndef MATRIXHELPER_H
#define MATRIXHELPER_H

#include <glm/glm.hpp>
#include <glm/common.hpp>

#ifndef SINGLETON
#define SINGLETON(CLASS)        \
private:                        \
    CLASS()                     \
    {                           \
    }                           \
    ~CLASS()                    \
    {                           \
    }                           \
                                \
public:                         \
    static CLASS *getInstance() \
    {                           \
        static CLASS instance;  \
        return &instance;       \
    }
#endif

class MatrixHelper
{
    SINGLETON(MatrixHelper)
public:
    // 倾斜角、方位角、距离
    void init(float elevationAngle, float azimuthAngle, float distance);

    void setIdentityM();

    void setCameraMatrix();

    void setModelIdentityM();
    /**
     * 平移
     * **/
    void translate(float x, float y, float z);

    /**
     * 旋转
     * **/
    void rotate(float angle, float x, float y, float z);

    /**
     * 伸缩
     * **/
    void scale(float factor);

    /**
     * 设置透视投影参数
     * **/
    void setPerspetiveM(float degree, float aspect, float zNear, float zFar);

    /**
     * 设置正交投影参数
     * **/
    void setOrthoM(float left, float right, float bottom, float top, float near, float far);

    /**
     * 获取最终的总矩阵
     * @return 视图投影变换矩阵
     */
    glm::mat4x4 getFinalProjectMatrix();

    glm::mat4x4 getModelMatrix();

    glm::mat4x4 getViewMatrix();

    glm::mat4x4 getProjectMatrix();

    /**
     * 更新相机位置,xyz为Opengl坐标系
     * @param elevationAngle  俯仰角
     * @param azimuthAngle    方位角
     * @param distance        距离
     */
    void updateCameraRelativePosition(float elevationAngle, float azimuthAngle, float distance);

    void updateCameraPosition(float targetX, float targetY, float targetZ);

    void updateCameraChangePosition(float targetX, float targetY, float targetZ);

private:
    glm::vec3 m_relativeCamera;
    glm::vec3 m_absoluteCamera;

    glm::mat4x4 m_viewMatrix;
    glm::mat4x4 m_projectionMatrix;
    glm::mat4x4 m_viewProjectMatrix;
    glm::mat4x4 m_modelMatrix;
    glm::mat4x4 m_mvpMatrix;

    glm::vec3 m_target;

    const float PI = 3.1415926535898;
};

#endif // MATRIXHELPER_H
