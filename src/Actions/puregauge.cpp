#include <cstdio>
#include <cmath>
#include "Actions/puregauge.h"
#include "Math/latticemath.h"

// COMPUTE THE ACTION DIFFERENCE ON SINGLE LINK AFTER UPDATE, TO BE USED IN
// METROPOLIS ALGORITHM
double PureGauge::compute(int x, int y, int z, int t, int mu,
                          SU3& newLink, SU3& constPart){
//    SU3 deltaS = (newLink - (*m_lat)(x,y,z,t)[mu]) * constPart;
//    return -m_beta / 3.0 * deltaS.realTrace();
    return  -m_beta / 3.0 * getMultSumTrace(newLink - (*m_lat)(x,y,z,t)[mu], constPart);

}


// COMPUTE THE SUM OF THE STAPLES OF A GIVEN LINK
SU3& PureGauge::computeConstant(int x, int y, int z, int t, int mu){
    constPart.setSU3Zero();
    for(int nu = 0; nu < 4; nu++){
        if(nu != mu){
            staple =  (   m_lat->shift(x,y,z,t,nu,mu,1) );
            staple *= ~(   m_lat->shift(x,y,z,t,mu,nu,1)) ;
            staple *= ~( (*m_lat)(x,y,z,t)[nu]);
            constPart += staple;

            staple = ~( m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,1) );
            staple *= ~( m_lat->shift  (x,y,z,t,mu,nu,-1));
            staple *=  ( m_lat->shift  (x,y,z,t,nu,nu,-1));
            constPart += staple;
        }
    }
    return constPart;
}
