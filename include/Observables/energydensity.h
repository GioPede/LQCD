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
        double value() { return m_value; }
        const char* getName();

    private:
        std::vector<int> m_size;
        std::string m_name;
        double m_value = 0.0;
        // auxiliary variables
        double m_norm;
        SU3 Gmn;

};
