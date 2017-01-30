#include <cstdio>
#include <vector>
#include "system.h"
#include "lattice.h"
#include "Actions/action.h"
#include "Actions/puregauge.h"


int main(){

    // GENERAL SIMULATION PARAMETERS
    std::vector<int> lattice_size = {4, 4, 4, 8};
    int numberOfSteps       = (int) 1e3;
    double equilibration    = 0.1;

    // CREATE THE SYSTEM
    System* system = new System();
    system->setLattice  (new Lattice(lattice_size));
    system->setAction   (new PureGauge(system));

    system->setEquilibrationFraction    (equilibration);

    // RUN THE SIMULATION
    system->runMetropolisSteps(numberOfSteps);

    return 0;
}

