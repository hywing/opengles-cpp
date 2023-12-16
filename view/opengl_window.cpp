#include "opengl_window.h"
#include "opengl_shader_program.h"
#include "line_graphic_item.h"
#include "grid_graphic_item.h"
#include "ground_graphic_item.h"
#include "mark_graphic_item.h"
#include "sky_graphic_item.h"
#include "text_graphic_item.h"
#include "trace_graphic_item.h"
#include "object_graphic_item.h"
#include "model_graphic_item.h"
#include "coordinate_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


int OpenglWindow::g_width = 1280;

int OpenglWindow::g_height = 800;

float OpenglWindow::g_lastX = g_width / 2.0f;

float OpenglWindow::g_lastY = g_height / 2.0f;

bool OpenglWindow::g_firstMouse = true;

OpenglWindow *OpenglWindow::g_instance = nullptr;

int OpenglWindow::g_flag = 0;

bool OpenglWindow::g_pressed = false;

OpenglWindow::OpenglWindow()
{
    initWindow();
}

OpenglWindow::~OpenglWindow()
{
    for(auto iter : m_nomalContainer) {
        if(iter) {
            delete iter;
        }
    }

    m_nomalContainer.clear();

    for(auto iter : m_textureContainer) {
        if(iter) {
            delete iter;
        }
    }

    m_textureContainer.clear();

    if(m_sky)
        delete m_sky;

    if(m_obj)
        delete m_obj;

    if(m_model)
        delete m_model;

    if(m_coordinate) {
        delete m_coordinate;
    }
}

void OpenglWindow::exec()
{
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        paintGL();
        glfwSwapBuffers(m_window);
    }
    glfwTerminate();
}

void OpenglWindow::onViewAngleChanged(int flag)
{
    if (flag >= 3)
    {
        flag = 0;
    }
    if (flag == 0)
    {
        m_elevationAngle = 15.0f;
        m_eyeDistance    = 280.0f;
    }
    else if (flag == 1)
    {
        m_elevationAngle = 15.0f;
        m_eyeDistance    = 100.0f;
    }
    else if (flag == 2)
    {
        m_elevationAngle = 89.9f;
        m_eyeDistance    = 600.0f;
    }

    MatrixHelper::getInstance()->updateCameraRelativePosition(m_elevationAngle, m_azimuthAngle, m_eyeDistance);
}

void OpenglWindow::onWheelValueChanged(double value)
{
    if(value > 0) {
        if(m_eyeDistance >= 70) {
            m_eyeDistance -= 20;
        }
        MatrixHelper::getInstance()->updateCameraRelativePosition(m_elevationAngle, m_azimuthAngle, m_eyeDistance);
    } else {
        if(m_eyeDistance <= 1800) {
            m_eyeDistance += 20;
        }
        MatrixHelper::getInstance()->updateCameraRelativePosition(m_elevationAngle, m_azimuthAngle, m_eyeDistance);
    }
}

void OpenglWindow::onYawValueChanged(float yaw)
{
    m_azimuthAngle = yaw;
    MatrixHelper::getInstance()->updateCameraRelativePosition(m_elevationAngle, m_azimuthAngle, m_eyeDistance);
}

void OpenglWindow::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glfwMakeContextCurrent(m_window);

//    NormalShaderHelper::getInstance()->bind();
//    {
        auto gnd = new GroundGrahicItem;
        gnd->init();
        m_nomalContainer.push_back(gnd);

        auto grid = new GridGraphicItem;
        grid->init();
        m_nomalContainer.push_back(grid);

        auto line = new LineGraphicItem;
        line->setColor(Color(255, 0, 0));
        line->setPoints(Point(-1000, -1000, 0), Point(1000, 1000, 0));
        line->init();
        m_nomalContainer.push_back(line);

        float max = 2000;
        auto xAxis = new LineGraphicItem;
        xAxis->setColor(Color(0, 255, 0));
        xAxis->setPoints(Point(-max, 0, 0), Point(max, 0, 0));
        m_nomalContainer.push_back(xAxis);

        auto yAxis = new LineGraphicItem;
        yAxis->setColor(Color(0, 255, 0));
        yAxis->setPoints(Point(0, -max, 0), Point(0, max, 0));
        m_nomalContainer.push_back(yAxis);

        auto trace = new TraceGraphicItem;
        trace->setColor(glm::vec3(1, 0, 0));
        std::vector<Point> points;
        for(double x = -5000; x <= 5000; x += 1) {
            points.push_back(Point(x, 100 + 100 * std::sin(x / 50)));
        }
        trace->addPoints(points);
        m_nomalContainer.push_back(trace);
//    }
//    NormalShaderHelper::getInstance()->release();

    TextureShaderHelper::getInstance()->bind();
    {
        auto mark = new MarkGraphicItem;
        mark->setInfo(Point(0, 0, 0), FileSystem::SOURCE_PATH + "res/pix/mark_a.png");
        m_textureContainer.push_back(mark);
        mark = new MarkGraphicItem;
        mark->setInfo(Point(50, 50, 0), FileSystem::SOURCE_PATH + "res/pix/mark_b.png");
        m_textureContainer.push_back(mark);
        mark = new MarkGraphicItem;
        mark->setInfo(Point(100, 100, 0), FileSystem::SOURCE_PATH + "res/pix/mark_c.png");
        m_textureContainer.push_back(mark);
        mark = new MarkGraphicItem;
        mark->setInfo(Point(150, 150, 0), FileSystem::SOURCE_PATH + "res/pix/mark_d.png");
        m_textureContainer.push_back(mark);
    }
    TextureShaderHelper::getInstance()->release();

    SkyShaderHelper::getInstance()->bind();
    {
        m_sky = new SkyGraphicItem;
        m_sky->init();
    }
    SkyShaderHelper::getInstance()->release();

    // 绘制文字
