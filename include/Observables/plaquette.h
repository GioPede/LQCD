#pragma once
#include <vector>
#include "observable.h"
#include "Math/latticemath.h"

class Plaquette : public Observable {
    public:
        Plaquette();
        void initObservable(Lattice *lattice);
        void compute();
        double value() { return m_value; }

    private:
        std::vector<int> m_size;
        double m_value = 0.0;
        // auxiliary variables
        double m_norm;
        SU3 plaq;
};
