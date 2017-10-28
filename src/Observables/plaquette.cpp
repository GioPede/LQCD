#include "Observables/plaquette.h"
#include "Observables/observable.h"
#include "Math/su3.h"
#include "Math/lattice.h"
#include <cstdio>
#include <cmath>
#include <string>

// CONSTRUCTOR
Plaquette::Plaquette() : Observable(){
    m_name = "Plaquette";
}

// NAME
const char* Plaquette::getName(){
    return m_name.c_str();
}

// INITIALIZER
void Plaquette::initObservable(Lattice* lattice){
    m_lat = lattice;
    m_size = m_lat->getSize();
    m_norm = 1.0 / 18.0 / m_size[0] / m_size[1] / m_size[2] / m_size[3];
}

// COMPUTE THE VALUE OF THE PLAQUETTE ON THE WHOLE LATTICE
void Plaquette::compute(){
    // loop through the sublattice
    m_value = 0;
    for(int mu = 0; mu < 4; mu++){
        for(int nu = mu + 1; nu < 4; nu++){
            for(int x = 0; x < m_size[0]; x++){
            for(int y = 0; y < m_size[1]; y++){
            for(int z = 0; z < m_size[2]; z++){
            for(int t = 0; t < m_size[3]; t++){
                plaq.setSU3Identity();
                plaq *= (*m_lat)(x,y,z,t)[mu];
                plaq *= m_lat->shift(x,y,z,t,nu, mu, 1);
                plaq *= ~(m_lat->shift(x,y,z,t,mu, nu, 1));
                plaq *= ~(*m_lat)(x,y,z,t)[nu];
                m_value += plaq.realTrace();
            }}}}
        }
    }
    m_value *= m_norm;
}
