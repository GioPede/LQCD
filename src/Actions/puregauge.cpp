#include <cstdio>
#include <cmath>
#include "Actions/puregauge.h"
#include "Math/latticemath.h"

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
                          SU3& newLink, SU3& constPart){
//    SU3 deltaS = (newLink - (*m_lat)(x,y,z,t)[mu]) * constPart;
//    return -m_beta / 3.0 * deltaS.realTrace();
    SU3 c = (newLink - (*m_lat)(x,y,z,t)[mu]);
    return  -m_beta / 3.0 * getMultSumTrace(c, constPart);

}


// COMPUTE THE SUM OF THE STAPLES OF A GIVEN LINK
SU3 PureGauge::computeConstant(int x, int y, int z, int t, int mu){
    constPart.setSU3Zero();
    for(int nu = 0; nu < 4; nu++){
        if(nu != mu){
            staple.setSU3Identity();
            staple *=  (   m_lat->shift(x,y,z,t,nu,mu,1) );
            staple *= ~(   m_lat->shift(x,y,z,t,mu,nu,1)) ;
            staple *= ~( (*m_lat)(x,y,z,t)[nu]);
            constPart = constPart + staple;

            staple.setSU3Identity();
            staple *= ~( m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,1) );
            staple *= ~( m_lat->shift  (x,y,z,t,mu,nu,-1));
            staple *=  ( m_lat->shift  (x,y,z,t,nu,nu,-1));
            constPart = constPart + staple;
        }
    }
    return constPart;
}
