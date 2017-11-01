#pragma once
#include <vector>
#include "Math/lattice.h"
#include "Observables/observable.h"

namespace LatticeIO {

    class OutputTerm{
    public:
        static void printInitialConditions();
        static void writeObs(int confNum, std::vector<Observable*>& obsList);
        static void printThermStep(int step, std::vector<Observable*>& obsList, double acceptRatio);
        static void printGenStep(int confNum, std::vector<Observable*>& obsList, double acceptRatio);
        static void writeFlowObs(double flowTime, std::vector<Observable*>& obsList);
    };

} // end LatticeIO
