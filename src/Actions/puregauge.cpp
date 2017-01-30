#include <cstdio>
#include <cmath>
#include "puregauge.h"
#include "action.h"
#include "../system.h"


PureGauge::PureGauge(System* system) : Action(system) {
    m_system = system;
}

void PureGauge::computeAction(){

}
