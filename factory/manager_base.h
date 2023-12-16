#ifndef MANAGERBASE_H
#define MANAGERBASE_H


class ManagerBase
{
public:
    ManagerBase();

    virtual ~ManagerBase();

    virtual void init();

    virtual void draw();
};

#endif // MANAGERBASE_H
