#include "matrix_helper.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

void MatrixHelper::init(float elevationAngle, float azimuthAngle, float distance)
{
    updateCameraRelativePosition(elevationAngle, azimuthAngle, distance);

    setIdentityM();
}

void MatrixHelper::setIdentityM()
{
    m_modelMatrix = glm::mat4(1.0f);
}

void MatrixHelper::setCameraMatrix()
{
    m_absoluteCamera = m_relativeCamera + m_target;
    m_viewMatrix = glm::mat4(1.0f);
    m_viewMatrix = glm::lookAt(m_absoluteCamera, m_target, glm::vec3(0.0f, 0.0f, 1.0f));
}

void MatrixHelper::setModelIdentityM()
{
    m_modelMatrix = glm::mat4(1.0f);
}

void MatrixHelper::translate(float x, float y, float z)
{
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(x, y, z));
}

void MatrixHelper::rotate(float angle, float x, float y, float z)
{
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), glm::vec3(x, y, z));
}

void MatrixHelper::scale(float factor)
{
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(factor, factor, factor));
}

void MatrixHelper::setPerspetiveM(float degree, float aspect, float zNear, float zFar)
{
    // 计算焦距
    float angle = (float)(degree * PI / 180.0);
    float a = (float)(1.0f / std::tan(angle / 2.0));

    m_projectionMatrix[0][0] = a / aspect;
    m_projectionMatrix[0][1] = 0.0f;
    m_projectionMatrix[0][2] = 0.0f;
    m_projectionMatrix[0][3] = 0.0f;

    m_projectionMatrix[1][0] = 0.0f;
    m_projectionMatrix[1][1] = a;
    m_projectionMatrix[1][2] = 0.0f;
    m_projectionMatrix[1][3] = 0.0f;

    m_projectionMatrix[2][0] = 0.0f;
    m_projectionMatrix[2][1] = 0.0f;
    m_projectionMatrix[2][2] = -((zFar + zNear) / (zFar - zNear));
    m_projectionMatrix[2][3] = -1.0f;

    m_projectionMatrix[3][0] = 0.0f;
    m_projectionMatrix[3][1] = 0.0f;
    m_projectionMatrix[3][2] = -((2.0f * zFar * zNear) / (zFar - zNear));
    m_projectionMatrix[3][3] = 0.0f;
}

void MatrixHelper::setOrthoM(float left, float right, float bottom, float top, float near, float far)
{
    m_projectionMatrix = glm::mat4(1.0f);
    m_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}

glm::mat4x4 MatrixHelper::getFinalProjectMatrix()
{
    m_viewProjectMatrix = m_projectionMatrix * m_viewMatrix;
    m_mvpMatrix = m_viewProjectMatrix * m_modelMatrix;
    return m_mvpMatrix;
}

glm::mat4x4 MatrixHelper::getModelMatrix()
{
    return m_modelMatrix;
}

glm::mat4x4 MatrixHelper::getViewMatrix()
{
    return m_viewMatrix;
}

glm::mat4x4 MatrixHelper::getProjectMatrix()
{
    return m_projectionMatrix;
}

void MatrixHelper::updateCameraRelativePosition(float elevationAngle, float azimuthAngle, float distance)
{
    float mCameraRelativeX = (float)(distance * std::cos(elevationAngle / 180 * PI) * std::sin(azimuthAngle / 180 * PI));
    float mCameraRelativeY = -(float)(distance * std::cos(elevationAngle / 180 * PI) * std::cos(azimuthAngle / 180 * PI));
    float mCameraRelativeZ = (float)(std::sin(elevationAngle / 180 * PI) * distance);
    m_relativeCamera = glm::vec3(mCameraRelativeX, mCameraRelativeY, mCameraRelativeZ);
}

void MatrixHelper::updateCameraPosition(float targetX, float targetY, float targetZ)
{
    m_target += glm::vec3(targetX, targetY, targetZ);
}

void MatrixHelper::updateCameraChangePosition(float targetX, float targetY, float targetZ)
{
    m_target = glm::vec3(targetX, targetY, targetZ);
}
