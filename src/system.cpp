#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "system.h"
#include "sampler.h"
#include "lattice.h"
#include "Actions/action.h"


// BASIC METROPOLIS STEP, NO IMPORTANCE SAMPLING
bool System::metropolisStep() {
    /*
    // SUGGEST A MOVE
    double deltaR = (m_random->nextDouble() - 0.5) * m_stepLength;
    double dimensionOfChange = m_random->nextInt(m_numberOfDimensions);
    int selectedParticleIndex = m_random->nextInt(m_numberOfParticles);
    Particle* selectedParticle = m_particles[selectedParticleIndex];
    selectedParticle->adjustPosition(deltaR, dimensionOfChange);

    m_waveFunction->updateSlaterMatrix(selectedParticleIndex);

    double slaterRatio  = m_waveFunction->computeSlaterRatio(selectedParticleIndex);
    double jastrowRatio = m_waveFunction->computeJastrowRatio (selectedParticleIndex);
    //double jastrowRatio = 1.0;
    //updateDistanceMatrix();
    
    //printf("R = %f\n", slaterRatio* jastrowRatio*slaterRatio* jastrowRatio);
    
    // ACCEPT OR REJECT THE MOVE
    if (slaterRatio*slaterRatio*jastrowRatio*jastrowRatio > m_random->nextDouble()){
        //m_waveOld = newWaveValue;
        m_waveFunction->updateInverseSlaterMatrix(selectedParticleIndex, slaterRatio);
        m_waveFunction->computeNewQuantumForce(m_particles);
        m_waveFunction->substituteQuantumForce();
        return true;
    }
    else {
        selectedParticle->adjustPosition(-deltaR, dimensionOfChange);
        updateDistanceMatrix(selectedParticleIndex);
        m_waveFunction->updateSlaterMatrix(selectedParticleIndex);
        return false;
    }*/
}



// MAIN FUNCTION OF THE PROGRAM, LOOP OVER THE MC CYCLES
void System::runMetropolisSteps(int numberOfMetropolisSteps) {
    // INITIALIZE THE WHOLE SYSTEM
    m_sampler = new Sampler(this);


    // CREATE THE SYSTEM

    // START THE METROPOLIS ALGORITHM
    m_numberOfMetropolisSteps   = numberOfMetropolisSteps;

    // thermalization
    for (int i=0; i < numberOfMetropolisSteps * m_equilibrationFraction; i++)
        metropolisStep();

    // sampling
    for (int i = numberOfMetropolisSteps * m_equilibrationFraction; i < numberOfMetropolisSteps; i++){
        bool acceptedStep = metropolisStep();
        m_sampler->sample(acceptedStep);
    }

    // COMPUTE AVERAGES AND EXIT
    m_sampler->computeAverages();


    // PRINT OUTPUT
    m_sampler->printOutputToTerminal();
}
