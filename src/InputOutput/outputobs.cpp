#include <cstdio>
#include <mpi.h>
#include "InputOutput/outputobs.h"
#include "Apps/app.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"
#include "Utils/clusterspecifier.h"

OutputObs::OutputObs(App *app){
    m_app = app;
    m_write = m_app->getParallel()->getRank() == 0 ? 1 : 0;
    if(m_write){
        char fileName [128];
        sprintf(fileName, "%s/%s/observables.dat", OUT_PREFIX, m_app->getOutDir());
        m_file = fopen(fileName, "w+");
    }
}

void OutputObs::writeObservables(int MCSteps){
    if(m_write){
        fprintf(m_file, "%i\t", MCSteps);
        for(int i = 0; i < m_app->getObsValues().size(); i++)
            fprintf(m_file, "%lf\t", m_app->getObsValues()[i]);
        fprintf(m_file, "\n");
    }
}

void OutputObs::closeFile(){
    if(m_write){
        fclose(m_file);
    }
}


