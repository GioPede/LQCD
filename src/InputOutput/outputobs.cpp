#include <cstdio>
#include <mpi/mpi.h>
#include "Observables/observable.h"
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
        fprintf(m_file, "Step\t");
        for(int i = 0; i < m_app->getObsValues().size(); i++)
            fprintf(m_file, "%s\t", m_app->getObs()[i]->getName());
        fprintf(m_file, "\n");
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

void OutputObs::writeFlowObservables(int confNum, std::vector<std::vector<double>> obsMatrix){
    if(m_write){
        char fileName [128];
        sprintf(fileName, "%s/%s/flow/conf%04d.dat", OUT_PREFIX, m_app->getOutDir(), confNum);
        FILE* flowOut = fopen(fileName, "w+");
        fprintf(flowOut, "Flow Time\t");
        for(int i = 0; i < m_app->getObsValues().size(); i++)
            fprintf(flowOut, "%s\t", m_app->getObs()[i]->getName());
        fprintf(flowOut, "\n");

        for(int i = 0; i < obsMatrix.size(); i++){
            for(int j = 0; j < obsMatrix[i].size(); j++)
                fprintf(flowOut, "%lf\t", obsMatrix[i][j]);
            fprintf(flowOut, "\n");
        }
        fclose(flowOut);
    }
}

