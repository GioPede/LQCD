#pragma once
#include <vector>
#include <string>
#include "Observables/observable.h"

namespace LatticeIO {

    class OutputObs{
    public:
        static void initialize(std::vector<Observable*>& obsList);
        static void writeObs(std::vector<Observable*>& obsList, int MCSteps);
        static void writeFlowObs(int confNum, std::vector<Observable*>& obsList, std::vector<std::vector<double>>& obsMatrix);
        static void setOutputDir(std::string outputDir);

    private:
        static FILE* m_file;
        static char fileName[1024];
        static std::string m_outputDir;

    };

} // end LatticeIO
