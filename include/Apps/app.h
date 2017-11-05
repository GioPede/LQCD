#pragma once
#include <array>
#include <vector>

class App{
public:
    void setAction(class Action *action);
    virtual void createLattice(std::array<int,4> latticeSize);
    void addObservable(class Observable* observable);

    // Virtual execution
    virtual void execute() = 0;

protected:
    class Action* m_act = nullptr;
    class Lattice* m_lat = nullptr;
    std::array<int,4> m_size;
    std::vector<class Observable*> m_obs;

};
