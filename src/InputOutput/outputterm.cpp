#include <cstdio>
#include <mpi.h>
#include "InputOutput/outputterm.h"
#include "Apps/app.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"
#include "Utils/clusterspecifier.h"
#include "Observables/observable.h"

// CONSTRUCTOR
OutputTerm::OutputTerm(App *app){
    m_app = app;
    m_write = m_app->getParallel()->getRank() == 0 ? 1 : 0;
}

void OutputTerm::printInitialConditions(){
    if(m_write){
        printf("Total Lattice Size: %2i %2i %2i %2i\n", m_app->getParallel()->getFullSize()[0],m_app->getParallel()->getFullSize()[1],
                m_app->getParallel()->getFullSize()[2],m_app->getParallel()->getFullSize()[3]);
        printf("Sublattice Size:    %2i %2i %2i %2i\n\n", m_app->getSize()[0],m_app->getSize()[1],
                m_app->getSize()[2],m_app->getSize()[3]);
    }
}

void OutputTerm::printThermSteps(int step, double acceptRatio){
    if(m_write){
        printf("Thermalization Step %4i\n", step);
        for(int i = 0; i < m_app->getObsValues().size(); i++)
            printf("\tPlaq = %lf \n", m_app->getObsValues()[i]);
        printf("\tAccept. Ratio = %lf\n", acceptRatio);
    }
}

void OutputTerm::printGenerationStep(int confNum, double acceptRatio){
    if(m_write){
        printf("Configuration Number %4i\n", confNum);
        for(int i = 0; i < m_app->getObsValues().size(); i++)
            printf("\tPlaq = %lf \n", m_app->getObsValues()[i]);
        printf("\tAccept. Ratio = %lf\n", acceptRatio);
    }
}

