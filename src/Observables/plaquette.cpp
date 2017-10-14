#include "Observables/plaquette.h"
#include "Observables/observable.h"
#include "Math/su3.h"
#include "Math/lattice.h"
#include <cstdio>
#include <cmath>

// CONSTRUCTOR
Plaquette::Plaquette() : Observable(){

}

// INITIALIZER
void Plaquette::initObservable(Lattice* lattice){
    m_lat = lattice;
    m_size = m_lat->getSize();
}

// COMPUTE THE VALUE OF THE PLAQUETTE ON THE WHOLE LATTICE
void Plaquette::compute(){
    // loop through the sublattice
    SU3 plaq;
    double sum = 0;
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
                sum += plaq.realTrace();
            }}}}
        }
    }
    m_value =  sum / 18 / m_size[0] / m_size[1] / m_size[2] / m_size[3] ;
}
