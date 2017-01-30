#pragma once
#include <vector>

class Action {
public:
    // CONSTRUCTOR
    Action(class System* system);

    virtual void computeAction() = 0;



protected:
    // PROTECTED VARIABLES
    class System*                        m_system = nullptr;

};

