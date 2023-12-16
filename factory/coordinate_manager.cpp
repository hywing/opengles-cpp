#include "coordinate_manager.h"
#include "axis.h"

CoordinateManager::CoordinateManager()
{

}

void CoordinateManager::init()
{
    m_xAxis = std::shared_ptr<Axis>(new Axis);
    m_xAxis->setInfo("x");

    m_yAxis = std::shared_ptr<Axis>(new Axis);
    m_yAxis->setInfo("y");

//    m_zAxis = std::shared_ptr<Axis>(new Axis);
//    m_zAxis->setInfo("z");
}

void CoordinateManager::draw()
{
    m_xAxis->draw();
    m_yAxis->draw();
//    m_zAxis->draw();
}
