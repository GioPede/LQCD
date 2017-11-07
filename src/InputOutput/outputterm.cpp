#include <cstdio>
#include "InputOutput/outputterm.h"
#include "Math/lattice.h"
#include "ParallelTools/parallel.h"
#include "Observables/observable.h"

namespace LatticeIO {

    void OutputTerm::printInitialConditions(){
        if(Parallel::rank() == 0){
            printf("Total Lattice Size: %2i %2i %2i %2i\n", Parallel::latticeFullSize()[0],Parallel::latticeFullSize()[1],
                                                            Parallel::latticeFullSize()[2], Parallel::latticeFullSize()[3]);
            printf("Sublattice Size:    %2i %2i %2i %2i\n\n", Parallel::latticeSubSize()[0],Parallel::latticeSubSize()[1],
                                                              Parallel::latticeSubSize()[2],Parallel::latticeSubSize()[3]);
        }
    }

    void OutputTerm::printThermStep(int step, std::vector<Observable*>& obsList, double acceptRatio){
        if(Parallel::rank() == 0){
            printf("Thermalization Step %4i\n", step);
            for(auto& obs : obsList)
                printf("\t%s\t = %lf \n", obs->getName(), obs->value());
            printf("\tAccept. Ratio = %lf\n", acceptRatio);
        }
    }

    void OutputTerm::printGenStep(int confNum, std::vector<Observable*>& obsList, double acceptRatio){
        if(Parallel::rank() == 0){
            printf("Configuration Number %4i\n", confNum);
            for(auto& obs : obsList)
                printf("\t%s\t = %lf \n", obs->getName(), obs->value());
            printf("\tAccept. Ratio = %lf\n", acceptRatio);
        }
    }

    void OutputTerm::writeObs(int confNum, std::vector<Observable*>& obsList){
        if(Parallel::rank() == 0){
            printf("Configuration Number %4i\n", confNum);
            //or(auto& obs : obsList)
            //    printf("\t%s\t = %lf \n", obs->getName(), obs->value());
            printf("\t%s\t = %g \n", "Plaq.", obsList[0]->plaq);
            printf("\t%s\t = %g \n", "Topc.", obsList[0]->topc);
            printf("\t%s\t = %g \n", "Energ.", obsList[0]->energy);
            }

    }

    void OutputTerm::writeFlowObs(double flowTime, std::vector<Observable*>& obsList){
        if(Parallel::rank() == 0){
            printf("Flow Time %2.2lf\n", flowTime);
            //for(auto& obs : obsList)
            //    printf("\t%s\t = %lf \n", obs->getName(), obs->value());
            printf("\t%s\t = %g \n", "Plaq.", obsList[0]->plaq);
            printf("\t%s\t = %g \n", "Topc.", obsList[0]->topc);
            printf("\t%s\t = %g \n", "Energ.", obsList[0]->energy);
        }
    }

} // end LatticeIO
