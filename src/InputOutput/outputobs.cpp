#include <cstdio>
#include <boost/filesystem.hpp>
#include "Observables/observable.h"
#include "InputOutput/outputobs.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"

namespace LatticeIO {

    char OutputObs::fileName[1024];
    FILE* OutputObs::m_file;
    std::string OutputObs::m_outputDir;

    void OutputObs::setOutputDir(std::string outputDir){
        m_outputDir = outputDir;
        boost::filesystem::create_directory(m_outputDir);
        boost::filesystem::create_directory(m_outputDir+"/flow");
    }

    void OutputObs::initialize(std::vector<Observable*>& obsList){
        if(Parallel::rank() == 0){
            sprintf(fileName, "%s/observables.dat", m_outputDir.c_str());
            m_file = fopen(fileName, "w+");
            fprintf(m_file, "Step\t");
            for(auto& obs : obsList)
                fprintf(m_file, "%s\t", obs->getName());
            fprintf(m_file, "\n");
            fclose(m_file);
        }
    }

    void OutputObs::writeObs(std::vector<Observable*>& obsList, int MCSteps){
        if(Parallel::rank() == 0){
            m_file = fopen(fileName, "a");
            fprintf(m_file, "%i\t", MCSteps);
            for(auto& obs : obsList)
                fprintf(m_file, "%lf\t", obs->value());
            fprintf(m_file, "\n");
            fclose(m_file);
        }
    }

    void OutputObs::writeFlowObs(int confNum, std::vector<Observable*>& obsList, std::vector<std::vector<double>>& obsMatrix){
        if(Parallel::rank() == 0){
            char flowFileName [1024];
            sprintf(flowFileName, "%s/flow/conf%04d.dat", m_outputDir.c_str(), confNum);
            FILE* flowOut = fopen(flowFileName, "w+");
            fprintf(flowOut, "Flow Time\t");
            //for(auto& obs : obsList)
            //    fprintf(flowOut, "%s\t", obs->getName());
            printf("%s\t", "Plaq.");
            printf("%s\t", "Topc.");
            printf("%s\t", "Energ.");
            fprintf(flowOut, "\n");

            for(auto& flowStep : obsMatrix){
                for(auto& obs : flowStep)
                    fprintf(flowOut, "%lf\t", obs);
                fprintf(flowOut, "\n");
            }
            fclose(flowOut);
        }
    }

} // end LatticeIO