//    {
//        int value = 400;
//        TextGraphicItem::setText(Point(value, -50, 0), "x", glm::vec3(1.0, 0, 0));
//        TextGraphicItem::setText(Point(-50, value, 0), "y", glm::vec3(0, 1, 0));
//        TextGraphicItem::setText(Point(-50, -50, 0), "0", glm::vec3(0, 0, 1));
//        TextGraphicItem::setText(Point(150, 250, 0), "广东皓行科技有限公司", glm::vec3(1, 0, 0), 0.3);
//    }

//    ObjectShaderHelper::getInstance()->bind();
//    {
//        m_obj = new ObjectGraphicItem;
//        m_obj->init();
//    }
//    ObjectShaderHelper::getInstance()->release();

    ModelShaderHelper::getInstance()->bind();
    {
        m_model = new ModelGraphicItem;
    }
    ModelShaderHelper::getInstance()->release();

    m_coordinate = new CoordinateManager;
    m_coordinate->init();
}

void OpenglWindow::resizeGL(GLFWwindow *, int w, int h)
{
    float aspect = w * 1.0 / float(h ? h : 1);
    float fov = 45.0f, zNear = 1.0f, zFar = 2000.f;

    g_width = w;
    g_height = h;

    glViewport(0, 0, w, h);

    MatrixHelper::getInstance()->setPerspetiveM(fov, aspect, zNear, zFar);
}

void OpenglWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    MatrixHelper::getInstance()->setCameraMatrix();
//    MatrixHelper::getInstance()->translate(0, 0, 0);

    SkyShaderHelper::getInstance()->bind();
    MatrixHelper::getInstance()->setIdentityM();
    MatrixHelper::getInstance()->translate(0, 0, -1100.0f);
    MatrixHelper::getInstance()->rotate(90.0f, 1.0f, 0.0f, 0.0f);
    m_sky->draw();
    SkyShaderHelper::getInstance()->release();

    NormalShaderHelper::getInstance()->bind();
    MatrixHelper::getInstance()->setIdentityM();
    for(auto item : m_nomalContainer) {
        item->draw();
    }
    NormalShaderHelper::getInstance()->release();

    TextureShaderHelper::getInstance()->bind();
    for(auto item : m_textureContainer) {
        item->draw();
    }
    TextureShaderHelper::getInstance()->release();

//    ObjectShaderHelper::getInstance()->bind();
//    {
//        m_obj->draw();
//    }
//    ObjectShaderHelper::getInstance()->release();

    ModelShaderHelper::getInstance()->bind();
    {
        m_model->draw();
    }
    ModelShaderHelper::getInstance()->release();

    m_coordinate->draw();

    TextGraphicItem::drawText();
}

void OpenglWindow::initWindow()
{
    setvbuf(stdout, 0, _IONBF, 0);

    MatrixHelper::getInstance()->init(m_elevationAngle, m_azimuthAngle, m_eyeDistance);

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    m_window = glfwCreateWindow(g_width, g_height, __FILE__, NULL, NULL);
    glfwMakeContextCurrent(m_window);
    glfwSetWindowTitle(m_window, "opengles api demos");

    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );

    initializeGL();
    resizeGL(m_window, g_width, g_height);

    glfwSetKeyCallback(m_window, keyboardCallback);
    glfwSetCursorPosCallback(m_window, mousePositionCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetScrollCallback(m_window, wheelCallback);
    glfwSetWindowSizeCallback(m_window, resizeGL);
    g_instance = this;
}

void OpenglWindow::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    static float yaw = 0;
    if (action != GLFW_PRESS)
        return;
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_A:
    {
        yaw -= 5;
        g_instance->onYawValueChanged(yaw);
        break;
    }
    case GLFW_KEY_D:
    {
        yaw += 5;
        g_instance->onYawValueChanged(yaw);
        break;
    }
    case GLFW_KEY_SPACE:
    {
        if(g_instance) {
            g_flag++;
            if(g_flag >= 3) {
                g_flag = 0;
            }
            g_instance->onViewAngleChanged(g_flag);
        }
        break;
    }
    default:
        break;
    }
}

void OpenglWindow::mouseButtonCallback(GLFWwindow *, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch(button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            g_pressed = true;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            break;
        default:
            break;
        }
    }

    if (action == GLFW_RELEASE) {
        g_pressed = false;
    }
}

void OpenglWindow::wheelCallback(GLFWwindow *, double x, double y)
{
    double value;
    if(g_instance) {
        if(x < 0 || y < 0) {
            value = -1;
        }
        else {
            value = 1;
        }
        g_instance->onWheelValueChanged(value);
    }
    //    std::cout << "x : " << x << ", y : " << y << std::endl;
}

void OpenglWindow::mousePositionCallback(GLFWwindow *, double xposIn, double yposIn)
{
    if(g_pressed != GLFW_PRESS) {
        return ;
    }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (g_firstMouse)
    {
        g_lastX = xpos;
        g_lastY = ypos;
        g_firstMouse = false;
    }

    float xoffset = xpos - g_lastX;
    float yoffset = g_lastY - ypos; // reversed since y-coordinates go from bottom to top

    g_lastX = xpos;
    g_lastY = ypos;

    if(g_instance) {
        MatrixHelper::getInstance()->updateCameraPosition(-xoffset, -yoffset, 0);
    }
}
