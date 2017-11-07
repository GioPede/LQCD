#pragma once
#include <vector>
#include <string>
#include "observable.h"
#include "Math/latticemath.h"

class SuperObs : public Observable {
public:
    SuperObs();
    void initObservable(Lattice *lattice);
    void compute();

private:
    std::array<int, 4> m_size;
    // auxiliary variables
    double m_norm;

    std::array<std::array<SU3,4>,4> clovers;
    SU3 plaqSum;

    std::array<int,24> mu;
    std::array<int,24> nu;
    std::array<int,24> rho;
    std::array<int,24> sig;
    void superGatherResults();
};
