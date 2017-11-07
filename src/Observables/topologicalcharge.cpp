#include "Observables/topologicalcharge.h"
#include "Observables/observable.h"
#include "Math/su3.h"
#include "ParallelTools/parallel.h"
#include "Math/lattice.h"
#include <cstdio>
#include <cmath>

int leviCivita(int i, int j, int k, int l){
    return ( (i-j)*(i-k)*(i-l)*(j-k)*(j-l)*(k-l)/12 );
}

// CONSTRUCTOR
TopologicalCharge::TopologicalCharge() : Observable(){
    m_name = "Top. Charge";
}

// NAME
const char* TopologicalCharge::getName(){
    return m_name.c_str();
}

// INITIALIZER
void TopologicalCharge::initObservable(Lattice* lattice){
    m_lat = lattice;
    m_size = m_lat->getSize();
    m_norm = 1.0 / 32.0 / M_PI / M_PI;
}

// COMPUTE THE VALUE OF THE PLAQUETTE ON THE WHOLE LATTICE
void TopologicalCharge::compute(){
    // loop through the sublattice
    m_value = 0;
    SU3 A;
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
        for(int nu = 0; nu < 4; nu++){
        if(nu!=mu){
            // compute G_mu_nu
            Gmn  =  (*m_lat)(x,y,z,t)[mu]
                 *    m_lat->shift(x,y,z,t,nu, mu, 1)
                 *  ~(m_lat->shift(x,y,z,t,mu, nu, 1))
                 * ~(*m_lat)(x,y,z,t)[nu];

            Gmn +=  (*m_lat)(x,y,z,t)[nu]
                 *  ~(m_lat->shift2 (x,y,z,t,mu,nu,1,mu,-1))
                 *  ~(m_lat->shift(x,y,z,t,nu, mu, -1))
                 *   (m_lat->shift(x,y,z,t,mu, mu, -1));

            Gmn +=  ~(m_lat->shift(x,y,z,t,mu, mu, -1))
                 *  ~(m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,-1))
                 *   (m_lat->shift2 (x,y,z,t,mu,nu,-1,mu,-1))
                 *   (m_lat->shift(x,y,z,t,nu, nu, -1));

            Gmn +=  ~(m_lat->shift(x,y,z,t,nu, nu, -1))
                 *   (m_lat->shift(x,y,z,t,mu, nu, -1))
                 *   (m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,1))
                 * ~(*m_lat)(x,y,z,t)[mu];

            Gmn*=0.25;
            for(int i = 0; i < 18; i+=2)
                Gmn.mat[i] = 0;

            for(int rho = 0; rho < 4; rho++){
            if(rho!=mu && rho != nu){
            for(int sig = 0; sig < 4; sig++){
            if(sig!=mu && sig!=nu && sig!=rho){
                // compute G_rho_sig
                Grs  =  (*m_lat)(x,y,z,t)[rho]
                     *    m_lat->shift(x,y,z,t,sig, rho, 1)
                     *  ~(m_lat->shift(x,y,z,t,rho, sig, 1))
                     * ~(*m_lat)(x,y,z,t)[sig];

                Grs +=  (*m_lat)(x,y,z,t)[sig]
                     *  ~(m_lat->shift2 (x,y,z,t,rho,sig,1,rho,-1))
                     *  ~(m_lat->shift(x,y,z,t,sig, rho, -1))
                     *   (m_lat->shift(x,y,z,t,rho, rho, -1));

                Grs +=  ~(m_lat->shift(x,y,z,t,rho, rho, -1))
                     *  ~(m_lat->shift2 (x,y,z,t,sig,sig,-1,rho,-1))
                     *   (m_lat->shift2 (x,y,z,t,rho,sig,-1,rho,-1))
                     *   (m_lat->shift(x,y,z,t,sig, sig, -1));

                Grs +=  ~(m_lat->shift(x,y,z,t,sig, sig, -1))
                     *   (m_lat->shift(x,y,z,t,rho, sig, -1))
                     *   (m_lat->shift2 (x,y,z,t,sig,sig,-1,rho,1))
                     * ~(*m_lat)(x,y,z,t)[rho];

                Grs*=0.25;
                for(int i = 0; i < 18; i+=2)
                    Grs.mat[i] = 0;
                // multiply and sum
                m_value += (Gmn*Grs).realTrace()*leviCivita(mu, nu, rho, sig);


            }}}}
        }}}
    }}}}

    gatherResults();

    //m_value = m_value / (32 * M_PI * M_PI);
    //m_value = 0.1973 / 0.0931 / 8 * pow(m_value*m_value, 0.25);
}


