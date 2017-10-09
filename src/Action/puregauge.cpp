#include "puregauge.h"
#include "../Utils/lattice.h"
#include "../Math/su3.h"
#include "../Math/complex.h"
#include <cstdio>
#include <cmath>

// CONSTRUCTOR
PureGauge::PureGauge(double beta) : Action(){
    m_beta = beta;
}

// INITIALIZER
void PureGauge::initAction(Lattice *lattice){
    m_lat = lattice;
}

// COMPUTE THE ACTION DIFFERENCE ON SINGLE LINK AFTER UPDATE, TO BE USED IN
// METROPOLIS ALGORITHM
double PureGauge::compute(int x, int y, int z, int t, int mu,
                          SU3 newLink, SU3 constPart){
    SU3 deltaS = (newLink - (*m_lat)(x,y,z,t)[mu]) * constPart;
    return -m_beta / 3.0 * deltaS.realTrace();
}

// COMPUTE THE SUM OF THE STAPLES OF A GIVEN LINK
SU3 PureGauge::computeConstant(int x, int y, int z, int t, int mu){
    SU3 constPart;
    SU3 A;
    constPart.setSU3Zero();
    for(int nu = 0; nu < 4; nu++){
        if(nu != mu){
            A.setSU3Identity();
            A *= m_lat->shift(x,y,z,t,nu,mu,1);
            A *= ~(m_lat->shift(x,y,z,t,mu,nu,1));
            A *= ~((*m_lat)(x,y,z,t)[nu]);
            constPart = constPart + A;

            A.setSU3Identity();
            A *= ~(  m_lat->shift2(x,y,z,t,nu,nu,-1,mu,1) );
            A *= ~(m_lat->shift(x,y,z,t,mu,nu,-1));
            A *= (m_lat->shift(x,y,z,t,nu,nu,-1));
            constPart = constPart + A;
        }
    }
    return constPart;
}
