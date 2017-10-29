#pragma once
#include "Actions/action.h"

// Implementation of the Pure Gauge Wilson Action
class PureGauge : public Action {
    public:
        PureGauge(double beta);
        void initAction(class Lattice* lattice);
        double compute(int x, int y, int z, int t, int mu,
                       struct SU3& newLink, struct SU3& constPart);
        struct SU3& computeConstant(int x, int y, int z, int t, int mu);
    private:
        double m_beta;
        class Lattice* m_lat = nullptr;
        // auxiliary variables
        struct SU3 constPart, staple, temp;
};
