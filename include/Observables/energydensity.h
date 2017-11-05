#pragma once
#include <vector>
#include <string>
#include "observable.h"
#include "Math/latticemath.h"

class EnergyDensity : public Observable {
public:
    EnergyDensity();
    void initObservable(Lattice *lattice);
    void compute();
    const char* getName();

private:
    std::array<int, 4> m_size;
    std::string m_name;
    // auxiliary variables
    double m_norm;
    SU3 Gmn;
};
