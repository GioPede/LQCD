#pragma once
#include "Actions/action.h"
#include "Math/latticemath.h"

// Implementation of the Pure Gauge Wilson Action
class PureGauge : public Action {
public:
    PureGauge(Lattice* lattice, double beta) :
        m_lat(lattice), m_beta(beta) {}
    double compute(int x, int y, int z, int t, int mu,
                   SU3& newLink, SU3& constPart);
    SU3& computeConstant(int x, int y, int z, int t, int mu);

private:
    double m_beta;
    Lattice* m_lat = nullptr;

    // auxiliary variables
    struct SU3 constPart, staple, temp;
};

