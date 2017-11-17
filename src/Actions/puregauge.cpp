#include <cstdio>
#include <cmath>
#include "Actions/puregauge.h"
#include "Math/latticemath.h"
#include "Utils/latticeunits.h"

// COMPUTE THE ACTION DIFFERENCE ON SINGLE LINK AFTER UPDATE, TO BE USED IN
// METROPOLIS ALGORITHM
PureGauge::PureGauge(double beta){
    m_beta = beta;
    LatticeUnits::initialize(m_beta);
}

double PureGauge::compute(int x, int y, int z, int t, int mu, SU3& newLink){
    return -m_beta / 3.0 * getMultSumTrace(newLink - (*m_lat)(x,y,z,t)[mu], m_constPart);

}


// COMPUTE THE SUM OF THE STAPLES OF A GIVEN LINK
void PureGauge::computeStaples(int x, int y, int z, int t, int mu){
    m_constPart.setSU3Zero();
    for(int nu = 0; nu < 4; nu++){
        if(nu != mu){
            m_constPart += (   m_lat->shift(x,y,z,t,nu,mu,1) )
                        * ~(   m_lat->shift(x,y,z,t,mu,nu,1))
                        * ~( (*m_lat)(x,y,z,t)[nu]);

            m_constPart += ~( m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,1) )
                        *  ~( m_lat->shift  (x,y,z,t,mu,nu,-1))
                        *   ( m_lat->shift  (x,y,z,t,nu,nu,-1));
        }
    }
}


SU3 PureGauge::computeDerivative(int x, int y, int z, int t, int mu){
    computeStaples(x, y, z, t, mu);
    m_omega = (~m_constPart) * (~(*m_lat)(x,y,z,t)[mu]);
    m_omega = (~m_omega-m_omega);
    double tr = m_omega.imagTrace()/3.0;
    for(int i = 1; i < 18; i+=8)
        m_omega.mat[i] -= tr;

    //m_omega*=0.5;
    for(int i = 0; i < 18; i+=2){
        double temp = m_omega.mat[i];
        m_omega.mat[i] = -m_omega.mat[i+1]*0.5;
        m_omega.mat[i+1] = temp*0.5;
    }
    return std::move(m_omega);
}
