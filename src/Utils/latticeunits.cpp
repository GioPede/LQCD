#include "Utils/latticeunits.h"
#include "ParallelTools/parallel.h"
#include <array>
#include <string>
#include <cmath>
#include <numeric>

int LatticeUnits::Nc = 0;
int LatticeUnits::latticeSites = 0;
double LatticeUnits::beta = 0;
double LatticeUnits::latticeVolume = 0;
double LatticeUnits::latticeSpacing = 0;
std::array<int,4> LatticeUnits::size;

void LatticeUnits::initialize(double beta){
    LatticeUnits::Nc = 3;
    LatticeUnits::beta = beta;
    LatticeUnits::latticeSpacing = calculateLatticeSpacing(beta);

    LatticeUnits::size = Parallel::latticeFullSize();
    LatticeUnits::latticeSites = std::accumulate(LatticeUnits::size.begin(),
                                                 LatticeUnits::size.end(), 1,
                                                 std::multiplies<int>());

    LatticeUnits::latticeVolume = pow(LatticeUnits::latticeSpacing, 4)
                                * LatticeUnits::latticeSites;
}

double LatticeUnits::calculateLatticeSpacing(double beta){
    return 0.5 * exp(-1.6805 - 1.7139*(beta-6.0) + 0.8155*(beta-6)*(beta-6.0)
                     -0.6667*(beta-6.0)*(beta-6.0)*(beta-6.0));
}
