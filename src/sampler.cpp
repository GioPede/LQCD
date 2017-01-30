#include <cstdio>
#include <cmath>
#include <vector>
#include "sampler.h"
#include "system.h"
#include "lattice.h"

Sampler::Sampler(System* system) {
    m_system = system;
    m_stepNumber = 0;
    m_numberOfMetropolisSteps = m_system->getNumberOfMetropolisSteps();
}
/*
// Make sure the sampling variable(s) are initialized at the first step.
void Sampler::initializeFirstStep(){
    m_cumulativeEnergy = 0;
    m_cumulativeEnergySquared = 0;
    m_localEnergy = m_system->getHamiltonian()->computeLocalEnergy(m_system->getParticles());
    m_dePsideAlpha = 0.0;
    for (int i=0; i < m_system->getNumberOfParticles(); i++) {
        for (int j=0; j < m_system->getNumberOfDimensions() - 1; j++) {
            m_dePsideAlpha -= m_system->getParticles()[i]->getPosition()[j]*m_system->getParticles()[i]->getPosition()[j];
        }
        int j = m_system->getNumberOfDimensions() - 1;
        m_dePsideAlpha -= m_dePsideAlpha -= m_system->getParticles()[i]->getPosition()[j]*m_system->getParticles()[i]->getPosition()[j]*
                          m_system->getWaveFunction()->getVariationalParameters()[1];
    }
    m_dePsideAlphaELocal = m_dePsideAlpha * m_localEnergy;
}
*/


// PERFORM THE ACTUAL SAMPLING
void Sampler::sample(bool acceptedStep) {
    if(acceptedStep){

    }
}

void Sampler::printOutputToTerminal() {
    printf("hola! \n");
}

void Sampler::computeAverages() {

}
