#include <cstdio>
#include <mpi/mpi.h>
#include "InputOutput/outputterm.h"
#include "Apps/app.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"
#include "Utils/clusterspecifier.h"
#include "Observables/observable.h"

// CONSTRUCTOR
OutputTerm::OutputTerm(App *app){
    m_app = app;
    m_write = Parallel::rank() == 0 ? 1 : 0;
}

void OutputTerm::printInitialConditions(){
    if(m_write){
        printf("Total Lattice Size: %2i %2i %2i %2i\n", Parallel::latticeFullSize()[0],Parallel::latticeFullSize()[1],
               Parallel::latticeFullSize()[2], Parallel::latticeFullSize()[3]);
        printf("Sublattice Size:    %2i %2i %2i %2i\n\n", m_app->getSize()[0],m_app->getSize()[1],
                m_app->getSize()[2],m_app->getSize()[3]);
    }
}

void OutputTerm::printThermSteps(int step, double acceptRatio){
    if(m_write){
        printf("Thermalization Step %4i\n", step);
        for(int i = 0; i < m_app->getObsValues().size(); i++)
            printf("\t%s\t = %lf \n", m_app->getObs()[i]->getName(), m_app->getObsValues()[i]);
        printf("\tAccept. Ratio = %lf\n", acceptRatio);
    }
}

void OutputTerm::printGenerationStep(int confNum, double acceptRatio){
    if(m_write){
        printf("Configuration Number %4i\n", confNum);
        for(int i = 0; i < m_app->getObsValues().size(); i++)
            printf("\t%s\t = %lf \n", m_app->getObs()[i]->getName(), m_app->getObsValues()[i]);
        printf("\tAccept. Ratio = %lf\n", acceptRatio);
    }
}

void OutputTerm::writeObservables(int confNum){
    if(m_write){
        printf("Configuration Number %4i\n", confNum);
        for(int i = 0; i < m_app->getObsValues().size(); i++)
            printf("\t%s\t = %lf \n", m_app->getObs()[i]->getName(), m_app->getObsValues()[i]);
    }
}

void OutputTerm::writeFlowObservables(std::vector<double> obsVector){
    if(m_write){
        printf("Flow Time %2.2lf\n", obsVector[0]);
        for(int i = 1; i < obsVector.size(); i++)
            printf("\t%s\t = %lf \n", m_app->getObs()[i-1]->getName(), obsVector[i]);
    }
}

