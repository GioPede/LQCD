#pragma once
#include <vector>
#include <string>
#include "observable.h"
#include "Math/latticemath.h"

class Plaquette : public Observable {
public:
    Plaquette();
    void initObservable(Lattice *lattice);
    void compute();
    double value() { return m_value; }
    const char* getName();

private:
    std::array<int, 4> m_size;
    double m_value = 0.0;
    // auxiliary variables
    double m_norm;
    SU3 plaq;
    std::string m_name;
};
