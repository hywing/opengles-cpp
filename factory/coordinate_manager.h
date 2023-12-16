#ifndef COORDINATEMANAGER_H
#define COORDINATEMANAGER_H

#include "manager_base.h"
#include <memory>

class Axis;

class CoordinateManager : public ManagerBase
{
public:
    CoordinateManager();

    virtual void init();

    virtual void draw();

private:
    std::shared_ptr<Axis> m_xAxis;
    std::shared_ptr<Axis> m_yAxis;
    std::shared_ptr<Axis> m_zAxis;
};

#endif // COORDINATEMANAGER_H
