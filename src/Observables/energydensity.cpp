#include "Observables/energydensity.h"
#include "Observables/observable.h"
#include "Math/su3.h"
#include "Math/lattice.h"
#include <cstdio>
#include <cmath>


// CONSTRUCTOR
EnergyDensity::EnergyDensity() : Observable(){
    m_name = "Energy Dens.";
}

// NAME
const char* EnergyDensity::getName(){
    return m_name.c_str();
}

// INITIALIZER
void EnergyDensity::initObservable(Lattice* lattice){
    m_lat = lattice;
    m_size = m_lat->getSize();
    m_norm = 1.0 / m_size[0] / m_size[1] / m_size[2] / m_size[3] / 4.0 / 18.0;
}

// COMPUTE THE VALUE OF THE PLAQUETTE ON THE WHOLE LATTICE
void EnergyDensity::compute(){
    // loop through the sublattice
    m_value = 0;
    SU3 A;
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){
        for(int mu = 0; mu < 4; mu++){
        for(int nu = mu+1; nu < 4; nu++){
            // compute G_mu_nu
            Gmn.setSU3Zero();
            A.setSU3Identity();
            A *= (*m_lat)(x,y,z,t)[mu];
            A *= m_lat->shift(x,y,z,t,nu, mu, 1);
            A *= ~(m_lat->shift(x,y,z,t,mu, nu, 1));
            A *= ~(*m_lat)(x,y,z,t)[nu];
            Gmn+=A;

            A.setSU3Identity();
            A *= (*m_lat)(x,y,z,t)[nu];
            A *= m_lat->shift2 (x,y,z,t,mu,nu,1,mu,-1);
            A *= ~(m_lat->shift(x,y,z,t,nu, mu, -1));
            A *= (m_lat->shift(x,y,z,t,mu, mu, -1));
            Gmn+=A;

            A.setSU3Identity();
            A *= ~(m_lat->shift(x,y,z,t,mu, mu, -1));
            A *= ~(m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,-1));
            A *= (m_lat->shift2 (x,y,z,t,mu,nu,-1,mu,-1));
            A *= (m_lat->shift(x,y,z,t,nu, nu, -1));
            Gmn+=A;

            A.setSU3Identity();
            A *= ~(m_lat->shift(x,y,z,t,nu, nu, -1));
            A *= (m_lat->shift(x,y,z,t,mu, nu, -1));
            A *= (m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,1));
            A *= ~(*m_lat)(x,y,z,t)[mu];
            Gmn+=A;
            Gmn*=0.25;
            for(int i = 0; i < 18; i+=2)
                Gmn.mat[i] = 0;
            A.setSU3Identity();
            m_value += (A - (Gmn*Gmn)).realTrace();
        }}
    }}}}
    m_value *= m_norm;
}


