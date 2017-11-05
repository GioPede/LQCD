#pragma once
#include "Actions/action.h"
#include "Math/latticemath.h"

// Implementation of the Pure Gauge Wilson Action
class PureGauge : public Action {
public:
    PureGauge(Lattice* lattice, double beta) {
              m_lat = lattice; m_beta = beta; }
    PureGauge(double beta);
    double compute(int x, int y, int z, int t, int mu, SU3& newLink);
    void   computeStaples(int x, int y, int z, int t, int mu);
    SU3    computeDerivative(int x, int y, int z, int t, int mu);

private:
    double m_beta;

    // auxiliary variables
    struct SU3 m_constPart, m_omega;
};

