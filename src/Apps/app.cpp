#include "lqcd.h"

void App::createLattice(std::array<int,4> latticeSize){
    m_size = latticeSize;
    m_lat = new Lattice(m_size);
}

void App::setAction(Action* action){
    m_act = action;
}

void App::addObservable(Observable *observable){
    m_obs.push_back(observable);
}
