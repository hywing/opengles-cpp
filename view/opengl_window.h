#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include "graphic_item_base.h"
#include "opengl_shader_program.h"

class SkyGraphicItem;
class ObjectGraphicItem;
class ModelGraphicItem;
class CoordinateManager;

class OpenglWindow
{
public:
    OpenglWindow();

    ~OpenglWindow();

    void exec();

    void onViewAngleChanged(int g_flag);

    void onWheelValueChanged(double value);

    void onYawValueChanged(float yaw);

protected:
    void initializeGL();

    static void resizeGL(GLFWwindow*, int w, int h);

    void paintGL();

private:
    void initWindow();

    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void mouseButtonCallback(GLFWwindow * , int button, int action, int mods);

    static void wheelCallback(GLFWwindow* , double x, double y);

    static void mousePositionCallback(GLFWwindow *, double xposIn, double yposIn);

private:
    GLFWwindow* m_window;

    // 摄像机初始方位角
    float m_azimuthAngle = 0.0f;

    // 摄像机初始仰角
    float m_elevationAngle = 89.9f;

    // 摄像机与目标点的初始距离
    float m_eyeDistance = 1000.0f;

    std::vector<GraphicItemBase *> m_nomalContainer;
    std::vector<GraphicItemBase *> m_textureContainer;

    SkyGraphicItem *m_sky = nullptr;
    ObjectGraphicItem *m_obj = nullptr;
    ModelGraphicItem *m_model = nullptr;
    CoordinateManager *m_coordinate = nullptr;

    static int g_width;
    static int g_height;
    static float g_lastX;
    static float g_lastY;
    static bool g_firstMouse;
    static OpenglWindow *g_instance;
    static int g_flag;
    static bool g_pressed;
};

#endif // OPENGLWINDOW_H
