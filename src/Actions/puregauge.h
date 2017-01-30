#pragma once
#include "action.h"


class PureGauge : public Action {
public:
    PureGauge(class System* system);

    // VIRTUAL FUNCTIONS REPLACEMENTS
    void computeAction();
private:
    class System* m_system = nullptr;
};
