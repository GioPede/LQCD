#pragma once
#include "observable.h"
#include <vector>

class Plaquette : public Observable {
    public:
        Plaquette();
        void initObservable(class Lattice *lattice);
        void compute();
        double value() { return m_value; }

    private:
        std::vector<int> m_size;
        double m_value = 0.0;
};
